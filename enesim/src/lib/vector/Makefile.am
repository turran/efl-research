SUBDIRS = component 

MAINTAINERCLEANFILES = \
Makefile.in

INCLUDES = -I. \
-I$(top_srcdir)/src/lib \
-I$(top_srcdir)/src/lib/cpu \
@EDATA_CFLAGS@

noinst_LTLIBRARIES = libenesim_vector.la
include_HEADERS = \
Enginy.h

libenesim_vector_la_SOURCES = \
path.c

libenesim_vector_la_LIBADD = \
component/libenesim_component.la

libenesim_vector_la_DEPENDENCIES = \
component/libenesim_component.la

libenesim_vector_la_LDFLAGS      = -version-info 1:0:0
