MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/lib/util

noinst_LTLIBRARIES = libenesim_util.la

libenesim_util_la_SOURCES = \
curve_decasteljau.c

ENESIM_UTILHEADERS = \
enesim_rectangle.h \
curve.h \
extender_int.h \
extender_float.h \
fixed_16p16.h \
fixed_24p8.h \
fixed_math.h

installed_headersdir = $(prefix)/include/enesim
installed_headers_DATA = $(ENESIM_UTILHEADERS)
