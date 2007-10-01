MAINTAINERCLEANFILES = \
Makefile.in

INCLUDES = -I. \
-I$(top_srcdir)/src/lib \
-I$(top_srcdir)/src/lib/component \
@EDATA_CFLAGS@

noinst_LTLIBRARIES = libenginy_component.la

libenginy_component_la_SOURCES = \
component.c \
reader.c \
source_csv.c \
transform.c

libenginy_component_la_LIBADD = \
 -lm \
@EDATA_LIBS@

libenginy_component_la_DEPENDENCIES = 

libenginy_component_la_LDFLAGS      = -version-info 1:0:0
