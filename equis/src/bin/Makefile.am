MAINTAINERCLEANFILES = Makefile.in

INCLUDES = -Wall -g -msse \
-I$(top_srcdir)/src/lib \
-I$(top_srcdir)/src/lib/cpu

bin_PROGRAMS = equis_test

equis_test_SOURCES = \
main.c

equis_test_LDADD = \
$(top_builddir)/src/lib/libequis.la \
-lpng12

equis_test_DEPENDENCIES = \
$(top_builddir)/src/lib/libequis.la


