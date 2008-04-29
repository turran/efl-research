MAINTAINERCLEANFILES = Makefile.in

SUBDIRS = draw

lib_LTLIBRARIES = libesvg.la

INCLUDES = \
-I. \
-I$(top_srcdir)/src/lib \
@ESVG_CFLAGS@

libesvg_la_SOURCES =

libesvg_la_LIBADD = \
draw/libesvg_draw.la \
@ESVG_LIBS@

libesvg_la_DEPENDENCIES = \
draw/libesvg_draw.la

libesvg_la_LDFLAGS      = -version-info 1:0:0