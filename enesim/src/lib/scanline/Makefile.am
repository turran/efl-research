MAINTAINERCLEANFILES = \
Makefile.in

INCLUDES = -I. \
-I$(top_srcdir)/src/lib \
@EDATA_CFLAGS@

noinst_LTLIBRARIES = libetc_scanline.la

libetc_scanline_la_SOURCES = \
scanline.c \
naa.c

libetc_scanline_la_LIBADD = \
 -lm

libetc_scanline_la_DEPENDENCIES =

libetc_scanline_la_LDFLAGS      = -version-info 1:0:0
