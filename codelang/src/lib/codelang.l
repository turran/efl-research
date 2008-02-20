%{
#include <stdio.h>
#include <string.h>
   
#define YYSTYPE char *
      
#include "libcodelang_la-codelang.h"

int yylex(void);
int lnum = 1;
int col = 0;
char *cur_file = NULL;
extern char *filename;

static int comment = 0, cpp_comment = 0;

#define COMMENT_START() BEGIN(SC_COMMENT); comment++;
#define COMMENT_END()   comment--; if (comment <= 0) { comment = 0; BEGIN(INITIAL); }
#define CPP_COMMENT()   comment = 1; cpp_comment = 1;


#define WHITE_SPACE()   col += yyleng;

#define NEW_LINE()  lnum ++; \
		    if (cpp_comment) { \
                        cpp_comment = 0; \
			comment = 0; \
                    } \
                    col = 0;
		    
#define KEYWORD_RETURN(x)   col += yyleng; \
			    if (!comment) return (x);

#define VAL_RETURN(x)   col += yyleng; \
			if (!comment) return (x);
			
#define STRING_RETURN() VAL_RETURN(STR);

#define FLOAT_RETURN()  yylval.val = atof(yytext); \
                        VAL_RETURN(FLOAT);

#define CHAR_RETURN()   VAL_RETURN(yytext[0]);

%}

%option nounput
  
%x SC_COMMENT

%%

public                  yylval=strdup(yytext); KEYWORD_RETURN(PUBLIC_TOK);
private                 yylval=strdup(yytext); KEYWORD_RETURN(PRIVATE_TOK);
protected               yylval=strdup(yytext); KEYWORD_RETURN(PROTECTED_TOK);

class                   KEYWORD_RETURN(CLASS_TOK);
\:                      KEYWORD_RETURN(COLON);
[0-9]+                  yylval=strdup(yytext); KEYWORD_RETURN(NUMBER);
[a-zA-Z][a-zA-Z0-9_-]*	yylval=strdup(yytext); KEYWORD_RETURN(WORD);

(\"[^\"]*\")*       {{
                        char *tmp = NULL;
                        int i = 0;

                        yylval = (char *)calloc(yyleng - 1, sizeof(char));

                        for(i = 0; i < yyleng; i++) {
                            if (yytext[i] == '"') {
                                if (tmp != NULL) {
                                    yytext[i] = '\0';
                                    strcat(yylval, tmp);
                                    tmp = NULL;
                                }
                                continue;
                            }

                            if (tmp == NULL)
                                tmp = yytext + i;
                        }
                        if (tmp != NULL)
                            strcat(yylval, tmp);

			    STRING_RETURN();
                    }}                           
[a-zA-Z0-9\/.-]+        yylval=strdup(yytext); KEYWORD_RETURN(FILENAME);
\"                      KEYWORD_RETURN(QUOTE);
\{                      KEYWORD_RETURN(O_BRACE);
\}                      KEYWORD_RETURN(C_BRACE);
\(                      KEYWORD_RETURN(O_PAREN);
\)                      KEYWORD_RETURN(C_PAREN);
;                       KEYWORD_RETURN(SEMICOLON);
\n                      NEW_LINE();
[ \t]+                  WHITE_SPACE();
\/\/                            { CPP_COMMENT(); }
#                   { CPP_COMMENT(); }
"/*"                { COMMENT_START(); }
<SC_COMMENT>{
  "/*"              { COMMENT_START(); }
  \n                { NEW_LINE(); }

  "*/"              { COMMENT_END(); }
  . 
}

#[ \t]+[0-9]+[ \t]+\".*\".* {
    /* this handles the #line <num> <name> <something>
	 * so that the lnum and cur_file hold correct 
	 * values for this file
	 */              

	int c;          
	char *txt = yytext;
	char *lpos;     
	int count = 1;
	char *tmp;
	
	c = *txt;
	while(c == '#' || c == ' ') 
		c = *++txt; 

	lpos = txt;
	while(c >= 48 && c <= 57) {
		c = *++txt;
		count ++;
	}       
	
	tmp = (char *)calloc(count + 1, sizeof(char));
	strncpy(tmp, lpos, count);

	/* subtract 1 becase gcc returns the no of the *next* line
	 * and we will add one when we hit the \n anyway */
	lnum = atoi(tmp) - 1;
	free(tmp);

	while(c == ' ' || c == '\"') 
		c = *++txt;   

	lpos = txt;
	count = 1;
	while(c != '\"') {
		c = *++txt; 
		count ++;
	}
	count --;

	if (cur_file)
		free(cur_file);

	cur_file = (char *)calloc(count + 1, sizeof(char));
	strncpy(cur_file, lpos, count);

	if (!strcmp(cur_file, "<stdin>")) {
		free(cur_file);
		cur_file = strdup(filename);
	}
}


%%
