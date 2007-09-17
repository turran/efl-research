MAINTAINERCLEANFILES = \
Makefile.in

INCLUDES = -I. \
-I$(top_srcdir)/src/lib \
-I$(top_srcdir)/src/lib/component \
@EDATA_CFLAGS@

noinst_LTLIBRARIES = libequis_component.la

libequis_component_la_SOURCES = \
component.c \
reader.c \
source_csv.c \
transform.c

libequis_component_la_LIBADD = \
 -lm \
@EDATA_LIBS@

libequis_component_la_DEPENDENCIES = 

libequis_component_la_LDFLAGS      = -version-info 1:0:0
