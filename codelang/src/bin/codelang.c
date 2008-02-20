#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <locale.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "Codelang.h"

static void main_help(void);

char      *file_in = NULL;
char      *progname = NULL;
int        verbose = 0;

static void
main_help(void)
{
   printf
     ("Usage:\n"
      "\t%s <input_file>\n"
      "\n"
      ,progname);
}

int
main(int argc, char **argv)
{
	int i;
	struct stat st;
#ifdef HAVE_REALPATH   
	char rpath[PATH_MAX], rpath2[PATH_MAX];
#endif   

	setlocale(LC_NUMERIC, "C");

	progname = argv[0];
	for (i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "-h"))
		{
			main_help();
			exit(0);
	  }
		else if (!strcmp(argv[i], "-v"))
	  {
	     verbose = 1;
	  }
		else if (!file_in)
	  	file_in = argv[i];

		if (!file_in)
		{
			fprintf(stderr, "%s: Error: no input file specified.\n", progname);
			main_help();
			exit(-1);
     }
  } 
	/* check whether file_in exists */
#ifdef HAVE_REALPATH
	if (!realpath(file_in, rpath) || stat(rpath, &st) || !S_ISREG(st.st_mode))
#else
	if (stat(file_in, &st) || !S_ISREG(st.st_mode))
#endif
  {
		fprintf(stderr, "%s: Error: file not found: %s.\n", progname, file_in);
		main_help();
		exit(-1);
  }

	file_load(file_in);
   
	return 0;  
}

