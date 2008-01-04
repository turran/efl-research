MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
-I.

lib_LTLIBRARIES = libeina.la
include_HEADERS = \
Eina.h

libeina_la_SOURCES = \
eina_array.c \
eina_hash.c \
eina_list.c \
eina_sheap.c \
eina_tree.c \
eina_value.c

libeina_la_LIBADD = -lm @dlopen_libs@ @create_shared_lib@
libeina_la_LDFLAGS = -version-info 1:0:0
