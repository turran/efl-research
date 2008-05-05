MAINTAINERCLEANFILES = Makefile.in

SUBDIRS = draw parse

lib_LTLIBRARIES = libesvg.la

INCLUDES = \
-I. \
-I$(top_srcdir)/src/include \
@ESVG_CFLAGS@

libesvg_la_SOURCES =

libesvg_la_LIBADD = \
draw/libesvg_draw.la \
parse/libesvg_parse.la \
@ESVG_LIBS@

libesvg_la_DEPENDENCIES = \
draw/libesvg_draw.la \
parse/libesvg_parse.la

libesvg_la_LDFLAGS      = -version-info 1:0:0