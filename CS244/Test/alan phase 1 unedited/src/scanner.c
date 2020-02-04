/**
 * @file    scanner.c
 * @brief   The scanner for ALAN-2019.
 * @author  W. H. K. Bester (whkbester@cs.sun.ac.za)
 * @date    2019-08-03
 */

#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "boolean.h"
#include "error.h"
#include "scanner.h"
#include "token.h"

/* --- type definitions and constants --------------------------------------- */

typedef struct {
	char       *word;                  /* the reserved word, i.e., the lexeme */
	TokenType  type;                   /* the associated token type           */
} Resword;

/* --- global static variables ---------------------------------------------- */

static FILE *src_file;                 /* the source file pointer             */
static char  ch;                       /* the next source character           */
static int   column_number;            /* the current column number           */

static Resword reserved[] = {          /* reserved words                      */
	/* TODO: Populate this array with the appropriate pairs of reserved word
	 * strings and token types, sorted alphabetically by reserved word string.
	 */
	{"and", TOKEN_AND}, {"array", TOKEN_ARRAY}, {"begin", TOKEN_BEGIN},
	{"boolean", TOKEN_BOOLEAN}, {"call", TOKEN_CALL}, {"do", TOKEN_DO}, {"else",
	TOKEN_ELSE}, {"elsif", TOKEN_ELSIF}, {"end", TOKEN_END}, {"false",
	TOKEN_FALSE}, {"function", TOKEN_FUNCTION}, {"get", TOKEN_GET}, {"if",
	TOKEN_IF}, {"integer", TOKEN_INTEGER}, {"leave", TOKEN_LEAVE}, {"not",
	TOKEN_NOT}, {"put", TOKEN_PUT}, {"relax", TOKEN_RELAX}, {"source",
	TOKEN_SOURCE}, {"then",	TOKEN_THEN}, {"to", TOKEN_TO}, {"true", TOKEN_TRUE},
	{"while", TOKEN_WHILE}
};

#define NUM_RESERVED_WORDS     (sizeof(reserved) / sizeof(Resword))
#define MAX_INITIAL_STRING_LEN (1024)

/* --- function prototypes -------------------------------------------------- */

static void next_char(void);
static void process_number(Token *token);
static void process_string(Token *token);
static void process_word(Token *token);
static void skip_comment(void);

/* --- scanner interface ---------------------------------------------------- */

void init_scanner(FILE *in_file)
{
	src_file = in_file;
	position.line = 1;
	position.col = column_number = 0;
	next_char();
}

void get_token(Token *token)
{
	/* remove whitespace */
	/* TODO: Skip all whitespace characters before the start of the token. */
	
	while((ch = fgetc(src_file)) == ' ');

	/* remember token start */
	position.col = column_number;

	/* get the next token */
	if (ch != EOF) {
		if (isalpha(ch) || ch == '_') {

			/* process a word */
			process_word(token);

		} else if (isdigit(ch)) {

			/* process a number */
			process_number(token);

		} else switch (ch) {

			/* process a string */
			case '"':
				position.col = column_number;
				next_char();
				process_string(token);
				break;

			/* TODO: Process the other tokens, and trigger comment skipping. */

		}
	} else {
		token->type = TOKEN_EOF;
	}
}

/* --- utility functions ---------------------------------------------------- */

void next_char(void)
{
	static char last_read = '\0';

	/* TODO:
	 * - Get the next character from the source file.
	 * - Increment the line number if the previous character is EOL.
	 * - Reset the global column number when necessary.
	 * - DO NOT USE feof!!!
	 */
}

void process_number(Token *token)
{
	/* TODO:
	 * - Build numbers up to the specificied maximum magnitude.
	 * - Store the value in the appropriate token field.
	 * - Set the appropriate token type.
	 * - "Remember" the correct column number globally.
	 */
}

void process_string(Token *token)
{
	size_t i, nstring = MAX_INITIAL_STRING_LEN;

	/* TODO:
	 * - Allocate heap space of the maximum initial string length.
	 * - If a string is about to overflow while scanning it, double the amount
	 *   of space available.
	 * - ONLY printable ASCII characters are allowed; see man 3 isalpha.
	 * - Check the legality of escape codes.
	 * - Set the appropriate token type.
	 */
}

void process_word(Token *token)
{
	char lexeme[MAX_ID_LENGTH+1];
	int i, cmp, low, mid, high;

	position.col = column_number;

	/* check that the id length is less than the maximum */
	/* TODO */

	/* do a binary search through the array of reserved words */
	/* TODO */

	/* if id was not recognised as a reserved word, it is an identifier */
	token->type = TOKEN_ID;
	strcpy(token->lexeme, lexeme);
}

void skip_comment(void)
{
	SourcePos start_pos;

	/* TODO:
	 * - Skip nested comments RECURSIVELY, which is to say, counting strategies
	 *   are not allowed.
	 * - Terminate with an error if comments are not nested properly.
	 */

	/* force line number of error reporting */
	position = start_pos;
	leprintf("comment not closed");
}
