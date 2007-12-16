MAINTAINERCLEANFILES = \
Makefile.in

INCLUDES = -I. \
-I$(top_srcdir)/src/lib \
@ENESIM_CFLAGS@ \
@EDATA_CFLAGS@

lib_LTLIBRARIES = libopenvg.la
include_HEADERS = openvg.h

libopenvg_la_SOURCES = \
path.c \
context.c

libopenvg_la_LIBADD = \
@ENESIM_LIBS@ \
@EDATA_LIBS@ 

libopenvg_la_DEPENDENCIES = 

libopenvg_la_LDFLAGS      = -version-info 1:0:0
