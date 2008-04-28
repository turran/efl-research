MAINTAINERCLEANFILES = Makefile.in

SUBDIRS = tiler

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/include \
@EINA_CFLAGS@ \
@ENESIM_CFLAGS@

lib_LTLIBRARIES = libekeko.la

libekeko_la_SOURCES = \
ekeko_core.c \
ekeko_canvas.c \
ekeko_input.c \
ekeko_object.c \
ekeko_tiler.c


libekeko_la_LIBADD = \
tiler/libekeko_tiler.la \
@EINA_LIBS@ \
@ENESIM_LIBS@

libekeko_la_DEPENDENCIES = \
tiler/libekeko_tiler.la

libekeko_la_LDFLAGS = @create_shared_lib@ -version-info @version_info@
