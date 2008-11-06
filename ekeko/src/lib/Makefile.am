MAINTAINERCLEANFILES = Makefile.in

SUBDIRS = tiler

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/include \
@EKEKO_CFLAGS@

lib_LTLIBRARIES = libekeko.la

libekeko_la_SOURCES = \
ekeko_attribute.c \
ekeko_element.c \
ekeko_canvas.c \
ekeko_document.c \
ekeko_event.c \
ekeko_input.c \
ekeko_main.c \
ekeko_renderable.c \
ekeko_tiler.c


libekeko_la_LIBADD = \
tiler/libekeko_tiler.la \
@EKEKO_LIBS@

libekeko_la_DEPENDENCIES = \
tiler/libekeko_tiler.la

libekeko_la_LDFLAGS = @create_shared_lib@ -version-info @version_info@
