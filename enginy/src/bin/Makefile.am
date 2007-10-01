MAINTAINERCLEANFILES = Makefile.in

INCLUDES = -Wall -g -msse \
-I$(top_srcdir)/src/lib \
-I$(top_srcdir)/src/lib/cpu

bin_PROGRAMS = enginy_test

enginy_test_SOURCES = \
main.c

enginy_test_LDADD = \
$(top_builddir)/src/lib/libenginy.la \
-lpng12

enginy_test_DEPENDENCIES = \
$(top_builddir)/src/lib/libenginy.la


