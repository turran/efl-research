MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/include \
-DPACKAGE_DATA_DIR=\"$(datadir)/$(PACKAGE)\" \
@ENESIM_CFLAGS@ \
@SDL_CFLAGS@ \
@PNG_CFLAGS@

if HAVE_SDL

bin_PROGRAMS = enesim_test

enesim_test_DEPENDENCIES = $(top_builddir)/src/lib/libenesim.la

enesim_test_SOURCES = \
surface.c \
png.c \
enesim_test.c

enesim_test_LDADD = \
$(top_builddir)/src/lib/libenesim.la \
@ENESIM_LIBS@ \
@SDL_LIBS@ \
@PNG_LIBS@

endif
