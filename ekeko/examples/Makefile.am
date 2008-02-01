MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/lib \
@EINA_CFLAGS@ \
@ENESIM_CFLAGS@ \
@SDL_CFLAGS@

if HAVE_SDL

bin_PROGRAMS = ekeko_test

ekeko_test_DEPENDENCIES = $(top_builddir)/src/lib/libekeko.la

ekeko_test_SOURCES = \
ekeko_test.c \
subcanvas.c \
canvas.c \
filter.c \
object.c \
rectangle.c

ekeko_test_LDADD = \
$(top_builddir)/src/lib/libekeko.la \
@EINA_LIBS@ \
@ENESIM_LIBS@ \
@SDL_LIBS@

endif
