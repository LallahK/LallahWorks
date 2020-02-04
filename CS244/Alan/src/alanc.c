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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "token.h"
#include "error.h"
#include "errmsg.h"
#include "valtypes.h"
#include "symboltable.h"
#include "boolean.h"
#include "codegen.h"

/* --- type definitions ----------------------------------------------------- */

/* TODO: Uncomment the following for use during type checking. */

#if 1
typedef struct variable_s Variable;
struct variable_s {
	char      *id;     /**< variable identifier                       */
	ValType    type;   /**< variable type                             */
	SourcePos  pos;    /**< variable position in the source           */
	Variable  *next;   /**< pointer to the next variable in the list  */
};
#endif

/* --- debugging ------------------------------------------------------------ */

/* TODO: Youor Makefile has a variable called DFLAGS.  If it is set to contain
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
SourcePos pos;
#if 1
ValType return_type;  /**< the return type of the current function */
#endif

/* TODO: Uncomment the previous definition for use during type checking. */

/* --- function prototypes: parser routines --------------------------------- */

void parse_source(void);
void parse_funcdef(void);
void parse_body(void);
void parse_type(ValType *type);
void parse_vardef(void);
void parse_statements(void);
void parse_statement(void);
void parse_assign(void);
void parse_call(void);
void parse_if(void);
void parse_input(void);
void parse_leave(void);
void parse_output(void);
void parse_while(void);
void parse_expr(ValType *type);
void parse_relop(void);
void parse_simple(ValType *type);
void parse_addop(void);
void parse_term(ValType *type);
void parse_mulop(void);
void parse_factor(ValType *type);


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

#if 1
void check_types(ValType type1, ValType type2, SourcePos *pos, ...);
#endif
void expect(TokenType type);
void expect_id(char **id);
int isType();
int isExpr();
ValType leave_type;
#if 1
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
#if 1
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
	init_symbol_table();
	init_code_generation();

	/* compile */
	get_token(&token);
	parse_source();

	/* produce the object code, and assemble */
	/* TODO: For final submission. */
	make_code_file();
	assemble(jasmin_path);
	/* release allocated resources */
	release_symbol_table();
	/*release_code_generation();*/
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

	IDprop *prop = malloc(sizeof(IDprop));

	pos.col = 0;
	pos.line = 0;

	expect(TOKEN_SOURCE);
	expect_id(&class_name);

	set_class_name(class_name);

	while (token.type == TOKEN_FUNCTION) {
		parse_funcdef();
	}
	prop->type = TYPE_NONE;
	prop->params = NULL;
	prop->offset = 1;
	init_subroutine_codegen("main", prop);
	parse_body();
	close_subroutine_codegen(get_variables_width());
	free(class_name);
	DBG_end("</source>");
}

/**
 * function = “function” id “(” [type id {“,” type id}] “)”
   	[“to” type] body
 */
void parse_funcdef(void)
{
	IDprop *p, *vp;
	ValType t, *par;
	Variable *v, *vi;
	int count = 0, i;
	char *func_name, *s;

	expect(TOKEN_FUNCTION);

	pos = position;
	expect_id(&func_name);
	if (find_name(func_name, &p)) {
		abort_compile_pos(&pos, ERR_MULTIPLE_DEFINITION, token.lexeme);
	}

	expect(TOKEN_OPEN_PARENTHESIS);

	if (!(token.type == TOKEN_CLOSE_PARENTHESIS)) {
		parse_type(&t);
		expect_id(&s);
		if (find_name(s, &p)) {
			abort_compile(ERR_MULTIPLE_DEFINITION, token.lexeme);
		}
		vi = variable(s, t, pos);
		count++;
		v = vi;

		while (token.type == TOKEN_COMMA) {
			get_token(&token);
			parse_type(&t);
			expect_id(&s);
			v->next = variable(s, t, pos);
			v = v->next;
			if (find_name(v->id, &p)) {
				abort_compile(ERR_MULTIPLE_DEFINITION, token.lexeme);
			}
			count++;
		}
	}
	expect(TOKEN_CLOSE_PARENTHESIS);

	par = (ValType*)malloc(sizeof(ValType*) * count);
	if (count > 0) {
		v = vi;
		for (i = 0; i < count; i++) {
			par[i] = v->type;
			v = v->next;
		}
	}
	p = idprop(8, 0, count, par);
	leave_type = 0;

	if (token.type == TOKEN_TO) {
		get_token(&token);
		parse_type(&t);
		leave_type = t;
		p->type = p->type + t;
	}

	if (!open_subroutine(func_name, p)) {
		printf("unable to open\n");
	}

	if (count > 0) {
		v = vi;
		for (i = 0; i < count; i++) {
			vp = idprop(v->type, 0, 0, NULL);
			insert_name(v->id, vp);
			vi = v;
			v = v->next;
			free(vi);
		}
	}

	init_subroutine_codegen(func_name, p);
	parse_body();
	close_subroutine_codegen(get_variables_width());
	close_subroutine();

}

/**
 * body = "begin” { vardef } statements “end”
 */
void parse_body(void)
{
	expect(TOKEN_BEGIN);
	while (isType(token.type)) {
		parse_vardef();
	}
	parse_statements();
	if (leave_type == 0) {
		gen_1(JVM_RETURN);
	}
	expect(TOKEN_END);
}

/**
 * type = (“boolean” ∣ “integer”) [“array”]
 */
void parse_type(ValType *type)
{
	ValType t = 0;
	switch (token.type) {
		case TOKEN_BOOLEAN:
			t = t + 2;
			get_token(&token);
			break;
		case TOKEN_INTEGER:
			t = t + 4;
			get_token(&token);
			break;
		default:
			abort_compile(ERR_TYPE_EXPECTED);
			break;
	}
	if (token.type == TOKEN_ARRAY) {
		t = t + 1;
		get_token(&token);
	}
	*type = t;
}

/**
 * vardef = type id {“,” id} “;”
 */
void parse_vardef(void)
{
	IDprop *p = NULL;
	ValType t;
	char *c;

	if (!(isType(token.type))) {
		abort_compile(ERR_ARRAY_ALLOCATION_OR_EXPRESSION_EXPECTED);
	}
	parse_type(&t);

	pos = position;
	expect_id(&c);

	p = idprop(t, 0, 0, NULL);
	if (!insert_name(c, p)) {
		position = pos;
		abort_compile_pos(&pos, ERR_MULTIPLE_DEFINITION, c);
	}

	while (token.type == TOKEN_COMMA) {
		get_token(&token);
		pos = position;
		expect_id(&c);
		p = idprop(t, 0, 0, NULL);
		if (!insert_name(c, p)) {
			position = pos;
			abort_compile(ERR_MULTIPLE_DEFINITION, token.lexeme);
		}
	}
	expect(TOKEN_SEMICOLON);
}

/**
 * statements = “relax” ∣ statement {“;” statement}
 */
void parse_statements(void)
{
	if (token.type == TOKEN_RELAX) {
		expect(TOKEN_RELAX);
	} else {
		parse_statement();
		while (token.type == TOKEN_SEMICOLON) {
			get_token(&token);
			parse_statement();
		}
	}
}

/**
 * statement = assign ∣ call ∣ if ∣ input | leave ∣ output ∣ while
 */
void parse_statement(void)
{
	switch (token.type) {
		case TOKEN_ID:
			parse_assign();
			break;
		case TOKEN_CALL:
			parse_call();
			break;
		case TOKEN_IF:
			parse_if();
			break;
		case TOKEN_GET:
			parse_input();
			break;
		case TOKEN_LEAVE:
			parse_leave();
			break;
		case TOKEN_PUT:
			parse_output();
			break;
		case TOKEN_WHILE:
			parse_while();
			break;
		default:
			abort_compile(ERR_STATEMENT_EXPECTED);
			break;
	}
}

/**
 * assign = ⟨id⟩ [“[” simple “]”] “:=” (expr ∣ “array” simple)
 */
void parse_assign(void)
{
	ValType t1, t2;
	IDprop *p;
	int index = 0;
	char *s;
	SourcePos posi, posi2;

	posi = position;
	expect_id(&s);
	if (!find_name(s, &p)) {
		abort_compile_pos(&posi, ERR_UNKNOWN_IDENTIFIER, s);
	}

	t1 = p->type;

	if (IS_CALLABLE_TYPE(t1)) {
		position = posi;
		abort_compile_pos(&posi, ERR_NOT_A_VARIABLE, s);
	}

	if (token.type == TOKEN_OPEN_BRACKET) {
		gen_2(JVM_ALOAD, p->offset);
		if (!IS_ARRAY(t1)) {
			abort_compile(ERR_NOT_AN_ARRAY, s);
		}

		get_token(&token);
		posi = position;
		parse_simple(&t2);

		check_types(t2, TYPE_INTEGER, &posi, "for array index of '%s'", s);
		SET_BASE_TYPE(return_type);
		index = 1;
		expect(TOKEN_CLOSE_BRACKET);
	}

	expect(TOKEN_GETS);

	if (token.type == TOKEN_ARRAY) {
		posi2 = position;
		get_token(&token);
		posi = position;
		parse_simple(&t2);

		if (!IS_ARRAY(t1)) {
			abort_compile(ERR_NOT_AN_ARRAY, s);
		}

		return_type = t1;
		SET_BASE_TYPE(return_type);
		if (index) {
			check_types(TYPE_ARRAY + return_type, return_type, &posi2,
			"for allocation to indexed array '%s'", s);
		}
		check_types(t2, TYPE_INTEGER, &posi, "for array size of '%s'", s);
		if (IS_INTEGER_TYPE(return_type)) {
			gen_newarray(T_INT);
		} else {
			gen_newarray(T_INT);
		}
		gen_2(JVM_ASTORE, p->offset);
	} else {
		pos = position;
		parse_expr(&t2);

		if (IS_ARRAY(t1) && index) {
			return_type = t1;
			SET_BASE_TYPE(return_type);

			if (IS_ARRAY(t2)) {
				check_types(t2, return_type, &pos,
				"for allocation to index array '%s'", s);
			}
			check_types(t2, return_type, &pos, "for assignment to '%s'", s);
			gen_1(JVM_IASTORE);
		} else if (IS_ARRAY(t1)) {
			return_type = t1;
            SET_BASE_TYPE(return_type);
			check_types(t2, TYPE_ARRAY + return_type, &pos, "for assignment to '%s'", s);
			gen_2(JVM_ASTORE, p->offset);
		} else if (IS_VARIABLE(t1)) {
			check_types(t2, t1, &pos, "for assignment to '%s'", s);
			gen_2(JVM_ISTORE, p->offset);
		} else {
	         abort_compile_pos(&pos, ERR_UNKNOWN_IDENTIFIER, s);
		}
	}
}

/**
 * call = “call” id “(” [expr {“,” expr}] “)”
 */
void parse_call()
{
	ValType t, t2;
	IDprop *p;
	unsigned int npar, i = 0;
	char *c;
	SourcePos pos, posi;
	Variable *vi, *v;

	expect(TOKEN_CALL);

	pos = position;
	posi = position;
	expect_id(&c);
	if (!find_name(c, &p)) {
		position = pos;
		abort_compile(ERR_UNKNOWN_IDENTIFIER, c);
	}
	if (!IS_PROCEDURE(p->type)) {
		position = pos;
		abort_compile(ERR_NOT_A_PROCEDURE, c);
	}
	expect(TOKEN_OPEN_PARENTHESIS);

	npar = p->nparams;
	vi = NULL;
	if (!(token.type == TOKEN_CLOSE_PARENTHESIS)) {
		pos = position;
		parse_expr(&t);
		posi = position;

		vi = v = variable("", t, pos);
		i++;
		while (token.type == TOKEN_COMMA) {
			posi = position;
			get_token(&token);
			pos = position;
			parse_expr(&t);

			v->next = variable("", t, pos);
			v = v->next;
			i++;
		}
	}

	if (i < npar) {
		abort_compile(ERR_TOO_FEW_ARGUMENTS, c);
	} else if (i > npar) {
		abort_compile_pos(&posi, ERR_TOO_MANY_ARGUMENTS, c);
	} else if (token.type == TOKEN_COMMA) {
		abort_compile_pos(&posi, ERR_TOO_MANY_ARGUMENTS, c);
	}

	v = vi;
	i = 0;
	while (v != NULL) {
		if (IS_FUNCTION(v->type)) {
			t2 = v->type;
			SET_RETURN_TYPE(t2);
			check_types(p->params[i], t2,
			            &v->pos, "for parameter %d of call to '%s'", i + 1, c);
		} else {
			check_types(p->params[i], v->type,
        	&v->pos, "for parameter %d of call to '%s'", i + 1, c);
		}
        v = v->next;
        i++;
	}
	gen_call(c, p);
	expect(TOKEN_CLOSE_PARENTHESIS);
}

/**
 * if = “if” expr “then” statements {“elsif” expr “then” statements}
 * [“else” statements] “end”
 */
void parse_if()
{
	ValType t;
	SourcePos posi;
	int l2, lend;

	l2 = get_label();
	lend = get_label();
	expect(TOKEN_IF);

	posi = position;
	parse_expr(&t);

	check_types(t, TYPE_BOOLEAN, &posi, "for %s guard",
	get_token_string(TOKEN_IF));

	expect(TOKEN_THEN);

	gen_2_label(JVM_IFEQ, l2);
	parse_statements();
	gen_2_label(JVM_GOTO, lend);
	gen_label(l2);
	while (token.type == TOKEN_ELSIF) {
		expect(TOKEN_ELSIF);

		posi = position;
		parse_expr(&t);
		l2 = get_label();
		gen_2_label(JVM_IFEQ, l2);

		check_types(t, TYPE_BOOLEAN, &posi, "for %s guard",
			get_token_string(TOKEN_ELSIF));

		expect(TOKEN_THEN);
		parse_statements();
		gen_2_label(JVM_GOTO, lend);
		gen_label(l2);
	}
	if (token.type == TOKEN_ELSE) {
		expect(TOKEN_ELSE);
		parse_statements();
	}
	gen_label(lend);
	expect(TOKEN_END);
}

/**
 * input = “get” id [“[” simple “]”]
 */
void parse_input()
{
	ValType t, base_type;
	IDprop *p;
	char *s;

	expect(TOKEN_GET);

	pos = position;
	expect_id(&s);
	if (!find_name(s, &p)) {
		abort_compile_pos(&pos, ERR_UNKNOWN_IDENTIFIER, s);
	}
	if (IS_CALLABLE_TYPE(p->type)) {
		abort_compile_pos(&pos, ERR_SCALAR_EXPECTED, s);
	}

	base_type = p->type;
	SET_BASE_TYPE(base_type);
	if (token.type == TOKEN_OPEN_BRACKET) {
		gen_2(JVM_ALOAD, p->offset);
		get_token(&token);
		parse_simple(&t);
		expect(TOKEN_CLOSE_BRACKET);
		gen_read(base_type);
		gen_1(JVM_IASTORE);
	} else {
		if (IS_ARRAY(p->type)) {
			abort_compile_pos(&pos, ERR_SCALAR_EXPECTED, s);
		}
		gen_read(base_type);
		gen_2(JVM_ISTORE, p->offset);
	}
}

/**
 * “leave” [expr]
 */
void parse_leave()
{
	ValType t = 0;
	SourcePos posi, posi2;

	posi2 = position;
	expect(TOKEN_LEAVE);
	posi = position;

	if (isExpr()) {
		parse_expr(&t);
	}

	if (leave_type == 0 && t!= 0) {
		abort_compile_pos(&posi,
			ERR_LEAVE_EXPRESSION_NOT_ALLOWED_FOR_PROCEDURE);
	} else if (leave_type != 0) {
		if (t == 0) {
			abort_compile_pos(&posi2,
				ERR_MISSING_LEAVE_EXPRESSION_FOR_FUNCTION);
		}
		check_types(t, leave_type, &posi, "for 'leave' statement");
		if (IS_ARRAY(leave_type)) {
			gen_1(JVM_ARETURN);
		} else {
			gen_1(JVM_IRETURN);
		}
	}
}

/**
 * output = “put” (string ∣ expr) { “.” (string ∣ expr) }
 */
void parse_output()
{
	ValType t;

	expect(TOKEN_PUT);

	if (token.type == TOKEN_STRING) {
		get_token(&token);
		gen_print_string(token.string);
	} else {
		parse_expr(&t);
		if (token.type == TYPE_BOOLEAN) {
			gen_print(TYPE_BOOLEAN);
		} else {
			gen_print(TYPE_INTEGER);
		}
	}

	while (token.type == TOKEN_CONCATENATE) {
		get_token(&token);
		if (token.type == TOKEN_STRING) {
			get_token(&token);
			gen_print_string(token.string);
		} else {
			parse_expr(&t);
			if (token.type == TYPE_BOOLEAN) {
				gen_print(TYPE_BOOLEAN);
			} else {
				gen_print(TYPE_INTEGER);
			}
		}
	}
}

/**
 * while = “while” expr “do” statements “end”.
 */
void parse_while()
{
	ValType t;
	SourcePos posi;
	int lend, lstart;

	posi = position;
	posi.col++;
	expect(TOKEN_WHILE);
	pos = position;
	lend = get_label();
	lstart = get_label();

	gen_label(lstart);
	parse_expr(&t);

	gen_2(JVM_LDC, 1);
	gen_cmp(JVM_IF_ICMPEQ);
	gen_2_label(JVM_IFEQ, lend);

	check_types(t, TYPE_BOOLEAN, &pos, "for %s guard",
		get_token_string(TOKEN_WHILE));

	expect(TOKEN_DO);

	parse_statements();
	gen_2_label(JVM_GOTO, lstart);

	expect(TOKEN_END);
	gen_label(lend);

}

/**
 * expr = simple [relop simple]
 * “=” ∣ “>=” ∣ “>” ∣ “<=” ∣ “<” ∣ “<>”
 */
void parse_expr(ValType *type)
{
	ValType t1, t2;
	parse_simple(&t1);
	SourcePos posi;

	posi = position;
	switch (token.type) {
		case TOKEN_EQUAL:
			get_token(&token);
			parse_simple(&t2);
			check_types(t1, t2, &posi, "for operator %s",
				get_token_string(TOKEN_EQUAL));
			*type = TYPE_BOOLEAN;
			gen_cmp(JVM_IF_ICMPEQ);
			break;
		case TOKEN_GREATER_EQUAL:
			get_token(&token);
			parse_simple(&t2);
			check_types(t1, TYPE_INTEGER, &posi, "for operator %s",
				get_token_string(TOKEN_GREATER_EQUAL));
            check_types(t2, TYPE_INTEGER, &posi, "for operator %s",
				get_token_string(TOKEN_GREATER_EQUAL));
			*type = TYPE_BOOLEAN;
			gen_cmp(JVM_IF_ICMPGE);
			break;
		case TOKEN_GREATER_THAN:
			get_token(&token);
			parse_simple(&t2);
			check_types(t1, TYPE_INTEGER, &posi, "for operator %s",
				get_token_string(TOKEN_GREATER_THAN));
			check_types(t2, TYPE_INTEGER, &posi, "for operator %s",
				get_token_string(TOKEN_GREATER_THAN));
			*type = TYPE_BOOLEAN;
			gen_cmp(JVM_IF_ICMPGT);
			break;
		case TOKEN_LESS_EQUAL:
			get_token(&token);
			parse_simple(&t2);
			check_types(t1, TYPE_INTEGER, &posi, "for operator %s",
				get_token_string(TOKEN_LESS_EQUAL));
            check_types(t2, TYPE_INTEGER, &posi, "for operator %s",
				get_token_string(TOKEN_LESS_EQUAL));
            *type = TYPE_BOOLEAN;
			gen_cmp(JVM_IF_ICMPLE);
			break;
		case TOKEN_LESS_THAN:
			get_token(&token);
			parse_simple(&t2);
			check_types(t1, TYPE_INTEGER, &posi, "for operator %s",
				get_token_string(TOKEN_LESS_THAN));
            check_types(t2, TYPE_INTEGER, &posi, "for operator %s",
				get_token_string(TOKEN_LESS_THAN));
            *type = TYPE_BOOLEAN;
			gen_cmp(JVM_IF_ICMPLT);
			break;
		case TOKEN_NOT_EQUAL:
			get_token(&token);
			parse_simple(&t2);
			check_types(t1, t2, &posi, "for operator %s",
				get_token_string(TOKEN_NOT_EQUAL));
			*type = TYPE_BOOLEAN;
			gen_cmp(JVM_IF_ICMPNE);
			break;
		default :
			*type = t1;
			break;
	}
}

/**
 * simple = [“-”] term {addop term}
 */
void parse_simple(ValType *type)
{
	ValType t1, t2;
	int min = 0;
	TokenType t;
	SourcePos posi;

	if (token.type == TOKEN_MINUS) {
		posi = position;
		get_token(&token);
		min = 1;
	}

	pos = position;
	parse_term(&t1);

	if (min) {
		if (IS_ARRAY(t1)) {
			abort_compile_pos(&pos, ERR_ILLEGAL_ARRAY_OPERATION, "'-'");
		}
		check_types(t1, TYPE_INTEGER, &pos, "for unary minus");
		gen_1(JVM_INEG);
	}

	min = 0;
	while (token.type == TOKEN_MINUS || token.type == TOKEN_OR ||
	token.type == TOKEN_PLUS) {
		min++;
		t = token.type;
		posi = position;
		get_token(&token);

		parse_term(&t2);

		if (IS_ARRAY(t2)) {
			abort_compile_pos(&posi, ERR_ILLEGAL_ARRAY_OPERATION,
				get_token_string(t));
		}
		switch (t) {
			case TOKEN_MINUS:
				check_types(t1, TYPE_INTEGER, &posi, "for operator %s",
                    get_token_string(TOKEN_PLUS));
				check_types(t2, TYPE_INTEGER, &posi, "for operator %s",
					get_token_string(TOKEN_MINUS));
				gen_1(JVM_ISUB);
				break;
			case TOKEN_PLUS:
				check_types(t1, TYPE_INTEGER, &posi, "for operator %s",
				    get_token_string(TOKEN_PLUS));
	            check_types(t2, TYPE_INTEGER, &posi, "for operator %s",
					get_token_string(TOKEN_PLUS));
				gen_1(JVM_IADD);
				break;
			case TOKEN_OR:
				check_types(t1, TYPE_BOOLEAN, &posi, "for operator %s",
				    get_token_string(TOKEN_OR));
				check_types(t2, TYPE_BOOLEAN, &posi, "for operator %s",
					get_token_string(TOKEN_OR));
				gen_1(JVM_IOR);
				break;
			default:
				break;
		}
	}
	*type = t1;
}

/**
 * term = factor {mulop factor}
 */
void parse_term(ValType *type)
{
	TokenType t;
	ValType t1, t2;
	SourcePos posi;

	parse_factor(&t1);

	t = token.type;

	while (t == TOKEN_AND || t == TOKEN_DIVIDE || 	t == TOKEN_MULTIPLY ||
	t == TOKEN_REMAINDER) {
		posi = position;
		get_token(&token);
		parse_factor(&t2);
		if (t == TOKEN_AND) {
			check_types(t1, TYPE_BOOLEAN, &posi, "for operator %s",
				get_token_string(TOKEN_AND));
			check_types(t2, TYPE_BOOLEAN, &posi, "for operator %s",
				get_token_string(TOKEN_AND));
		} else {
			check_types(t1, TYPE_INTEGER, &posi, "for operator %s",
				get_token_string(t));
			check_types(t2, TYPE_INTEGER, &posi, "for operator %s",
				get_token_string(t));
		}
		switch (t) {
			case TOKEN_AND : gen_1(JVM_IAND);
				break;
			case TOKEN_DIVIDE : gen_1(JVM_IDIV);
				break;
			case TOKEN_MULTIPLY : gen_1(JVM_IMUL);
				break;
			case TOKEN_REMAINDER : gen_1(JVM_IREM);
				break;
			default:
				break;
		}
		t = token.type;
	}
	*type = t1;
}

/**
 * factor = id [“[” simple “]” | “(” [ expr {“,” expr} ] “)”] ∣ num ∣
 * “(” expr “)” ∣ “not” factor ∣ “true” ∣ “false”
 */
void parse_factor(ValType *type)
{
	ValType t1, t2;
	IDprop *id;
	Boolean found;
	int npar, i;
	char *c;
	SourcePos posi;
	Variable *vi, *v;

	posi = position;
	switch (token.type) {
		case TOKEN_ID:
			expect_id(&c);
			found = find_name(c, &id);

			if (!found) {
				abort_compile_pos(&posi, ERR_UNKNOWN_IDENTIFIER, c);
			}

			if (token.type == TOKEN_OPEN_BRACKET) {
				get_token(&token);
				t2 = id->type;
				if (!IS_ARRAY(t2)) {
					abort_compile_pos(&posi, ERR_NOT_AN_ARRAY, c);
				}
				gen_2(JVM_ALOAD, id->offset);
				parse_simple(&t1);

				check_types(t1, TYPE_INTEGER, &posi,
					"for array index of '%s'", c);

				return_type = t2;
				*type = SET_BASE_TYPE(return_type);

				expect(TOKEN_CLOSE_BRACKET);
				gen_1(JVM_IALOAD);
			} else if (token.type == TOKEN_OPEN_PARENTHESIS) {
				expect(TOKEN_OPEN_PARENTHESIS);
				if (!IS_FUNCTION(id->type)) {
					abort_compile_pos(&posi, ERR_NOT_A_FUNCTION, c);
				}
				i = 0;
				npar = id->nparams;
				vi = NULL;
				if (!(token.type == TOKEN_CLOSE_PARENTHESIS)) {
					posi = position;

					parse_expr(&t1);
					vi = v = variable(token.lexeme, t1, posi);
					i++;

					while (token.type == TOKEN_COMMA) {
						get_token(&token);
						posi = position;
						parse_expr(&t1);
						v->next = variable(token.lexeme, t1, posi);
						i++;
					}
				}

				if (i < npar) {
					abort_compile(ERR_TOO_FEW_ARGUMENTS, c);
				} else if (i > npar) {
					position = posi;
					abort_compile(ERR_TOO_MANY_ARGUMENTS, c);
				} else if (token.type == TOKEN_COMMA) {
					abort_compile(ERR_TOO_MANY_ARGUMENTS, c);
				}
				expect(TOKEN_CLOSE_PARENTHESIS);

				v = vi;
				i = 0;
				while (v != NULL) {
					check_types(v->type, id->params[i],
					&v->pos, "for parameter %d of call to '%s'", i + 1, c);
					i++;
					v = v->next;
				}

				*type = SET_RETURN_TYPE(id->type);
				gen_call(c, id);
			} else {
				*type = id->type;
				if (IS_ARRAY(*type)) {
					gen_2(JVM_ALOAD, id->offset);
				} else {
					gen_2(JVM_ILOAD, id->offset);
				}
			}
			break;
		case TOKEN_NUMBER:
			*type = TYPE_INTEGER;
			gen_2(JVM_LDC, token.value);
			get_token(&token);
			break;
		case TOKEN_OPEN_PARENTHESIS:
			get_token(&token);
			parse_expr(&t1);
			expect(TOKEN_CLOSE_PARENTHESIS);
			*type = t1;
			break;
		case TOKEN_NOT:
			get_token(&token);
			pos = position;
			parse_factor(&t1);
			check_types(t1, TYPE_BOOLEAN, &pos, "for 'not'");
			*type = t1;

			gen_2(JVM_LDC, 1);
			gen_1(JVM_IXOR);

			break;
		case TOKEN_TRUE:
			get_token(&token);
			*type = TYPE_BOOLEAN;
			gen_2(JVM_LDC, 1);
			break;
		case TOKEN_FALSE:
			get_token(&token);
			*type = TYPE_BOOLEAN;
			gen_2(JVM_LDC, 0);
			break;
		default :
			abort_compile(ERR_FACTOR_EXPECTED);
			break;
	}
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

#if 1
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

int isType()
{
	if (token.type == TOKEN_BOOLEAN || token.type == TOKEN_INTEGER) {
		return 1;
	} else {
		return 0;
	}
}

int isExpr()
{
	switch (token.type) {
		case TOKEN_MINUS:
			return 1;
			break;
		case TOKEN_ID:
			return 1;
			break;
		 case TOKEN_OPEN_PARENTHESIS:
		    return 1;
			break;
		case TOKEN_NUMBER:
			return 1;
			break;
		case TOKEN_NOT:
			return 1;
			break;
		case TOKEN_TRUE:
			return 1;
			break;
		case TOKEN_FALSE:
			return 1;
			break;
		default:
			return 0;
			break;
	}
}
/* TODO: Uncomment the following functions for use during type checking. */

#if 1
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
	char expstr[MAX_MESSAGE_LENGTH], *s, estr[MAX_MESSAGE_LENGTH];
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

		case ERR_TYPE_EXPECTED:
			leprintf(expstr, "type");
			break;

		case ERR_STATEMENT_EXPECTED:
			leprintf(expstr, "statement");
			break;

		case ERR_ARRAY_ALLOCATION_OR_EXPRESSION_EXPECTED:
			leprintf(expstr, "array allocation", "expression");
			break;

		case ERR_EXPRESSION_OR_STRING_EXPECTED:
			leprintf(expstr, "expression", "string");
			break;

		case ERR_ILLEGAL_ARRAY_OPERATION:
			snprintf(estr, MAX_MESSAGE_LENGTH,
			"%s is an illegal array operation", s);
			leprintf(estr);
			break;

		case ERR_LEAVE_EXPRESSION_NOT_ALLOWED_FOR_PROCEDURE:
			snprintf(estr, MAX_MESSAGE_LENGTH,
            "a leave expression is not allowed for a procedure");
            leprintf(estr);
			break;

		case ERR_MISSING_LEAVE_EXPRESSION_FOR_FUNCTION:
			snprintf(estr, MAX_MESSAGE_LENGTH,
            "missing leave expression for a function");
            leprintf(estr);
			break;

		case ERR_MULTIPLE_DEFINITION:
			snprintf(estr, MAX_MESSAGE_LENGTH,
            "multiple definition of '%s'", s);
            leprintf(estr);
			break;

	    case ERR_NOT_A_FUNCTION:
			snprintf(estr, MAX_MESSAGE_LENGTH,
            "'%s' is not a function", s);
            leprintf(estr);
			break;

		case ERR_NOT_AN_ARRAY:
			snprintf(estr, MAX_MESSAGE_LENGTH,
            "'%s' is not an array", s);
            leprintf(estr);
			break;

	    case ERR_NOT_A_PROCEDURE:
			snprintf(estr, MAX_MESSAGE_LENGTH,
            "'%s' is not a procedure", s);
            leprintf(estr);
			break;

		case ERR_NOT_A_VARIABLE:
			snprintf(estr, MAX_MESSAGE_LENGTH,
            "'%s' is not a variable", s);
            leprintf(estr);
			break;

		case ERR_SCALAR_EXPECTED:
			snprintf(estr, MAX_MESSAGE_LENGTH,
            "expected scalar variable instead of '%s'", s);
            leprintf(estr);
			break;

		case ERR_TOO_FEW_ARGUMENTS:
			snprintf(estr, MAX_MESSAGE_LENGTH,
            "too few arguments for call to '%s'", s);
            leprintf(estr);
			break;

		case ERR_TOO_MANY_ARGUMENTS:
			snprintf(estr, MAX_MESSAGE_LENGTH,
            "too many arguments for call to '%s'", s);
            leprintf(estr);
			break;

		case ERR_UNKNOWN_IDENTIFIER:
			snprintf(estr, MAX_MESSAGE_LENGTH,
            "unknown identifier '%s'", s);
            leprintf(estr);
			break;

		default:
			leprintf("Unreachable code");
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


