#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include "build_spec_repr.h"
#include "build_spec_graph.h"
#include "proc_creation_proc_exe.h"

//this function is to check whether child file is newer than parent file
int isNew(char* parent, char* children) {
	struct stat pattr;
	struct stat cattr;
	//get the file status struct from parent and children 
	stat(parent, &pattr);
	stat(children, &cattr);

	//if children was modified later than parent
	if ((cattr.st_mtime - pattr.st_mtime) > 0) {
		return 1;
	}
	else return 0;
}

//this function is to check whether the file exists in the folder
int isExist(char* filename) {
	struct stat attr;

	//check if file exist
	int exist = stat(filename, &attr);
	if (exist == 0)
		return 1;
	else
		return 0;
}

//this function is to compile a command line
void execution(char* cmdline)
{
	//record which char is processing	
	int count = 0;
	//record number of argument stored
	int argNum = 0;
	//record length of each argumen
	int argLen = 0;
	//signal that argNum can increase by one
	int sig = 0;
	//store the splited cmd line, there'll  
	//be maximux size/2 seperate argument
	int size = 1024;
	char** myargs = malloc((size / 2) * sizeof(char *));
	for (int i = 0; i<size / 2; i++) {
		myargs[i] = malloc(size * sizeof(char));
	}

	while (cmdline[count] != '\0') {
		//if a character other than space and tap is read, 
		//add into ret array
		while ((cmdline[count] != ' ') && (cmdline[count] != '\t') &&
			(cmdline[count] != '\0'))
		{
			//store the char into ret	    
			*(*(myargs + argNum) + argLen) = cmdline[count];
			argLen++;
			//mark that one arg has been added 
			sig = 1;
			count++;
		}
		count++;
		*(*(myargs + argNum) + argLen + 1) = '\0';
		//zero argLen for reading the next arg
		argLen = 0;
		if (sig) {
			argNum++;
			sig = 0;
		}
	}
	myargs[argNum] = NULL;
	
	//start to execute the command
	int rc = fork();
	//fork failed; exit
	if (rc < 0) {
		fprintf(stderr, "fork failed\n");
		exit(1);
	}
	//if in the child process then execute
	else if (rc == 0) {
		if (execvp(myargs[0], myargs) == -1) {
			fprintf(stderr, "Error: the command are not "
					"executable!\n");
            exit(1);
		}
	}
	//wait until child process has done
	else {
		wait(NULL);
	}
}


/*This is a method to compile the commands of the given node. To do so, 
 * we need to check whether the children are file or not. If at least 
 * some children are file, we need to check whether child file exist or not.
 * If child file exist, we need to compare whether the parent file exist if: 
 * 1.parent file exist but older than children file; 2. parent file does not 
 * exist; 3. parent is not a file. For any one of the three above, compile 
 * parent's commands 
 */
void CompileorNot(GraphNode *gn) {
	//a count to see whether any child is new than already existed gn file
	int ChildrenNew = 0;
	//a count to see whether gn file existed
	int gnExist = isExist(gn->name);
    if ((gn->target == 1) && (gn->outdegree == 0)) {
	gn->isFile = 0;
    }
    
    int ChildisFile = 0;
	//if gn has children
	if (gn->outdegree > 0) {
	//check whether children exists in directory or not
	for (int i = 0; i < gn->outdegree; i++) {
		GraphNode *child = gn->children[i];
	    //check whether the child is a file or not
            //only if child is a file we need to compare parent with 
	    //child's update time and whether exist child
            if (child->isFile == 1) {
                ChildisFile ++;
                //if children does not exist, exit
                if (isExist(child->name) == 0) {
                    fprintf(stderr, "Error: file %s does not exist"
				    " in the directory\n", child->name);
                    exit(1);
                }

                //check whether the children is newer than parent
                if (ChildrenNew == 0) {
                    //check whether parent file exist
                    if (gnExist) {
                        //compare time of parent and child
                        if (isNew(gn->name, child->name)) {
                            ChildrenNew = 1;
                        } 
                    }
                }
            }
			
	}
	}
    
    //if all children of gn is not file, gn is not a file
    if ((ChildisFile == 0) && (gn->target == 1)){
      gn->isFile = 0;
    }
          //compile again when one of the children is modified 
	  //later than its parent.
	  //gn does not exist
	  //or there are childern newer than the time gn exis 
	  if ((gnExist == 0) || (gn->isFile == 0) || (ChildrenNew == 1)) {
         	  for (int j = 0; j < gn->commands; j++) {
		  	  printf("%s\n",gn->cmd[j]);
			  execution(gn->cmd[j]);
		  }
	  }
}

/*This method is to execute each line of makefile using the
result of nodes in a post-order*/
void ExecuteEveryNode(graph *gg) {
	/*traverse from last to first.
	ensure each node's children, grandchildren, grandgrandchildren,...
	always execute before the node itself.*/
	for (int i = gg->nodereal - 1; i >=0; i--) {
		GraphNode *LastOne = gg->nodes[i];
		//compile each node's commands
		CompileorNot(LastOne);
	}
	return;
}
