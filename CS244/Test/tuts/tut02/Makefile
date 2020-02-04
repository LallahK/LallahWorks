
CC = gcc
CFLAGS = -I.
DEPS = length.h copy.h
OBJ = length.o dup.o copy.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

duplicate: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS)

quicksort: q_sort.o
		$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	-rm *.o $(objects) q_sort.o
