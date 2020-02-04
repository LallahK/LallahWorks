#
# COMPUTER SCIENCE 244
# Makefile for Tutorial 3
# (c) 2018 University of Stellenbosch
#

NAME     = tut3
ASM_SRC  = tut3.asm
C_OBJS   = test3.o
ASM_OBJS = tut3.o
OBJS     = $(ASM_OBJS) $(C_OBJS)

# flags
DEBUG    = -ggdb
OPTIMISE = -O0
TARGET   = -m32
WARNINGS = -Wall -Wextra -Wno-variadic-macros -pedantic
CFLAGS   = $(DEBUG) $(OPTIMISE) $(WARNINGS) $(TARGET)
DFLAGS   = # nothing yet....
ASMFLAGS = -f elf32

# commands
ASM      = nasm
CC       = gcc
RM       = rm -f
COMPILE  = $(CC) $(CFLAGS) $(DFLAGS)
ASSEMBLE = $(ASM) $(ASMFLAGS)

# rules ########################################################################

all: clean $(NAME)

$(NAME): $(OBJS)
	$(COMPILE) -o $@ $(OBJS)

$(ASM_OBJS): $(ASM_SRC)
	$(ASSEMBLE) $(ASM_SRC)

clean:
	$(RM) *.o *.map *.s *.bin
	$(RM) $(NAME)

