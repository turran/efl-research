MAINTAINERCLEANFILES = \
Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/lib/ \
-I$(top_srcdir)/src/lib/util \
-I$(top_srcdir)/src/lib/raster \
-I$(top_srcdir)/src/lib/raster/scanline \
@EINA_CFLAGS@

noinst_LTLIBRARIES = libenesim_rasterizer.la

libenesim_rasterizer_la_SOURCES = \
rasterizer.c \
cpsc.c \
kiia.c \
kiia8.c \
kiia16.c

libenesim_rasterizer_la_LIBADD = -lm
