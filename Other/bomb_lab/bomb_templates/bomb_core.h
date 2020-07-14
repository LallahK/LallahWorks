#ifndef BOMB_CORE_H
#define BOMB_CORE_H

#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <curl/curl.h>

#define URL_BUF_SIZE  100
#define DATA_BUF_SIZE 1000
#define ERR_BUF_SIZE  1000
#define MSG_BUF_SIZE  1000
#define MAX_LINE_LEN 1000

#define EXPLODED 1
#define NOT_EXPLODED 0

typedef struct {
  char *memory;
  size_t size;
} MemoryStruct;

extern int phase_num;

void bomb_init(void);

void bomb_cleanup(void);

void bomb_exit(void);

void explode_bomb(char *student_input);

char *get_phase_info(void);

void defuse_phase(char *student_input);

void explode_bomb(char *student_input);

char *read_line(void);

#endif
