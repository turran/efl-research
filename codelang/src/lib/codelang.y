%{
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "Codelang.h"
   
#define YYSTYPE char *
#define YYERROR_VERBOSE 1

extern method_t last_method;
extern class_t last_class;
extern var_t last_var;
extern member_t last_member;

static type_t tp;
static access_t ac;

int yylex(void);   
void yyerror(char *format, ...);
void parse_error(void);
extern char *file_in;                                          
extern int lnum;
extern int col;

void yyerror(char *format, ...)
{
   va_list ap;
   
   va_start(ap, format);
   vfprintf(stderr, format, ap);
   va_end(ap);
   parse_error();
}
 
void parse_error()
{
   fprintf(stderr, "file: %s, line: %d, column: %d\n\n",
	   file_in, lnum, col);
   exit(-1);                                                               
} 
   
int yywrap()
{
   return 1;
} 
 
%}

%token COLON NUMBER WORD STR SEMICOLON CLASS_TOK MEMBER_TOK
%token PUBLIC_TOK PRIVATE_TOK PROTECTED_TOK VAR_TOK
%token FILENAME QUOTE O_BRACE C_BRACE O_PAREN C_PAREN

%%  

classes: /* empty */
	| classes class
	;

class: access CLASS_TOK WORD 
	{
		int ret;

		ac = access_parse($1);
		ret = class_parse($3);

		if (ret == CLASS_ALREADY_DEFINED)
			yyerror("Class '%s' already defined.\n", $3);	
	}O_BRACE class_code C_BRACE
	;

class_code: /* empty */
	| class_code members methods
	;

members: /* empty */
	| members member
	;

member: 
	access type WORD SEMICOLON
	{
		int ret;

		ac = access_parse($1);
		tp = type_parse($2);
		ret = member_parse($3);

		if (ret == MEMBER_ALREADY_DEFINED)
			yyerror("Member '%s' already defined.\n", $3);

		member_type_set(last_member, tp);
		member_access_set(last_member, ac);
	}
	;

access: /* empty */
	| PUBLIC_TOK
	| PRIVATE_TOK
	| PROTECTED_TOK
	;

variable:
	type WORD SEMICOLON 
	{
		int ret;

		tp = type_parse($1);
		ret = var_parse($2);

		if (ret == VAR_ILLEGAL_NAME)
			yyerror("Illegal variable name: '%s'\n", $3);
		else if (ret == VAR_ALREADY_DEFINED)
			yyerror("Variable '%s' is already defined in this scope.\n", $3);

		var_type_set(last_var, tp);
	}
	;

type: WORD 
	;

methods: /* empty */
	| methods method
	;

method: 
	access type WORD O_PAREN C_PAREN 
	{
		int ret;
		
		ac = access_parse($1);
		tp = type_parse($2);
		ret = method_parse($3);

		if (ret == METHOD_ALREADY_DEFINED)
			yyerror("Method already defined '%s'\n", $5);
		else if (ret == METHOD_INVALID_NAME)
			yyerror("Invalid name for method '%s'\n", $5);

		method_access_set(last_method, ac);
		method_type_set(last_method, tp);

	}O_BRACE method_body C_BRACE
	;

method_body: /* empty */
	| method_body variable 
	{
	}
	;

%%
  
