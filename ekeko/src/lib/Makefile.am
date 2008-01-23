MAINTAINERCLEANFILES = Makefile.in

SUBDIRS = tiler

AM_CPPFLAGS = \
-I. \
@EINA_CFLAGS@ \
@ENESIM_CFLAGS@

lib_LTLIBRARIES = libekeko.la

include_HEADERS = \
Ekeko.h

libekeko_la_SOURCES = \
ekeko_canvas.c \
ekeko_tiler.c \
ekeko_object.c

libekeko_la_LIBADD = \
tiler/libekeko_tiler.la \
@EINA_LIBS@ \
@ENESIM_LIBS@

libekeko_la_DEPENDENCIES = \
tiler/libekeko_tiler.la

libekeko_la_LDFLAGS = @create_shared_lib@ -version-info @version_info@
