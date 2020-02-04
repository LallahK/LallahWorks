/**
 * @file    scan:ner.c
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
static int  ch;                       /* the next source character           */
static int  column_number;            /* the current column number           */
static int read_flag = 0;

static Resword reserved[] = {          /* reserved words                      */
	/* TODO: Populate this array with the appropriate pairs of reserved word
	 * strings and token types, sorted alphabetically by reserved word string.
	 */
	{"(", TOKEN_OPEN_PARENTHESIS},
	{")", TOKEN_CLOSE_PARENTHESIS},
	{"*", TOKEN_MULTIPLY},
	{"+", TOKEN_PLUS},
	{",", TOKEN_COMMA},
	{"-", TOKEN_MINUS},
	{".", TOKEN_CONCATENATE},
	{"/", TOKEN_DIVIDE},
	{":=", TOKEN_GETS},
	{";", TOKEN_SEMICOLON},
	{"<", TOKEN_LESS_THAN},
	{"<=", TOKEN_LESS_EQUAL},
	{"<>", TOKEN_NOT_EQUAL},
	{"=", TOKEN_EQUAL},
	{">", TOKEN_GREATER_THAN},
	{">=", TOKEN_GREATER_EQUAL},
	{"[", TOKEN_OPEN_BRACKET},
	{"]", TOKEN_CLOSE_BRACKET},
	{"and", TOKEN_AND},
	{"array", TOKEN_ARRAY},
	{"begin", TOKEN_BEGIN},
	{"boolean", TOKEN_BOOLEAN},
	{"call", TOKEN_CALL},
	{"do", TOKEN_DO},
	{"else", TOKEN_ELSE},
	{"elsif", TOKEN_ELSIF},
	{"end", TOKEN_END},
	{"false",TOKEN_FALSE},
	{"function", TOKEN_FUNCTION},
	{"get", TOKEN_GET},
	{"if", TOKEN_IF},
	{"integer", TOKEN_INTEGER},
	{"leave", TOKEN_LEAVE},
	{"not", TOKEN_NOT},
	{"or", TOKEN_OR},
	{"put", TOKEN_PUT},
	{"relax", TOKEN_RELAX},
	{"rem", TOKEN_REMAINDER},
	{"source", TOKEN_SOURCE},
	{"string", TOKEN_STRING},
	{"then", TOKEN_THEN},
	{"to", TOKEN_TO},
	{"true", TOKEN_TRUE},
	{"while", TOKEN_WHILE}
}
;

#define NUM_RESERVED_WORDS     (sizeof(reserved) / sizeof(Resword))
#define MAX_INITIAL_STRING_LEN (1024)

/* --- function prototypes -------------------------------------------------- */

/**
 * Readsthe next char from the source file and saves it to the global
 * variable 'ch'
 *
 */
static void next_char(void);
/**
 * Processes a number that has been scanned, by checking if the number is a
 * valid size
 *
 * @param[in]	token
 *		A pointer of type 'Token' with which the value of the number
 *		and the token type is saved
 */
static void process_number(Token *token);
/**
 * Processes a string that has been scanned
 * Throws an error if invalid characters are found, if invalid escape sequences
 * are written or if the string is not closed
 *
 * @param[in]	token
 *		A pointer of type 'Token' with which the value of the string and
 *		the token type is saved
 */
static void process_string(Token *token);
/**
 * Processes a word type to check if it a valid identifier
 * Throws an exception if the word is too long or contains invalid characters
 *
 * @param[in]	token
 *		A pointer of type 'Token' with which the the characters of the word
 * 		and the token type is saved
 */
static void process_word(Token *token);
/**
 * skips comments recursively and throws an error of the comment is
 * not closed
 *
 */
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
	int cmp, low = 0, mid, high = NUM_RESERVED_WORDS;
	char *c;
	/* remove whitespace */
	/* TODO: Skip all whitespace characters before the start of the token. */
	while (ch == ' ' || ch == '\t') {
		next_char();
	}

	/* remember token start */
	/* get the next token */

	position.col = column_number;
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
			case '{':
				skip_comment();
				get_token(token);
				break;
			case '\n':
				next_char();
				get_token(token);
				break;
			default :
				position.col = column_number;

				c = (char*)malloc(sizeof(char) * 4);

				/*In the case of special tokens with a length greater than 1,
				 *reads the next char*/
				*c = ch;
				next_char();
				if (*c == ':' && ch == '=') {
					*(c + 1) = '=';
					*(c + 2) = '\0';
					next_char();
				} else if (*c == '<' && (ch == '>' || ch == '=')) {
					c[1] = ch;
					c[2] = '\0';
					next_char();
				} else if (*c == '=' && (ch == '<' || ch == '>')) {
					*(c + 1) = ch;
					*(c + 2) = '\0';
					next_char();
				} else if (*c == '>' && ch == '=') {
					*(c + 1) = ch;
					*(c + 2) = '\0';
					next_char();
				} else {
					*(c + 1) = '\0';
				}

				/*Binary searches through tokens to find the specefied one*/
				while (1) {
					mid = low + (high - low) / 2;
					cmp = strcmp(c, reserved[mid].word);
					if (high <= low) {
					/*If not found the character must be illegal*/
						leprintf("illegal character '%c' (ASCII #%d)", *c, *c);
						exit(0);
					}

					if (cmp < 0) {
						high = mid;

					} else if (cmp > 0) {
						low = mid + 1;
					} else{
						token->type = reserved[mid].type;
						free(c);
						break;
					}
				}
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

	if (last_read == '\n' && !read_flag) {
		position.line++;
		column_number = 0;
	}

	last_read = ch = fgetc(src_file);
	column_number++;

	/* TODO:
	 * - Get the next character from the source file.
	 * - Increment the line number if the previous character is EOL.
	 * - Reset the global column number when necessary.
	 * - DO NOT USE feof!!!
	 */
}

void process_number(Token *token)
{
	signed int v = 0;
	while (isdigit(ch)) {
		v = v * 10;
		if (INT_MAX - v < (ch - 48)) {
			leprintf("number too large");
			exit(0);
		}
		v = v + (ch - 48);
		next_char();
	}

	token->type = TOKEN_NUMBER;
	token->value = v;
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
	char *p = NULL, *sp, *newsp;

	while (1) {
		if (p == NULL) {
			i = nstring;
			p = (char*)malloc(nstring);
			sp = p;
		}
		if ((sp - p + 1)*sizeof(char) == nstring) {
			i = i * 2;
			newsp = (char*)malloc(i);
			strcpy(newsp, p);
			sp = &newsp[sp - p];
			free(p);
			p = newsp;
		}

		if (ch == EOF) {
			leprintf("string not closed\n");
			exit(0);
		}
		if (!isprint(ch)) {
			position.col = column_number;
			leprintf("non-printable character (ASCII #%d) in string\n", ch);
			exit(0);
		}
		switch (ch) {
			case '"' :
				*sp = '\0';
				position.col = column_number;
				token->type = TOKEN_STRING;
				token->string = (char*)malloc(i);
				strcpy(token->string, p);
				next_char();
				return;
				break;
			case '\\':
				*sp++ = ch;
				next_char();
				switch (ch) {
					case 'n' :
						break;
					case 't' :
						break;
					case '"' :
						break;
					case '\\':
						break;
					default :
						position.col = column_number - 1;
						leprintf("illegal escape code '\\%c' in string", ch);
						exit(0);
						break;
				}
			default :
				*sp++ = ch;
				break;
		}
		next_char();
	}

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
	int cmp, low = 0, mid, high, found = 0;
	char *sp = lexeme;

	position.col = column_number;

	while (isalnum(ch) || ch == '_') {
		*sp++ = ch;
		next_char();
		if (sp - lexeme - 1 == MAX_ID_LENGTH) {
			leprintf("identifier too long");
			exit(0);
		}
	}
	*sp = '\0';

	high = NUM_RESERVED_WORDS - 1;
	while (!found) {
		mid = low + (high - low) / 2;
		cmp = strcmp(lexeme, reserved[mid].word);
		if (high <= low) {
			found = 1;
			token->type = TOKEN_ID;
			strcpy(token->lexeme, lexeme);
		}

		if (cmp == 0) {
			found = 1;
			token->type = reserved[mid].type;
		} else if (cmp < 0) {
			high = mid;
		} else {
			low = mid + 1;
		}
	}
	/* check that the id length is less than the maximum */
	/* TODO */

	/* do a binary search through the array of reserved words */
	/* TODO */

	/* if id was not recognised as a reserved word, it is an identifier */
}

void skip_comment(void)
{
	SourcePos start_pos;

	start_pos.line = position.line;
	start_pos.col = column_number - 1;

	while (ch != EOF) {
		next_char();
		if (ch == '{') {
			next_char();
			skip_comment();
		}
		if (ch == '}') {
			next_char();
			return;
		}
	}
	/* TODO:
	 * - Skip nested comments RECURSIVELY, which is to say, counting strategies
	 *   are not allowed.
	 * - Terminate with an error if comments are not nested properly.
	 */

	/* force line number of error reporting */
	position = start_pos;
	leprintf("comment not closed");
}
