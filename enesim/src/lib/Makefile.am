SUBDIRS = rasterizer scanline renderer surface

MAINTAINERCLEANFILES = \
Makefile.in

INCLUDES = -I. \
-I$(top_srcdir)/src/lib 

lib_LTLIBRARIES = libenesim.la
include_HEADERS = \
Enesim.h

libenesim_la_SOURCES =

libenesim_la_LIBADD = \
rasterizer/libenesim_rasterizer.la \
scanline/libenesim_scanline.la \
@EDATA_LIBS@ \
 -lm

libenesim_la_DEPENDENCIES = \
rasterizer/libenesim_rasterizer.la \
scanline/libenesim_scanline.la 

libenesim_la_LDFLAGS      = -version-info 1:0:0
