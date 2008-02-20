%{
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "Codelang.h"
   
#define YYSTYPE char *
#define YYERROR_VERBOSE 1

int yylex(void);   
void yyerror(char *format, ...);
void parse_error(void);
extern char *cur_file;                                          
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
	   cur_file, lnum, col);
   exit(-1);                                                               
} 
   
int yywrap()
{
   return 1;
} 
 
%}

%token COLON NUMBER WORD STR SEMICOLON CLASS_TOK 
%token PUBLIC_TOK PRIVATE_TOK PROTECTED_TOK
%token FILENAME QUOTE O_BRACE C_BRACE O_PAREN C_PAREN

%%  

code: /* empty */
	| code class
	;

class: access CLASS_TOK WORD O_BRACE class_code C_BRACE
	{
		int ret = parse_class($3);
		if (ret == CLASS_ALREADY_DEFINED)
			yyerror("Class '%s' already defined.\n", $2);		
	}
	;

class_code: /* empty */
	| method
	| member
	;

member: access type WORD SEMICOLON
	{
		int ret = parse_member($3);
		if (ret == MEMBER_ALREADY_DEFINED)
			yyerror("Member '%s' already defined.\n", $3);
	}
	;

access: /* empty */
	| PUBLIC_TOK
	| PRIVATE_TOK
	| PROTECTED_TOK
	{
		int ret = parse_access($1);
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

variable: access type WORD SEMICOLON 
	{
		int ret = parse_variable($2);
		if (ret == VAR_ILLEGAL_NAME)
			yyerror("Illegal variable name: '%s'\n", $2);
		else if (ret == VAR_ALREADY_DEFINED)
			yyerror("Variable '%s' is already defined in this scope.\n", $2);
	}
	;

type: WORD 
	{
		if (!parse_type($1))
			yyerror("Type not found: '%s'\n", $1);
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
  
