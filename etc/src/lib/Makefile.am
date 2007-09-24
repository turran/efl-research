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
 -lm

libetc_la_DEPENDENCIES = 

libetc_la_LDFLAGS      = -version-info 1:0:0
