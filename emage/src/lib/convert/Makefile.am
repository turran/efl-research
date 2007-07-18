AUTOMAKE_OPTIONS     = 1.4 foreign

MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
-I. \
-I$(top_srcdir)/src/lib 

noinst_LTLIBRARIES      = libemage_convert.la

libemage_convert_la_SOURCES  = \
emage_convert_color.c \
emage_convert_main.c \
emage_convert_rgb_8.c \
emage_convert_gry_1.c \
emage_convert_rgb_16.c \
emage_convert_yuv.c \
emage_convert_gry_4.c \
emage_convert_rgb_24.c \
emage_convert_gry_8.c \
emage_convert_rgb_32.c

libemage_convert_la_DEPENDENCIES = \
$(top_builddir)/src/lib/config.h
