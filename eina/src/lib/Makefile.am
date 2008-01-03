MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
-I. \
@EDATA_CFLAGS@

lib_LTLIBRARIES = libeina.la
include_HEADERS = \
Ecore_Li.h

libeina_la_SOURCES = \
eina.c \
device.c

libeina_la_LIBADD = \
@EDATA_LIBS@

libeina_la_LDFLAGS = -version-info 1:0:0
