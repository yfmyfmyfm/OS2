#include "build_spec_graph.h"
#include "build_spec_repr.h"
#ifndef text_parse_h
#define text_parse_h

char* tar_parsing(graph* mGraph, char* buffer, int size, int lineNum);
void cmd_parsing(char* buffer, int size, GraphNode* tarNode, int lineNum);

#endif
