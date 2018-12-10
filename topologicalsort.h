#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "build_spec_graph.h"
#include "build_spec_repr.h"

#ifndef topologicalsort_h
#define topologicalsort_h
typedef struct nodequeue{
    int capacity;
    int size;
    int first;
    int last;
    GraphNode **elements;
}nodequeue;
nodequeue *createqueue();
void enqueue(nodequeue *q, GraphNode *string);
GraphNode *dequeue(nodequeue *q);
graph *topologicalsort(graph *gg);
void getnamee(graph *gg, graph *newgg);
graph *subgraph(graph *allgraph, char *startname);
#endif
