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
VALGRIND   = valgrind -q --error-exitcode=13 --leak-check=full

# targets
TARGET     = nsort
TESTDIR    = tests
TESTS     := $(sort $(addsuffix .test, $(notdir \
				$(basename $(basename $(wildcard $(TESTDIR)/*.out.txt))))))

### RULES ######################################################################

.PHONY: all clean test %.test

all: $(TARGET)

test: $(TARGET) $(TESTS)

$(TARGET): %: %.c
	$(COMPILE) -o $@ $@.c

%.test: $(TESTDIR)/%.cmd.txt $(TESTDIR)/%.in.txt $(TESTDIR)/%.out.txt
	@echo -n Test $(basename $@):\ 
	@./$(TARGET) $(shell cat $<) < $(word 2, $?) 2>&1 | \
		cmp -s $(word 3, $?) - && \
		echo -n "\033[0;32mpassed\033[0m; " || \
		echo -n "\033[0;31mfailed\033[0m; "
	@echo -n 'valgrind: '
	@$(VALGRIND) ./$(TARGET) $(shell cat $<) < $(word 2, $?) > /dev/null 2>&1; \
		test $$? -eq 13 && \
		echo "\033[0;31mfailed\033[0m" || \
		echo "\033[0;32mpassed\033[0m"

clean:
	$(RM) $(foreach EXE, $(TARGET), $(EXE))
