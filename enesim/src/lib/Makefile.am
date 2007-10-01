SUBDIRS = render vector

MAINTAINERCLEANFILES = \
Makefile.in

INCLUDES = -I. \
-I$(top_srcdir)/src/lib 

lib_LTLIBRARIES = libenesim.la
include_HEADERS = \
Enesim.h

libenesim_la_SOURCES =

libenesim_la_LIBADD = \
rasterizer/libenesim_render.la \
scanline/libenesim_vector.la \
@EDATA_LIBS@ \
 -lm

libenesim_la_DEPENDENCIES = \
rasterizer/libenesim_render.la \
scanline/libenesim_vector.la 

libenesim_la_LDFLAGS      = -version-info 1:0:0
