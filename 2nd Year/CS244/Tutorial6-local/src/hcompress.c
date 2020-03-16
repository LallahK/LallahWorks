#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "heap.h"
#include "huffman.h"

#define BUF_SIZE 1024

void charfreq(int fd, int freq[]);
void showfreq(int table[]);
void huffman_print_tree(HeapNode *t, int i);
void huffman_print_branch(HeapNode *t);
void huffman_print_heap(Heap *h);
void huffman_print_table(HuffmanNode *n);

int main(int argc, char *argv[])
{
	char *in_name, *out_name;
	int bi, ci, j;
	int ftable[MAX_HEAP_SIZE];
	int in_fd, out_fd;
	int nchars, tchars;
	size_t nbytes;
	unsigned char ch, b;
	unsigned char inbuf[BUF_SIZE], outbuf[BUF_SIZE];
	unsigned int i;
	Heap h;
	HeapNode hnode, *htree;
	HuffmanNode htable[MAX_HEAP_SIZE];

	if (argc != 2) {
		fprintf(stderr, "usage: compress <filename>\n");
		exit(EXIT_FAILURE);
	}
	in_name = argv[1];
	out_name = (char *) malloc((strlen(in_name)+4)*sizeof(char));
	strcpy(out_name, in_name);
	strcat(out_name, ".hz");

	if ((in_fd = open(in_name, O_RDONLY)) < 0) {
		fprintf(stderr, "File \"%s\" could not be opened.\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	charfreq(in_fd, ftable);
	close(in_fd);

	/* heap construction */
	heap_initialize(&h);
	for (i = 0; i < MAX_HEAP_SIZE; i++) {
		if (ftable[i] > 0) {
			hnode.c = (unsigned char) i;
			hnode.frequency = ftable[i];
			hnode.left = hnode.right = NULL;
			heap_insert(&h, &hnode);	
		}
	}	
	huffman_print_heap(&h);

	nchars = h.n;
	
	/* huffman table construction */
	huffman_build_tree(&h, &htree);
	huffman_initialize_table(htable);
	huffman_build_table(htree, htable, 0, 0);

	/* open files for processing */
	if ((in_fd = open(in_name, O_RDONLY)) < 0) {
		fprintf(stderr, "File \"%s\" could not be opened.\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	if ((out_fd = open(out_name, O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0) {
		fprintf(stderr, "File \"%s\" could not be opened.\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	/* write header */
	write(out_fd, &nchars, sizeof(int));
	for (i = 0; i < MAX_HEAP_SIZE; i++) {
		if (ftable[i] > 0) {
			ch = (unsigned char) i;
			write(out_fd, &ch, 1);
			write(out_fd, &ftable[i], sizeof(int));
		}
	}

	/* encode */
	b = 0;
	bi = 7;
	j = 0;
	tchars = 0;
	while ((nbytes = read(in_fd, inbuf, BUF_SIZE)) > 0) {
		for (i = 0; i < nbytes; i++) {
			ci = htable[(int)inbuf[i]].length - 1;
			tchars++;
			while (ci >= 0) {
				if (htable[(int)inbuf[i]].code & (1 << ci)) {
					b |= 1 << bi;
				}
				ci--;
				bi--;
				if (bi == -1) {
					outbuf[j++] = b;
					b = 0;
					bi = 7;
					if (j == BUF_SIZE) {
						write(out_fd, &outbuf, BUF_SIZE);
						j = 0;
					}
				}
			}
		}
	}
	
	if (bi >= 0) {
		outbuf[j++] = b;
	}
	if (j > 0) {
		write(out_fd, &outbuf, j);
	}

	/* close files */

	close(in_fd);
	close(out_fd);
	free(out_name);

	return EXIT_SUCCESS;
}

void charfreq(int fd, int freq[])
{
	unsigned int i;
	unsigned char buf[BUF_SIZE];
	size_t nbytes;

	memset(freq, 0, MAX_HEAP_SIZE*sizeof(int));
	memset(buf, 0, BUF_SIZE);
	while ((nbytes = read(fd, buf, BUF_SIZE)) > 0) {
		for (i = 0; i < nbytes; i++) {
			freq[buf[i]]++;
		}
	}
}

void showfreq(int table[])
{
	int i;

	for (i = 0; i < MAX_HEAP_SIZE; i++) {
		printf("%3d: %8d\n", i, table[i]);
	}
}

void huffman_print_tree(HeapNode *t, int i) {
	printf("(%c,%d -- %d)\n", t->c, t->frequency, i);
	if (t->left != NULL) {
		huffman_print_tree(t->left, i + 1);
	}
	if (t->right != NULL) {
		huffman_print_tree(t->right, i + 1);
	}
}

void huffman_print_branch(HeapNode *t) {
	printf("\n(%c,%d) : ", t->c, t->frequency);
	printf("r - (%c,%d) : ", t->right->c, t->right->frequency);
	printf("l - (%c,%d)", t->left->c, t->left->frequency);
	printf("--------\n");
}

void huffman_print_heap(Heap *h) {
	int i;
	for (i = 0; i < h->n; i++) {
		printf("(%c,%d) ", h->a[i].c, h->a[i].frequency);
	}
	printf("\n");
}

void huffman_print_table(HuffmanNode *n) {
	int i;
	for (i = 0; i < 256; i++) {
		if (n[i].code != -1) {
			printf("char '%c' = ASCII %d: (%d, %d)\n", i, i,  n[i].code, n[i].length);
		}
	}
}

/*void binToStr(int i) {
	long tempDecimal;  
	char binary[65];  
	int index = 0; 
	tempDecimal = i;  

	while(tempDecimal!=0)  
	{
		binary[index] = (tempDecimal % 2) + '0';
		tempDecimal /= 2;
		index++;
	}
	binary[index] = '\0';
	strrev(binary);  
}*/
