SUBDIRS = component 

MAINTAINERCLEANFILES = \
Makefile.in

INCLUDES = -I. \
-I$(top_srcdir)/src/lib \
-I$(top_srcdir)/src/lib/cpu \
-msse
##@EIX_CFLAGS@

lib_LTLIBRARIES = libequis.la

libequis_la_SOURCES = \
path.c

libequis_la_LIBADD = \
 -lm \
component/libequis_component.la \
@EDATA_LIBS@

libequis_la_DEPENDENCIES = \
component/libequis_component.la

libequis_la_LDFLAGS      = -version-info 1:0:0
