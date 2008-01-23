MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir) \
-I$(top_srcdir)/src/lib \
-I$(top_srcdir)/src/lib/util \
-DPACKAGE_DATA_DIR=\"$(datadir)/$(PACKAGE)\" \
@EINA_CFLAGS@ \
@PNG_CFLAGS@

if HAVE_PNG

bin_PROGRAMS = enesim_test

enesim_test_DEPENDENCIES = $(top_builddir)/src/lib/libenesim.la

enesim_test_SOURCES = \
png.c \
surface.c \
enesim_test.c

enesim_test_LDADD = \
$(top_builddir)/src/lib/libenesim.la \
@EINA_LIBS@ \
@PNG_LIBS@

endif
