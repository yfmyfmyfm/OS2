#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "build_spec_graph.h"
#include "build_spec_repr.h"
#include "text_parsing.h"

/*Description:
 *  This method is used to parse the target line and 
 *  create or update target file or dependence node by
 *  calling methods in build_graph method.
 */

char* tar_parsing(graph* mGraph, char* str, int size, int lineNum){
  //record if space detected within target file
  int isSpace = 0;
  int count = 0;
  int buffersize = 1024;
  char* buffer = malloc(buffersize*sizeof(char));
  char* ret = malloc(buffersize*sizeof(char));
  GraphNode* tarNode;
  //first to get the target file from buffer
  while ((str[count]!= ':') && (count < size)){
    //error if space appear before colon
    if (str[count] == ' '){
      isSpace = 1;
      count++;
    }
    //error if tap appears befor colon
    else if (str[count] == '\t'){
      fprintf(stderr,"%d: Invalid line: Tap dectected in " 
		     "Target file.\n", lineNum);
      exit(1);
    }
    //store the char into buffer
    else{
      if (isSpace){
        fprintf(stderr,"%d: Invalid line: Space detected "
			"in Target file.\n", lineNum);
        exit(1);  
      }
      buffer[count] = str[count]; 
      count++;
    }
  }
  //cannot find a colon until the end of the line
  if (count == size){
    fprintf(stderr, "%d: Invalid line: Cannot "
		    "find Target file\n", lineNum);
    exit(1);
  }  
  //create/update a node for the target file
  else{
    tarNode = inornot(mGraph,buffer);
    tarNode->target = 1;
    strcpy(ret, buffer);
  }
  //skip the colon just found
  count++;
 
  //record the length of each dependence
  int de_count = 0; 
  buffer = malloc(buffersize*sizeof(char)); 
  //get dependence from buffer
  while(count < size){
    
    //skip space between dependence
    if (str[count] == ' '){
      count++;
    }
    //error if tap is found between denpendence
    if (str[count] == '\t'){
      fprintf(stderr, "%d: Invalid line: dependence can "
		      "only be seperated by spaces.\n", lineNum);
      exit(1);
    }
    //add a dependence to a new buffer
    while((str[count] != ' ') && (count < size)){ 
      buffer[de_count] = str[count];
      count++;
      de_count++;      
    }
   
    //create/update a node for the dependence and added as 
    //children to target node
    if (de_count != 0){
      buffer[de_count+1] = '\0';
      de_count = 0;
      GraphNode* deNode = inornot(mGraph,buffer);
      addchildren(tarNode, deNode);     
      buffer = malloc(buffersize*sizeof(char));
    }

  }
 return ret; 
}

/*This method is used to acquire the command for each target file
 */
void cmd_parsing(char* buffer, int size, GraphNode* tarNode, int lineNum){
  //check if the cmd line is empty
  int isEmpty = 1;
  for(int i = 0; i<size; i++){
    if ((buffer[i] != '\t') && (buffer[i] != ' ') && (buffer[i] != '\0'))
      isEmpty = 0;
  }
  if (isEmpty){
    fprintf(stderr,"%d: Invalid line: command line cannot"
		  " be an empty line\n", lineNum);
    exit(1);
  }
  else{
    //add command into corresponding node    
    addcommands(tarNode, buffer); 
  }

}









