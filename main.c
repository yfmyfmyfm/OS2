////////////////////////////////////////////////////////////////////////////////
// Main File:        main.c
// This File:        main.c
// Other Files:      build_spec_graph.c/h build_spec_repr.c/h text_parsing.c/h 
//                   topological.c/h proc_creation_proc.exe.c/h
// Semester:         CS 537 Fall 2018
//
// Author:           Siyuan Ji
// Email:            sji27@wisc.edu
// CS Login:         siyuan
//
// Author:           Yifan Mei
// Email:            ymei8@wisc.edu
// CS Login:         yifanmei
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          NULL
//
// Online sources:   NULL
//
//////////////////////////// 80 columns wide ///////////////////////////////////

#include<stdio.h>
#include<stdlib.h>
#include "text_parsing.h"
#include "build_spec_graph.h"
#include "topologicalsort.h"
#include "build_spec_repr.h"
#include "proc_creation_proc_exe.h"

int main(int argc, char**argv){
  FILE* fp;
  //record line number
  int lineNum = 1;
  //maximum length for input line
  int size = 1024;
  char c;
  char* buffer = malloc(size*sizeof(char));
  //mark as the first line execution of program
  int init = 1;
  int count = 0;
  //record if the line is a target line or cmd line
  int tar_sig = 0;
  int cmd_sig = 0;
  char* tarName = malloc(size*sizeof(char));
  //record how many colons appear
  int colonNum = 0;
  //creat an instance of graph
  graph* mGraph = creategraph(); 
  //store the sepcification file to build if exist
  char* specFile = malloc(size*sizeof(char));

  //error if more than two files ask for specification build
  if (argc > 4){
    fprintf(stderr, "Error: too many arguments for "
		    "537make\n");
    exit(1); 
  }
  //process the input like 537make -f xxx read.o
  else if (argc == 4){
    if (!strcmp(argv[1], "-f")){
      if ((fp = fopen(argv[2], "r")) == NULL){
        fprintf(stderr, "Error: Cannot open target makefile\n");
        exit(1);
      }
      strcpy(specFile,argv[3]);
    }
    else {
      fprintf(stderr, "Error: Cannot recognize the argument\n");
      exit(1);
    }
  }
  //process the input like 537make -f xxx
  else if (argc == 3){
    if (!strcmp(argv[1], "-f")){
      if ((fp = fopen(argv[2], "r")) == NULL){
        fprintf(stderr, "Error: Cannot open target makefile\n");
        exit(1);
      }
    }
    else {
      fprintf(stderr, "Error: Cannot recognize the argument\n");
      exit(1);
    }
  }
  //cases for one or two  arguments are read
  else{ 
    //look for makefile, if it does not exist, look for Makefile
    if ((fp = fopen("makefile", "r")) == NULL){
      fp = fopen("Makefile", "r");
      if (fp == NULL){
        printf("Error! Cannot open target file\n");
        exit(1);
      }
    }
    // store the name of the file need build specification
    if (argc == 2){
    strcpy(specFile, argv[1]);
    }
  }
  //read user inputs
  while((c = fgetc(fp)) != EOF){
    if (c == '\0'){
      fprintf(stderr, "%d: Invalid line: Input cannot " 
		      "contain null character\n", lineNum);
      exit(1);
    }
 
    //check if the input line is a target line or cmd line
    if(count == 0){ 
      //error if the line start with a space	    
      if(c == ' '){
        fprintf(stderr, "%d: Invalid line: Input line cannot " 
			"start with a space.\n", lineNum);
        exit(1);
      }
      //ignore blank lines
      else if (c == '\n'){
	lineNum++;      
        continue;
      }
      //ignore comment line
      else if (c == '#'){
        while((c != '\n') && (c != '\0')){
	  c = fgetc(fp);
	}
	// exit if a null character is found 
	if (c == '\0'){
	  fprintf(stderr, "%d: Invalid line: Input cannot " 
		      "contain null character\n", lineNum);
	  exit(1);
	}
        lineNum++;
      }
      //it may be a cmd line if start with a tap
      else if(c == '\t'){
        //first line cannot start with cmd line
        if (init){
	  fprintf(stderr, "%d: Invalid line: Cannot start " 
			  "with command line.\n", lineNum);
	  exit(1);
	}
	cmd_sig = 1;
      }
      //it may be a target line if start with letters
      else{
	//set to zero once the file start with target line      
	init = 0;      
        tar_sig = 1;
      }	    
    }
   //check if user input string is longer than buffersize
    if(c != '\n' && count == (size-1)){
      fprintf(stderr, "%d: Invalid line: Input line is "
		      "too long.\n", lineNum);
      exit(1);
    }
    
    //one line of input has been readed
    else if (c == '\n'){
      buffer[count] = '\0';
      //call text_parsing to get info from either cmd or target line
      if (tar_sig){
	if (colonNum > 1){
          fprintf(stderr, "%d: Invalid line: There are more than "
			  "one colon appear.\n", lineNum);
          exit(1);
	}

	//store name of Target file for adding cmd
        tarName = malloc(sizeof(char)*size);
        tarName = tar_parsing(mGraph, buffer, count, lineNum);
	tar_sig = 0;
      }
      
      if (cmd_sig){
        GraphNode* tarNode = getNode(mGraph, tarName);
        cmd_parsing(buffer, count, tarNode, lineNum);
	cmd_sig = 0;
      }
      //reset colonNum for each line
      colonNum = 0;
      //record line number
      lineNum++;
      //reset counter to read new line
      count = 0;
      // alloc new memeory space for another input
      buffer = malloc(sizeof(char) * size);
    }
    else{
      //record how many colons appear in the target line
      if ((tar_sig == 1) && (c == ':')){
        colonNum++;
      }
      buffer[count] = c;
      count++;
    }
  }
    //go over all graph to check if loop exist. 
    graph *toposort = topologicalsort(mGraph);
     
    //if there is a build specification, then first get the
    //subgraph and traverse every node
	
    char *substrings = "!";
    if ((argc == 2) || (argc == 4)){ 
      substrings = specFile;
    }
    graph *subg = subgraph(mGraph, substrings);
    toposort = topologicalsort(subg);

    ExecuteEveryNode(toposort);
    
    // free all space on the heap
    free(buffer);
    free(tarName); 
    free(toposort);
    free(mGraph);
    free(specFile);
    free(subg);
    fclose(fp);
    return 0;
}
