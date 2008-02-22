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
extern char *file_in;                                          
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

code: /* empty */
	| code class
	;

class: access CLASS_TOK WORD O_BRACE class_code C_BRACE
	{
		printf("parsing class...\n");
		int ret = class_parse($3);
		if (ret == CLASS_ALREADY_DEFINED)
			yyerror("Class '%s' already defined.\n", $2);		
	}
	;

class_code: /* empty */
	| class_code member method
	;

member: { section = IN_MEMBER; } access  { section = IN_MEMBER; } type WORD SEMICOLON
	{
		printf("parsing member %s\n", $5);
		int ret = member_parse($5);
		if (ret == MEMBER_ALREADY_DEFINED)
			yyerror("Member '%s' already defined.\n", $5);
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

variable: { section = IN_VAR; } type WORD SEMICOLON 
	{
		printf("parseing var %s\n", $3);
		int ret = var_parse($3);
		if (ret == VAR_ILLEGAL_NAME)
			yyerror("Illegal variable name: '%s'\n", $3);
		else if (ret == VAR_ALREADY_DEFINED)
			yyerror("Variable '%s' is already defined in this scope.\n", $3);
	}
	;

type: WORD 
	{		
		int type = type_parse($1);
		printf("parsing type %s\n", $1);
		if (type == TYPE_UNKOWN)
			yyerror("Type not found: '%s'\n", $1);

		switch(section)
		{
			case IN_METHOD:
				method_type_set(last_method, type);
				break;

			case IN_VAR:
				var_type_set(last_var, type);
				break;

			case IN_MEMBER:
				member_type_set(last_member, type);
				break;

			default:
				yyerror("Trying to set a type where a type does not belong (%d %d).\n",
				section, IN_METHOD);
				break;
		}
	}
	;

method: { section = IN_METHOD; } access { section = IN_METHOD; } type WORD
O_PAREN C_PAREN O_BRACE method_body C_BRACE
	{
		printf("parseing method %s\n", $5);
		int ret = method_parse($5);
		if (ret == METHOD_ALREADY_DEFINED)
			yyerror("Method already defined '%s'\n", $5);
		else if (ret == METHOD_INVALID_NAME)
			yyerror("Invalid name for method '%s'\n", $5);
	}
	;

method_body: /* empty */
	| method_body variable 
	{
		printf("method body\n");
	}
	;

%%
  
