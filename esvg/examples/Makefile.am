MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST =

INCLUDES = \
-I. \
-I$(top_srcdir)/src/lib \
@EINA_CFLAGS@ \
@ENESIM_CFLAGS@

bin_PROGRAMS = esvg_test

esvg_test_DEPENDENCIES = $(top_builddir)/src/lib/libesvg.la

esvg_test_SOURCES = \
esvg_test.c

esvg_test_LDADD = \
$(top_builddir)/src/lib/libesvg.la \
@EINA_LIBS@ \
@EKEKO_LIBS@ \
@ENESIM_LIBS@ \
@EVG_CFLAGS@