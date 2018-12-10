CC=gcc
WARNING_FLAGS=-Wall -Wextra
EXE=537make

# the -g flag at all gcc compilation stages ensures that you can use gdb to debug your code
$(EXE): main.o build_spec_repr.o build_spec_graph.o topologicalsort.o proc_creation_proc_exe.o text_parsing.o 
	$(CC) -g -o $(EXE) main.o build_spec_repr.o build_spec_graph.o topologicalsort.o proc_creation_proc_exe.o text_parsing.o 

main.o: main.c build_spec_repr.h build_spec_graph.h topologicalsort.h proc_creation_proc_exe.h text_parsing.h 
	$(CC) -g $(WARNING_FLAGS) -c main.c

proc_creation_proc_exe.o: proc_creation_proc_exe.c proc_creation_proc_exe.h build_spec_graph.h build_spec_graph.h 
	$(CC) -g $(WARNING_FLAGS) -c proc_creation_proc_exe.c

topologicalsort.o: topologicalsort.c topologicalsort.h build_spec_graph.h build_spec_graph.h 
	$(CC) -g $(WARNING_FLAGS) -c topologicalsort.c

text_parsing.o: text_parsing.c text_parsing.h build_spec_graph.h build_spec_graph.h 
	$(CC) -g $(WARNING_FLAGS) -c text_parsing.c

build_spec_graph.o: build_spec_graph.c build_spec_graph.h build_spec_repr.h
	$(CC) -g $(WARNING_FLAGS) -c build_spec_graph.c

build_spec_repr.o: build_spec_repr.c build_spec_repr.h
	$(CC) -g $(WARNING_FLAGS) -c build_spec_repr.c


# the -f flag for rm ensures that clean doesn't fail if file to be deleted doesn't exist
clean:
	rm -f $(EXE) *.o

# recompile runs clean and then makes everything again to generate executable
# this is equivalent to running "make clean" followed by "make"
recompile: clean $(EXE)


scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html
