#
# Makefile for queue
#

# assembler and compiler flags
DEBUG        = -ggdb
OPTIMISE     = -O0
TARGET       = -m32
WARNINGS     = -Wall -Wextra -pedantic -ansi
CFLAGS       = $(DEBUG) $(OPTIMISE) $(WARNINGS) $(TARGET)
ASMFLAGS     = -f elf32

# commands
ASM          = nasm
CC           = gcc
RM           = rm -f
COMPILE      = $(CC) $(CFLAGS) $(DFLAGS)
ASSEMBLE     = $(ASM) $(ASMFLAGS)

# rule types
executables  = test_driver
asm_sources := $(wildcard *.asm)
asm_objects := $(asm_sources:%.asm=%.ao)
c_sources   := $(wildcard *.c)
c_objects   := $(c_sources:%.c=%.o)
tests       := $(sort $(addsuffix .test, $(basename $(wildcard test[0-9].txt))))

# reporting
fail          = "\033[0;31mfail\033[0m"
pass          = "\033[0;32mpass\033[0m"

# rules ########################################################################

test_driver: $(asm_objects) $(c_objects)
	$(COMPILE) -o $@ $^

%.ao: %.asm
	$(ASSEMBLE) -o $@ $<

%.o: %.c
	$(COMPILE) -o $@ -c $<

# phony targest ################################################################

.PHONY: all clean types.vim

all: clean $(executables)

clean:
	$(RM) $(asm_objects)
	$(RM) $(c_objects)
	$(RM) $(executables)

test: $(tests)

%.test: %.txt
	@echo -n "$^: "
	@./test_driver < $^ && echo $(pass) || echo $(fail)

types.vim: *.[ch]
	ctags --c-kinds=t -o- *.[ch] |\
		awk 'BEGIN{printf("syntax keyword Type\t")}\
		{printf("%s ", $$1)}END{print ""}' > $@
	ctags --c-kinds=gsu -o- *.[ch] |\
		awk 'BEGIN{printf("syntax keyword StorageClass\t")}\
		{printf("%s ", $$1)}END{print ""}' >> $@
	ctags --c-kinds=d -o- *.[ch] |\
		awk 'BEGIN{printf("syntax keyword Constant\t")}\
		{printf("%s ", $$1)}END{print ""}' >> $@

