MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/lib \
@ENESIM_CFLAGS@ \
@EINA_CFLAGS@

lib_LTLIBRARIES = libopenvg.la

include_HEADERS = openvg.h

libopenvg_la_SOURCES = \
context.c \
filter.c \
image.c \
paint.c \
path.c

#matrix.c
libopenvg_la_LIBADD = \
@ENESIM_LIBS@ \
@EINA_LIBS@

libopenvg_la_LDFLAGS = @create_shared_lib@ -version-info @version_info@
