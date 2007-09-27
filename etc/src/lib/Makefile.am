SUBDIRS = rasterizer scanline

MAINTAINERCLEANFILES = \
Makefile.in

INCLUDES = -I. \
-I$(top_srcdir)/src/lib 

lib_LTLIBRARIES = libetc.la
include_HEADERS = \
Etc.h

libetc_la_SOURCES =

libetc_la_LIBADD = \
rasterizer/libetc_rasterizer.la \
scanline/libetc_scanline.la \
 -lm

libetc_la_DEPENDENCIES = \
rasterizer/libetc_rasterizer.la \
scanline/libetc_scanline.la 

libetc_la_LDFLAGS      = -version-info 1:0:0
