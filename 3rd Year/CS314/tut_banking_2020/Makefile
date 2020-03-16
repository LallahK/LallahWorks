# sample Makefile.
# It compiles every .c files in the src/ directory to object files in obj/ directory, and build the executable

#COMPILER = /opt/local/bin/clang-mp-9.0 
# using gcc:
COMPILER ?= $(GCC_PATH)gcc

FLAGS ?= -O2 -Wall -Wno-variadic-macros -pedantic -g -fopenmp $(GCC_SUPPFLAGS)
#-DDEBUG

LDFLAGS ?= -g -fopenmp
LDLIBS = -lm

EXECUTABLE = bank_simulation 

SRCS=$(wildcard src/*.c)
OBJS=$(SRCS:src/%.c=obj/%.o)

all: release

release: $(OBJS)
	$(COMPILER) $(LDFLAGS) -o $(EXECUTABLE) $(OBJS) $(LDLIBS) 

obj/%.o: src/%.c
	$(COMPILER) $(FLAGS) -o $@ -c $<

$(shell mkdir -p obj/)

clean:
	rm -f obj/*
	rm ${EXECUTABLE} 
