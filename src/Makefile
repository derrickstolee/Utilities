#
# Base Makefile for SymmetricBranching project.
#

			
TARGETS =   \
			adjmattos6.exe				\
			canonoutput.exe				\
			seconds.exe					\
			testtranslation.exe			\
			testcolextranslation.exe	\
			testlextranslation.exe \
			test_fraction.exe
			


OBJECTS = 	fraction.o					\
			Set.o							\
			translation.o					\
			TreeSet.o				\
			


CC 	= gcc
CXX = g++

OPT = -O4
DEBUG = 
# -g 
# -static 
WARNINGS = -Wall


CFLAGS 		= $(OPT) $(DEBUG) $(WARNINGS)
CXXFLAGS 	= $(OPT) $(DEBUG) $(WARNINGS)
LFLAGS 		= $(DEBUG) $(WARNINGS)


TREESEARCHDIR = ../../TreeSearch
NAUTYDIR = ../../nauty

INCLUDES =  -I$(TREESEARCHDIR) \
			-I$(NAUTYDIR)


TREESEARCHOBJS	= $(TREESEARCHDIR)/SearchManager.o 
				  
NAUTYOBJS     	= $(NAUTYDIR)/nauty.o 			\
				  $(NAUTYDIR)/nausparse.o		\
				  $(NAUTYDIR)/gtools.o			\
				  $(NAUTYDIR)/nautil.o			\
				  $(NAUTYDIR)/nautinv.o			\
				  $(NAUTYDIR)/naugraph.o		\
				  $(NAUTYDIR)/schreier.o		\
				  $(NAUTYDIR)/naurng.o	
					
LIBOBJS			= $(TREESEARCHOBJS) $(NAUTYOBJS)
			
# These would be used if we didn't flatten
NESTINCL =  -I$(src_dir)/src \
			-I$(src_dir)/src/dsrg \
			-I$(src_dir)/src/mains \
			-I$(src_dir)/src/model \
			-I$(src_dir)/src/model/algorithms \
			-I$(src_dir)/src/model/branching \
			-I$(src_dir)/src/model/constraints \
			-I$(src_dir)/src/model/data \
			-I$(src_dir)/src/permutations \
			-I$(src_dir)/src/test \
			-I$(src_dir)/src/utils
			
.SUFFIXES: .c .cpp .o .obj .exe 

all: $(OBJECTS) $(TESTS) $(TARGETS)


# The default object compiler
.c.o: $<
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -c $< -o $@
        
.cpp.o: $<
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -c $< -o $@
        
.cpp.exe: $< $(OBJECTS)  
	$(CXX) $(CXXFLAGS) $(LFLAGS)			    \
        	$(INCLUDES)				\
        	$(LIBOBJS) $(LIBS)		\
        	$(OBJECTS)       \
            $< -o $@
	cp $@ ../bin/
        
.c.exe: $< $(COBJECTS)
	$(CC) $(CXXFLAGS) $(LFLAGS)			    \
        	$(INCLUDES)				\
        	$(NAUTYOBJS)  $(COBJECTS) $(LIBS)		\
            $< -o $@
	cp $@ ../bin/
        
clean:
	rm -f $(OBJECTS) $(TARGETS) $(TESTS)
	
cleanexe:
	rm -f $(TARGETS)
