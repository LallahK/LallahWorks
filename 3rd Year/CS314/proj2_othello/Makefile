PRG_SUFFIX_FLAG := 0
#
LDFLAGS := 
CFLAGS_INC := -g
CFLAGS := -O2 -Wall -Wno-variadic-macros -pedantic $(CFLAGS_INC) -DDEBUG
#
## ==================- NOTHING TO CHANGE BELOW THIS LINE ===================
##
SRCS := $(wildcard src/*.c)
PRGS := $(patsubst %.c,%,$(SRCS))
PRG_SUFFIX=.exe
CC := mpicc
BINS := $(patsubst %,%$(PRG_SUFFIX),$(PRGS))
## OBJS are automagically compiled by make.
OBJS := $(patsubst %,%.o,$(PRGS))
##
all : $(BINS)
##
## For clarity sake we make use of:
.SECONDEXPANSION:
OBJ = $(patsubst %$(PRG_SUFFIX),%.o,$@)
ifeq ($(PRG_SUFFIX_FLAG),0)
	BIN = $(patsubst src/%$(PRG_SUFFIX),player/%,$@)
else
	BIN = $@
endif
## Compile the executables
%$(PRG_SUFFIX) : $(OBJS)
	$(CC) $(OBJ) $(LDFLAGS) -o $(BIN)
##
## $(OBJS) should be automagically removed right after linking.
##
veryclean:
ifeq ($(PRG_SUFFIX_FLAG),0)
	$(RM) $(PRGS)
else
	$(RM) $(BINS)
endif
##
rebuild: veryclean all
##
## eof Generic_Multi_Main_PWD.makefile
