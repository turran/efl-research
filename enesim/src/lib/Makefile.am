SUBDIRS = converter drawer rasterizer renderer surface util

MAINTAINERCLEANFILES = \
Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/include \
-I$(top_srcdir)/src/lib/surface \
@ENESIM_CFLAGS@

lib_LTLIBRARIES = libenesim.la
include_HEADERS = Enesim.h

libenesim_la_SOURCES = \
enesim_converter.c \
enesim_drawer.c \
enesim_rasterizer.c \
enesim_renderer.c \
enesim_surface.c

libenesim_la_LIBADD = \
@ENESIM_LIBS@


libenesim_la_DEPENDENCIES = \
util/libenesim_util.la

libenesim_la_LDFLAGS = @create_shared_lib@ -version-info @version_info@
