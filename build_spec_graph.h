#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "build_spec_repr.h"

#ifndef build_spec_graph_h
#define build_spec_graph_h
/*This is to define a data structure called graph 
which store all possible nodes
*/
typedef struct graph{
        GraphNode ** nodes;
        int nodemalloc;
        int nodereal;
}graph;
graph *creategraph();
GraphNode *addnode(graph *gg, char *name);
GraphNode *inornot(graph *gg, char *name);
GraphNode *getNode(graph *gg, char *name);
void printgraph(graph *gg);
void addrealnode(graph *gg, GraphNode *gn);
void inornotreal(graph *gg, GraphNode *gn);
#endif 
