#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "build_spec_repr.h"
#include "build_spec_graph.h"
#include "topologicalsort.h"
#ifndef proc_creation_proc_exe_h
#define proc_creation_proc_exe_h
int isExist(char* filename);
int isNew(char* parent, char* children);
void execution(char* cmdline);
void  CompileorNot(GraphNode *gn);
void ExecuteEveryNode(graph *gg);
#endif
