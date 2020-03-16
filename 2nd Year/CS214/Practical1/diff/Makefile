#
# Makefile for practical tests
#

# compiler flags
DEBUGGER   = -ggdb
OPTIMISE   = -O0
WARNINGS   = -Wall -Wextra -pedantic -ansi
CFLAGS     = $(DEBUGGER) $(OPTIMISE) $(WARNINGS)

# commands
CC         = cc
RM         = rm -rf
COMPILE    = $(CC) $(CFLAGS)

# targets
TARGET     = showdiff
LIBRARIES  = lcs
OBJECTS   := $(foreach OBJ, $(LIBRARIES), $(addsuffix .o, $(OBJ)))
TESTDIR    = tests
TESTS     := $(sort $(addsuffix .test, $(notdir \
				$(basename $(basename $(wildcard $(TESTDIR)/*.out.txt))))))

### RULES ######################################################################

.PHONY: all clean test %.test

all: $(TARGET)

test: $(TARGET) $(TESTS)

$(TARGET): %: %.c $(OBJECTS)
	$(COMPILE) -o $@ $@.c $(OBJECTS)

$(LIBRARIES): %: %.c
	$(COMPILE) -c $<

%.test: $(TESTDIR)/%.cmd.txt $(TESTDIR)/%.out.txt
	@echo -n Test $(basename $@):\ 
	@timeout 60s bash -c "./$(TARGET) $(shell cat $<) 2>&1 | \
			cmp -s $(word 2, $?) - \
				&& echo -e '\033[0;32mpassed\033[m' \
				|| echo -e '\033[0;31mfailed\033[m'" || \
		echo "\033[0;33mtimed out\033[0m"

clean:
	$(RM) $(foreach EXE, $(TARGET), $(EXE))
	$(RM) $(foreach OBJ, $(OBJECTS), $(OBJ))
