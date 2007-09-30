MAINTAINERCLEANFILES = \
Makefile.in

INCLUDES = -I. \
-I$(top_srcdir)/src/lib \
-I$(top_srcdir)/src/lib/scanline

noinst_LTLIBRARIES = libetc_rasterizer.la

libetc_rasterizer_la_SOURCES = \
rasterizer.c \
faap.c \
cpsc.c 

libetc_rasterizer_la_LIBADD = \
 -lm

libetc_rasterizer_la_DEPENDENCIES =

libetc_rasterizer_la_LDFLAGS      = -version-info 1:0:0
