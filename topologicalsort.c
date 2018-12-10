#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "build_spec_graph.h"
#include "build_spec_repr.h"
#include "topologicalsort.h"


/*This c file mainly resolves two questions:
 * 1. Extract the family of a startnode. 
 * 2. Detect whether the graph is a DAG or not, 
 * and get post order traver's result of nodes for a DAG graph
 * By solving these two questions, we mainly use BFS and topological sort. 
 * To achive BFS, we create nodequeue structure and add enqueue and dequeue methods
 */


/*This function to create a queue of graphnodes with original size of 32.
*/
nodequeue *createqueue() {
    nodequeue *queue = malloc(sizeof(nodequeue));
    queue->capacity = 32;
    queue->size = 0;
    queue->first = 0;
    queue->last = -1;
    queue->elements = (GraphNode **)malloc(32*sizeof(GraphNode*));
    return queue;
}

/*This function is to add item into the queue.
 * It places the pointer to the graphnode at the end of queue q. 
 * If the queue is full, double the capacity of queue 
 * until there is space available.
 */
void enqueue(nodequeue *q, GraphNode *string) {
    if(q->size >= q->capacity) {
        q->capacity = 2*(q->capacity);
        q->elements = realloc(q->elements, (q->capacity)*sizeof(GraphNode *));
    }
    q->size ++;
    q->last ++;
    if (q->last == q->capacity) {
            q->last = 0;
    }
    q->elements[q->last] = string;
    return;
}

/*This function is to remove the item corresponding to the first pointer 
 * from the queue.
 * If the queue is empty, stderr error type and exit
 */
GraphNode *dequeue(nodequeue *q) {
    if(q->size == 0) {
        fprintf(stderr,"Error! Cannot dequeue with empty queue!");
        exit(1);
    }        
    GraphNode *ss = q->elements[q->first];
    q->size --;
    q->first ++;
    if (q->first == q->capacity) {
            q->first = 0;
    }
    return ss;
}


/*This function is to get the family of the startnode. 
 * By using this function to the original graph, we only leave the children,
 * grandchildren,grandgrand......grandchildren of the startnode.
 * If the startnode is assigned as "", it means the first node in the graph
 */
graph *subgraph(graph *allgraph, char *startname) {
    //create a newgg as a return graph
    graph *newgg = creategraph();
    //create a nodequeue for later BFS
    nodequeue *que = createqueue();
    //set the startnode as the firstnode
    GraphNode *startnode = allgraph->nodes[0];
    //if there are an arguement from the commandline
    if (strcmp(startname, "!") != 0) { 
      //set the startnode to the commandline's file
      startnode = getNode(allgraph, startname);
      // if the specify name doesnt exit, error
      if (startnode == NULL){
        fprintf(stderr, "Error: %s is not found\n", startname);
	    exit(1);
      }
    }
    //visitedsubgraph is a integer helping to judge 
    //whether the node has been visited or not
    //during our subgraph's BFS
    startnode->visitedsubgraph = 1;
    //indegree calculate how many father intotal this node has
    //however, we only consider startnode's family now.
    //if a child has other father beyond the start node's family
    //this father should be ignore
    //so we recount father number
    startnode->indegree = 0;
    // printeverything(startnode);
    //put startnode into the output graph
    inornotreal(newgg,startnode);
    //put startnode into BFS queue
    enqueue(que, startnode);
    //while the queue is not empty, keep searching
    while (que->size > 0) {
        //pick the first element in the queue out
        GraphNode *node = dequeue(que);
        //search all its children
        for (int k = 0; k< node->outdegree; k++) {
            //if this children has not been visited yet, visit it
            //add it into the output graph and add it into the BFS queue
            if (node->children[k]->visitedsubgraph == 0) {
                node->children[k]->indegree = 1;
                node->children[k]->visitedsubgraph = 1;
                inornotreal(newgg,node->children[k]);
                enqueue(que, node->children[k]);
            }
            //if we have already visited this children
            //give it one more father
            else {
                node->children[k]->indegree++;
            }
        }
    }
    //return the startnode's family nodes
    return newgg;
}


/*This function is to get the nodes' names that are in gg but not in newgg. 
By implementing this function, we can drag the nodes that are in a circle 
and pick the nodes that are not in the circle out. 
*/
void getnamee(graph *gg, graph *newgg) {
    //traverse all nodes in larger graph
    for (int i = 0; i < gg->nodereal; i++) {
        int temp = 0;
        for (int j = 0; j < newgg->nodereal; j++) {
            //traverse all nodes in newgg
            if (strcmp(newgg->nodes[j]->name, gg->nodes[i]->name) == 0) {
                temp = 1;
                break;
            }
        }
        //if the node in gg is not in newgg
        if (temp == 0) {
            //this is the node that is in circle
            fprintf(stderr, "%s ", gg->nodes[i]->name);
        }
    }
    return;
}

/*This function is to detect whether the graph is a DAG or not. 
If it is, the output is the nodes in post order traverse order.
If it is not, it returns a graph without node.
*/
graph *topologicalsort(graph *gg) {
     //use bfs to topological sorting
    
    //create a graph to store return nodes
    graph *newgg = creategraph();
    //create a queue for BFS
    nodequeue *que = createqueue();
    //loop for all nodes in the graph
    for (int i = 0; i < gg->nodereal; i++) {
        //if this node has no father at all or it 
	//has been visited by all father, and it has grown up
        if (gg->nodes[i]->indegree == 0) {
            //indegree here is an indicator. If this has been no 
	    //father left, set it to -1 when indegree = -1, it also 
	    //indicate that the node has been add to output graph
            gg->nodes[i]->indegree -= 1;
            //add this no more father node into graph
            inornotreal(newgg,gg->nodes[i]);
            //add this no more father node for bfs
            enqueue(que, gg->nodes[i]);
            //while queue for bfs is not empty
            while (que->size > 0) {
                //get the first node out of queue to check children
                GraphNode *node = dequeue(que);
    		    // printeverything(node);
                for (int k = 0; k< node->outdegree; k++) {
                    //for each child been visit, 
		    //get independent from this father
                    node->children[k]->indegree -= 1;
                    //all father of the child has been visited, 
                    //then the child has no more father waiting, 
		    //then put it into output and put it into bfs queue
                    if (node->children[k]->indegree == 0) {
                        node->children[k]->indegree -= 1;
                        inornotreal(newgg,node->children[k]);
                        enqueue(que, node->children[k]);
                    }
                }
            }
        }
    }

    //if no circle, all nodes are added into output file
    if (newgg->nodereal == gg->nodereal) {
        return newgg;
    }

    //if there is a circle, exit(1)
    else {
        fprintf(stderr, "There is a circle inside following files: ");
        getnamee(gg, newgg);
        fprintf(stderr, "\n");
        exit(1);
    }
}



