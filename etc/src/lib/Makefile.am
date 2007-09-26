SUBDIRS = rasterizer

MAINTAINERCLEANFILES = \
Makefile.in

INCLUDES = -I. \
-I$(top_srcdir)/src/lib 

lib_LTLIBRARIES = libetc.la
include_HEADERS = \
Etc.h

libetc_la_SOURCES = \
scanline.c

libetc_la_LIBADD = \
rasterizer/libetc_rasterizer.la \
 -lm

libetc_la_DEPENDENCIES = \
rasterizer/libetc_rasterizer.la

libetc_la_LDFLAGS      = -version-info 1:0:0
