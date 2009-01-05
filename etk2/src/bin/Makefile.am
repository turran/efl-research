MAINTAINERCLEANFILES = Makefile.in

AUTOMAKE_OPTIONS = 1.4 foreign

EXTRA_DIST =

INCLUDES	= \
-I. -I../.. -I$(top_srcdir)/src/include -Wall -g \
@EINA_CFLAGS@ @EVAS_CFLAGS@ @ECORE_CFLAGS@ @EDJE_CFLAGS@

bin_PROGRAMS = etk2_test

etk2_test_DEPENDENCIES = $(top_builddir)/src/lib/libetk2.la

etk2_test_SOURCES = \
etk2_test.h \
etk2_test.c 

etk2_test_LDADD = $(top_builddir)/src/lib/libetk2.la \
		 @EINA_LIBS@ @EVAS_LIBS@ @ECORE_LIBS@ @EDJE_LIBS@
