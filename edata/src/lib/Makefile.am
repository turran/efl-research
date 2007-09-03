MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
-I.

lib_LTLIBRARIES = libedata.la
include_HEADERS = \
Edata.h

libedata_la_SOURCES = \
hash.c \
list.c \
sheap.c \
tree.c \
value.c \
private.h

libedata_la_LIBADD = -lm @dlopen_libs@ @create_shared_lib@
libedata_la_LDFLAGS = -version-info 1:0:0
