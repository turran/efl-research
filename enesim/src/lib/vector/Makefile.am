SUBDIRS = component 

MAINTAINERCLEANFILES = \
Makefile.in

INCLUDES = -I. \
-I$(top_srcdir)/src/lib \
-I$(top_srcdir)/src/lib/cpu \
@EDATA_CFLAGS@

lib_LTLIBRARIES = libenginy.la
include_HEADERS = \
Enginy.h

libenginy_la_SOURCES = \
path.c

libenginy_la_LIBADD = \
 -lm \
component/libenginy_component.la \
@EDATA_LIBS@

libenginy_la_DEPENDENCIES = \
component/libenginy_component.la

libenginy_la_LDFLAGS      = -version-info 1:0:0
