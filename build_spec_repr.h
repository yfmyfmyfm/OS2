#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifndef build_spec_repr_h
#define build_spec_repr_h
/*define a data structure, graphnode, which correspond to each target or dependency.*/
typedef struct GraphNode{
        char *name;
        struct GraphNode **children;
        char **cmd;
        //indegree is from node's parent to node itself. 
        //count how many parents this node has
        int indegree;
        //count how many children this node has
        int outdegree;
        //malloc children size
        int childrensize;
        //count how many commands there are
        int commands;
        //malloc commandsize
        int cmdsize;
        int visitedsubgraph;
        int visitedwhole;
        int target;
        int isFile;
}GraphNode;
GraphNode *createnode(char *name);
void addchildren(GraphNode *gn, GraphNode *newchildren);
void addcommands(GraphNode *gn, char *thiscmd);
void printeverything(GraphNode *gn);
#endif
