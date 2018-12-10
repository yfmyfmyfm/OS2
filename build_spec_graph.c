#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "build_spec_repr.h"
#include "build_spec_graph.h"


//This function is to create a new graph
graph *creategraph() {
	graph *gg = (graph *) malloc(sizeof(graph));
	gg->nodemalloc = 4;
	gg->nodereal = 0;
	gg->nodes = (GraphNode **) malloc((gg->nodemalloc)*sizeof(GraphNode *));
	return gg;
}

/*This function will generate new graphnode
 * and add it to a graph and return the node back
 */
GraphNode *addnode(graph *gg, char *name) {
    if ((gg->nodereal)>=(gg->nodemalloc)) {
        gg->nodemalloc = 2*(gg->nodemalloc);
        gg->nodes = realloc(gg->nodes, (gg->nodemalloc)*sizeof(GraphNode *));
    }
    GraphNode *node = createnode(name);
    gg->nodes[gg->nodereal] = node;
    gg->nodereal++;
    return node;
}

/*this function is to check whether 
 * there is already some node inside the graph having the given name
 */
GraphNode *inornot(graph *gg, char *name) {
	for (int i = 0; i < gg->nodereal; i++) {
		if (strcmp(((gg->nodes)[i])->name,name) == 0) {
			return gg->nodes[i];
		}
	}
	return addnode(gg, name);
}
/*This function is used to get node 
 *for a certain name
 */
GraphNode *getNode(graph *gg, char *name){
	for (int i = 0; i < gg->nodereal; i++) {
		if (strcmp(((gg->nodes)[i])->name,name) == 0) {
			return gg->nodes[i];
		}
	}
	return NULL;
}

/*this function is used to add node into a graph
 */
void addrealnode(graph *gg, GraphNode *gn) {
    if ((gg->nodereal)>=(gg->nodemalloc)) {
        gg->nodemalloc = 2*(gg->nodemalloc);
        gg->nodes = realloc(gg->nodes, (gg->nodemalloc)*sizeof(GraphNode *));
    }
    gg->nodes[gg->nodereal] = gn;
    gg->nodereal++;
    return;
}
/*this fucntion is to check if the node is already in the graph,
 * if not, add into the graph, otherwise do nothing
 */
void inornotreal(graph *gg, GraphNode *gn) {
    for (int i = 0; i < gg->nodereal; i++) {
        if (strcmp(((gg->nodes)[i])->name,gn->name) == 0) {
            return;
        }
    }
    addrealnode(gg, gn);
    return;
}

