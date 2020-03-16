#
# Makefile for CS244 Tutorial 6 (2019)
#

# compiler flags
DEBUGGER   = -ggdb
OPTIMISE   = -O0
WARNINGS   = -Wall -Wextra -pedantic -ansi
ARCH       = -m32
AFLAGS     = -g -f elf32
CFLAGS     = $(DEBUGGER) $(OPTIMISE) $(WARNINGS) $(ARCH)
DFLAGS     = # up to you...

# commands
ASM        = nasm
CC         = clang
RM         = rm -rf
MKDIR      = mkdir -p
COMPILE    = $(CC) $(CFLAGS) $(DFLAGS)
ASSEMBLE   = $(ASM) $(AFLAGS)
INSTALL    = install

# directories
BINDIR     = ../bin
LOCALBIN   = ~/.local/bin

# targets
TARGETS    = hcompress
LIBRARIES  = heap huffman
OBJECTS    = $(foreach OBJ, $(LIBRARIES), $(addsuffix .o, $(OBJ)))

### RULES ######################################################################

.PHONY: all clean install uninstall

all: $(TARGETS)

$(TARGETS): %: %.c $(LIBRARIES) | $(BINDIR)
	$(COMPILE) -o $(BINDIR)/$@ $< $(OBJECTS)

$(LIBRARIES): %: %.asm %.h
	$(ASSEMBLE) $<

$(BINDIR):
	$(MKDIR) $(BINDIR)

clean:
	$(RM) $(foreach EXE, $(TARGETS), $(wildcard $(BINDIR)/$(EXE)))
	$(RM) $(foreach OBJ, $(OBJECTS), $(OBJ))

install:
	$(MKDIR) $(LOCALBIN)
	$(INSTALL) $(foreach EXE, $(TARGETS), $(wildcard $(BINDIR)/$(EXE))) \
		$(LOCALBIN)

uninstall:
	$(RM) $(foreach EXE, $(TARGETS), $(wildcard $(LOCALBIN)/$(EXE)))
