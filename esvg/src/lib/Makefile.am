SUBDIRS = draw

lib_LTLIBRARIES = libesvg.la
include_HEADERS = ESVG.h

INCLUDES = \
-I. \
-I$(top_srcdir)/src/lib \
@EINA_CFLAGS@ \
@EKEKO_CFLAGS@ \
@ENESIM_CFLAGS@ \
@EVG_CFLAGS@

libesvg_la_SOURCES =

libesvg_la_LIBADD = \
draw/libesvg_draw.la \
@EINA_LIBS@ \
@EKEKO_LIBS@ \
@ENESIM_LIBS@ \
@EVG_LIBS@

libesvg_la_DEPENDENCIES = \
draw/libesvg_draw.la

libesvg_la_LDFLAGS      = -version-info 1:0:0