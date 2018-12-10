#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "build_spec_repr.h"
GraphNode *createnode(char *name) {
        //set a graphnode
        GraphNode *gn = (GraphNode *) malloc(sizeof(GraphNode));
        //assign name of node to name
        gn->name = name;
        //assign length of list of children
        gn->childrensize = 32;
        //test case: 1
        // gn->childrensize = 1;
        //assign list of children
        gn->children = malloc((gn->childrensize)*sizeof(GraphNode *));
        //assign length of list of commands
        gn->cmdsize  = 32;
        //test case: 1
        // gn->cmdsize  = 1;
        //assign list of commands
        gn->cmd = malloc((gn->cmdsize)*sizeof(char *));
        //parent's number
        gn->indegree = 0;
        //children's real number
        gn->outdegree = 0;
        //command's real number
        gn->commands = 0;
        gn->visitedsubgraph = 0;
        gn->visitedwhole = 0;
        gn->target = 0;
        gn->isFile = 1;
        return gn;
}

/*This function is adding children nodes */
void addchildren(GraphNode *gn, GraphNode *newchildren) {
     
	//if the original malloc size of children is out of space, we need to double the size.
        if ((gn->outdegree)>=(gn->childrensize)) {
                gn->childrensize = 2*(gn->childrensize);
                gn->children = realloc(gn->children, (gn->childrensize)*sizeof(GraphNode *));
        }
        //add newchildren to gn's list of children
        gn->children[gn->outdegree] = newchildren;
        //add total children's number of gn
        gn->outdegree++;
        //add one more parent to newchildren
        newchildren->indegree++;  
}

/*This function is adding children nodes */
void addcommands(GraphNode *gn, char *thiscmd) {
        //if the original malloc size of command is out of space, we need to double the size.
   	if ((gn->commands)>=(gn->cmdsize)) {
                gn->cmdsize = 2*(gn->cmdsize);
                gn->cmd = realloc(gn->cmd, (gn->cmdsize)*sizeof(char *));
        }
	
        //add new command to gn's list of commands
        gn->cmd[gn->commands] = thiscmd;
        //add total command's number of gn
        gn->commands++; 
}
