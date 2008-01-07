MAINTAINERCLEANFILES = Makefile.in

SUBDIRS = tiler

INCLUDES = \
-I. \
@EINA_CFLAGS@

lib_LTLIBRARIES = libekeko.la
include_HEADERS = \
Ekeko.h

libekeko_la_SOURCES = \
ekeko_canvas.c \
ekeko_tiler.c \
ekeko_object.c

libekeko_la_LIBADD = \
@EINA_LIBS@

libekeko_la_LDFLAGS = -version-info 1:0:0
