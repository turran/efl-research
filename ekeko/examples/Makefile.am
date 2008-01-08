MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST =

INCLUDES = \
-I. \
-I$(top_srcdir)/src/lib \
@EINA_CFLAGS@ \
@SDL_CFLAGS@

bin_PROGRAMS = ekeko_test

ekeko_test_DEPENDENCIES = $(top_builddir)/src/lib/libekeko.la

ekeko_test_SOURCES = \
ekeko_test.c

ekeko_test_LDADD = \
$(top_builddir)/src/lib/libekeko.la \
@EINA_LIBS@ \
@SDL_LIBS@

