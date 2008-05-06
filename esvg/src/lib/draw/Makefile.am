MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
-I. \
-I$(top_srcdir)/src/include \
@ESVG_CFLAGS@


noinst_LTLIBRARIES = libesvg_draw.la

libesvg_draw_la_SOURCES = \
esvg_canvas.c \
esvg_engine.c \
esvg_engine_cairo.c \
esvg_rect.c \
esvg_shape.c \
esvg_units.c

libesvg_draw_la_LIBADD = \
@ESVG_LIBS@

libesvg_draw_la_DEPENDENCIES =
