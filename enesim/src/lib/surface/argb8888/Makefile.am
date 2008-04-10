MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/include \
-I$(top_srcdir)/src/lib/surface \
@ENESIM_CFLAGS@

noinst_LTLIBRARIES = libenesim_surface_argb8888.la

libenesim_surface_argb8888_la_SOURCES = \
color_c.c \
pixel_c.c \
draw_blend.c \
draw_fill.c
