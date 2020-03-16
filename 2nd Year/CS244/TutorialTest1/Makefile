#
# Makefile for tutorial tests
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
TARGET     = max
TESTDIR    = tests
TESTS     := $(sort $(addsuffix .test, \
				$(notdir $(basename $(wildcard $(TESTDIR)/*.in)))))

### RULES ######################################################################

.PHONY: all clean test %.test

all: $(TARGET)

test: $(TARGET) $(TESTS)

$(TARGET): %: %.c
	$(COMPILE) -o $@ $@.c

%.test: $(TESTDIR)/%.in $(TESTDIR)/%.out
	@echo -n Test $(basename $@):\ 
	@./$(TARGET) $(shell cat $<) | cmp -s $(word 2, $?) - && \
		echo "\033[0;32mPassed\033[0m" || \
		echo "\033[0;31mFailed\033[0m" 

clean:
	$(RM) $(foreach EXE, $(TARGET), $(EXE))
