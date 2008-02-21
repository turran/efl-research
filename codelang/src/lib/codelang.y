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

int yylex(void);   
void yyerror(char *format, ...);
void parse_error(void);
extern char *cur_file;                                          
extern int lnum;
extern int col;
static int section;

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
	   cur_file, lnum, col);
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

code: /* empty */
	| code class
	;

class: { section = IN_CLASS;  } access CLASS_TOK WORD O_BRACE class_code C_BRACE
	{
		int ret = class_parse($3);
		if (ret == CLASS_ALREADY_DEFINED)
			yyerror("Class '%s' already defined.\n", $2);		
	}
	;

class_code: /* empty */
	| method
	| member
	;

member: { section = IN_MEMBER; } access type WORD SEMICOLON
	{
		int ret = member_parse($3);
		if (ret == MEMBER_ALREADY_DEFINED)
			yyerror("Member '%s' already defined.\n", $3);
	}
	;

access: /* empty */
	| PUBLIC_TOK
	| PRIVATE_TOK
	| PROTECTED_TOK
	{
		int ret = access_parse($1, section);
		printf("parsing access... %s\n", $1);
		if (ret == ACCESS_INVALID_NAME)
			yyerror("Invalid access specification '%s'\n", $1);
	}
	;

declarations: /* empty */
	| declarations declaration
	;

declaration:
	| variable
	| method
	;

variable: { section = IN_VAR; } type WORD SEMICOLON 
	{
		int ret = var_parse($2);
		if (ret == VAR_ILLEGAL_NAME)
			yyerror("Illegal variable name: '%s'\n", $2);
		else if (ret == VAR_ALREADY_DEFINED)
			yyerror("Variable '%s' is already defined in this scope.\n", $2);
	}
	;

type: WORD 
	{		
		if (!type_parse($1))
			yyerror("Type not found: '%s'\n", $1);

		switch(section)
		{
			case IN_METHOD:
				method_type_set(last_method, $1);
				break;

			case IN_VAR:
				var_type_set(last_var, $1);
				break;

			default:
				yyerror("Trying to set a type where a type does not belong.\n");
				break;
		}
	}
	;

method: access type WORD O_PAREN C_PAREN O_BRACE method_body C_BRACE
	;

method_body: /* empty */
	| method_body variable 
	{
		printf("method body\n");
	}
	;

%%
  
