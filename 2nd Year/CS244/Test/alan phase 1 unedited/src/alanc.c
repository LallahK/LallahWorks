/**
 * @file    alanc.c
 *
 * A syntax analyser (parser) and code generation container for ALAN.
 *
 * All scanning errors are handled in the scanner.  Parser errors are handbled
 * by the <code>AbortCompile</code> function.  System and environment errors,
 * like running out of memory, must be handled in the in which they occur.
 * Transient errors, such as non-existent files, are reported where they occur.
 * There are no warnings, i.e., all errors are fatal and must cause compilation
 * to terminate with abnormal error code.
 *
 * @author  W. H. K. Bester (whkbester@cs.sun.ac.za)
 * @date    2019-08-03
 */

/* TODO: Include the appropriate system and project header files */

/* --- type definitions ----------------------------------------------------- */

/* TODO: Uncomment the following for use during type checking. */

#if 0
typedef struct variable_s Variable;
struct variable_s {
	char      *id;     /**< variable identifier                       */
	ValType    type;   /**< variable type                             */
	SourcePos  pos;    /**< variable position in the source           */
	Variable  *next;   /**< pointer to the next variable in the list  */
};
#endif

/* --- debugging ------------------------------------------------------------ */

/* TODO: Your Makefile has a variable called DFLAGS.  If it is set to contain
 * -DDEBUG_PARSER, it will cause he following prototypes to be included, and the
 * functions to which they refer (given at the end of this file) to be compiled.
 * If, on the other hand, this flag is commented out, by setting DFLAGS to
 * #-DDEBUG_PARSER, these functions will be excluded.  These definitions should
 * be used at the start end end of every parse function.  For an example, the
 * provided parse_program function.
 */

#ifdef DEBUG_PARSER
void debug_start(const char *fmt, ...);
void debug_end(const char *fmt, ...);
void debug_info(const char *fmt, ...);
#define DBG_start(...) debug_start(__VA_ARGS__)
#define DBG_end(...) debug_end(__VA_ARGS__)
#define DBG_info(...) debug_info(__VA_ARGS__)
#else
#define DBG_start(...)
#define DBG_end(...)
#define DBG_info(...)
#endif /* DEBUG_PARSER */

/* --- global variables ----------------------------------------------------- */

Token token;          /**< the lookahead token.type                */
FILE *src_file;       /**< the source code file                    */
char *class_name;     /**< the name of the compiled JVM class file */
#if 0
ValType return_type;  /**< the return type of the current function */
#endif

/* TODO: Uncomment the previous definition for use during type checking. */

/* --- function prototypes: parser routines --------------------------------- */

void parse_source(void);
/* TODO: Add the prototypes for the rest of the parse functions. */

/* --- helper macros -------------------------------------------------------- */

#define STARTS_FACTOR(toktype) \
	(toktype == TOKEN_ID || toktype == TOKEN_NUMBER || \
	 toktype == TOKEN_OPEN_PARENTHESIS || toktype == TOKEN_NOT || \
	 toktype == TOKEN_TRUE || toktype == TOKEN_FALSE)

#define STARTS_EXPR(toktype) /* TODO */

#define IS_ADDOP(toktype) \
	(toktype >= TOKEN_MINUS && toktype <= TOKEN_PLUS)

#define IS_MULOP(toktype) /* TODO */

#define IS_ORDOP(toktype) /* TODO */

#define IS_RELOP(toktype) /* TODO */

#define IS_TYPE_TOKEN(toktype) /* TODO */

/* --- function prototypes: helper routines --------------------------------- */

/* TODO: Uncomment the following prototypes for use during type checking. */

#if 0
void check_types(ValType type1, ValType type2, SourcePos *pos, ...);
#endif
void expect(TokenType type);
void expect_id(char **id);
#if 0
IDprop *idprop(ValType type, unsigned int offset, unsigned int nparams,
		ValType *params);
Variable *variable(char *id, ValType type, SourcePos pos);
#endif

/* --- function prototypes: error reporting --------------------------------- */

void abort_compile(Error err, ...);
void abort_compile_pos(SourcePos *posp, Error err, ...);

/* --- main routine --------------------------------------------------------- */

int main(int argc, char *argv[])
{
#if 0
	char *jasmin_path;
#endif
	/* TODO: Uncomment the previous definition for code generation. */

	/* set up global variables */
	setprogname(argv[0]);

	/* check command-line arguments and environment */
	if (argc != 2) {
		eprintf("usage: %s <filename>", getprogname());
	}

	if ((jasmin_path = getenv("JASMIN_JAR")) == NULL) {
		eprintf("JASMIN_JAR environment variable not set");
	}

	setsrcname(argv[1]);

	/* open the source file, and report an error if it cannot be opened */
	if ((src_file = fopen(argv[1], "r")) == NULL) {
		eprintf("file '%s' could not be opened:", argv[1]);
	}

	/* initialise all compiler units */
	init_scanner(src_file);

	/* compile */
	get_token(&token);
	parse_source();

	/* produce the object code, and assemble */
	/* TODO: For final submission. */

	/* release allocated resources */
	release_symbol_table();
	release_code_generation();
	fclose(src_file);
	freeprogname();
	freesrcname();

#ifdef DEBUG_PARSER
	printf("SUCCESS!\n");
#endif

	return EXIT_SUCCESS;
}

/* --- parser routines ------------------------------------------------------ */

/*
 * source = "source" id { fundef } body.
 */
void parse_source(void)
{
	DBG_start("<source>");

	expect(TOKEN_SOURCE);
	expect_id(&class_name);

	while (token.type == TOKEN_FUNCTION) {
		parse_funcdef();
	}

	parse_body();

	free(class_name);

	DBG_end("</source>");
}

/* TODO: Turn the EBNF into a program by writing one parse function for each
 * production as instructed in the specification.  I suggest you use the
 * production as comment to the function.  Also, you may only report errors
 * through the abort_compile and abort_compile_pos functions.  You must figure
 * out what arguments you should pass for each particular error.  Keep to the
 * EXACT error messages given in the spec.
 */

/* --- helper routines ------------------------------------------------------ */

#define MAX_MESSAGE_LENGTH 256

/* TODO: Uncomment the following function for use during type checking. */

#if 0
void check_types(ValType found, ValType expected, SourcePos *pos, ...)
{
	char buf[MAX_MESSAGE_LENGTH], *s;
	va_list ap;

	if (found != expected) {
		buf[0] = '\0';
		va_start(ap, pos);
		s = va_arg(ap, char *);
		vsnprintf(buf, MAX_MESSAGE_LENGTH, s, ap);
		va_end(ap);
		if (pos != NULL) {
			position = *pos;
		}
		leprintf("incompatible types (%s and %s) %s",
			get_valtype_string(found), get_valtype_string(expected), buf);
	}
}
#endif

void expect(TokenType type)
{
	if (token.type == type) {
		get_token(&token);
	} else {
		abort_compile(ERR_EXPECT, type);
	}
}

void expect_id(char **id)
{
	if (token.type == TOKEN_ID) {
		*id = strdup(token.lexeme);
		get_token(&token);
	} else {
		abort_compile(ERR_EXPECT, TOKEN_ID);
	}
}

/* TODO: Uncomment the following functions for use during type checking. */

#if 0
IDprop *idprop(ValType type, unsigned int offset, unsigned int nparams,
		ValType *params)
{
	IDprop *ip;

	ip = emalloc(sizeof(IDprop));
	ip->type = type;
	ip->offset = offset;
	ip->nparams = nparams;
	ip->params = params;

	return ip;
}

Variable *variable(char *id, ValType type, SourcePos pos)
{
	Variable *vp;

	vp = emalloc(sizeof(Variable));
	vp->id = id;
	vp->type = type;
	vp->pos = pos;
	vp->next = NULL;

	return vp;
}
#endif

/* --- error handling routine ----------------------------------------------- */

void _abort_compile(SourcePos *posp, Error err, va_list args);

void abort_compile(Error err, ...)
{
	va_list args;

	va_start(args, err);
	_abort_compile(NULL, err, args);
	va_end(args);
}

void abort_compile_pos(SourcePos *posp, Error err, ...)
{
	va_list args;

	va_start(args, err);
	_abort_compile(posp, err, args);
	va_end(args);
}

void _abort_compile(SourcePos *posp, Error err, va_list args)
{
	char expstr[MAX_MESSAGE_LENGTH], *s;
	int t;

	if (posp)
		position = *posp;

	snprintf(expstr, MAX_MESSAGE_LENGTH, "expected %%s, but found %s",
		get_token_string(token.type));

	switch (err) {
		case ERR_ILLEGAL_ARRAY_OPERATION:
		case ERR_MULTIPLE_DEFINITION:
		case ERR_NOT_A_FUNCTION:
		case ERR_NOT_A_PROCEDURE:
		case ERR_NOT_A_VARIABLE:
		case ERR_NOT_AN_ARRAY:
		case ERR_SCALAR_EXPECTED:
		case ERR_TOO_FEW_ARGUMENTS:
		case ERR_TOO_MANY_ARGUMENTS:
		case ERR_UNREACHABLE:
		case ERR_UNKNOWN_IDENTIFIER:
			s = va_arg(args, char *);
			break;
		default:
			break;
	}

	switch (err) {

		/* TODO: Add additional cases here as is necessary, referring to
		 * errmsg.h for all possible errors.  Some errors only become possible
		 * to recognise once we add type checking.  Until you get to type
		 * checking, you can handle such errors by adding the default case.
		 * However, you final submission MUST handle all cases explicitly.
		 */

		case ERR_EXPECT:
			t = va_arg(args, int);
			leprintf(expstr, get_token_string(t));
			break;

		case ERR_FACTOR_EXPECTED:
			leprintf(expstr, "factor");
			break;

		case ERR_UNREACHABLE:
			leprintf("unreachable: %s", s);
			break;

	}
}

/* --- debugging output routines -------------------------------------------- */

#ifdef DEBUG_PARSER

static int indent = 0;

void debug_start(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	debug_info(fmt, ap);
	va_end(ap);
	indent += 2;
}

void debug_end(const char *fmt, ...)
{
	va_list ap;

	indent -= 2;
	va_start(ap, fmt);
	debug_info(fmt, ap);
	va_end(ap);
}

void debug_info(const char *fmt, ...)
{
	int i;
	char buf[MAX_MESSAGE_LENGTH], *buf_ptr;
	va_list ap;

	buf_ptr = buf;

	va_start(ap, fmt);

	for (i = 0; i < indent; i++) {
		*buf_ptr++ = ' ';
	}
	vsprintf(buf_ptr, fmt, ap);

	buf_ptr += strlen(buf_ptr);
	snprintf(buf_ptr, MAX_MESSAGE_LENGTH, " in line %d.\n", position.line);
	fflush(stdout);
	fputs(buf, stdout);
	fflush(NULL);

	va_end(ap);
}

#endif /* DEBUG_PARSER */
