# sample Makefile.
# It compiles every .c file in the src/ directory to object files in
# the obj/ directory, and build the ./my_executable my_executable.

# using icc :
#COMPILER ?= $(ICC_PATH)icpc
# using gcc :
#COMPILER ?= $(GCC_PATH)gcc
# using mpirun :
COMPILER ?= mpicc

FLAGS ?= -O2 -Wall -Wno-variadic-macros -pedantic $(GCC_SUPPFLAGS)

LDFLAGS ?= -g 
LDLIBS =

EXECUTABLE = my_executable

SRCS=$(wildcard src/*.c)
OBJS=$(SRCS:src/%.c=obj/%.o)

all: release

release: $(OBJS)
	$(COMPILER) $(LDFLAGS) -o $(EXECUTABLE) $(OBJS) $(LDLIBS) 

obj/%.o: src/%.c
	$(COMPILER) $(FLAGS) -o $@ -c $<


clean:
	rm -f obj/*
	rm ${EXECUTABLE} 

cleandata:
	rm cachegrind.out.*
