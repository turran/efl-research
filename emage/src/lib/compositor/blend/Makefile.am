AUTOMAKE_OPTIONS     = 1.4 foreign

MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
-I. \
-I$(top_srcdir)/src/lib 

noinst_LTLIBRARIES      = libemage_compositor_blend.la

libemage_compositor_blend_la_SOURCES  = \
main.c \
argb8888_c.c \
rgb565_a5_c.c

libemage_compositor_blend_la_DEPENDENCIES = \
$(top_builddir)/src/lib/config.h
