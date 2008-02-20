#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include "Codelang.h"

char *filename = NULL;
extern FILE *yyin;
int yyparse(void);

void file_load(char *file)
{
	if (!file) return;

	yyin = fopen(file, "r");
	if (!yyin)
	{
		fprintf(stderr, "Problem loading file: %s\n", file);
		return;
	}

	filename = strdup(file);
	yyparse();
	fclose(yyin);
}

