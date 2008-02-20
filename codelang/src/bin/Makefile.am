AUTOMAKE_OPTIONS	= 1.5 foreign
MAINTAINERCLEANFILES 	= Makefile.in

AM_CFLAGS	= -W -Wall -Wno-implicit
INCLUDES	= -I$(top_srcdir)/src/lib 

bin_PROGRAMS	= codelang

codelang_DEPENDENCIES     = $(top_builddir)/src/lib/libcodelang.la
codelang_SOURCES 	     = codelang.c 
codelang_LDADD	     = $(top_builddir)/src/lib/libcodelang.la 
