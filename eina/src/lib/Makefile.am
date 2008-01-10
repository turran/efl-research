MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
-I.

EINAHEADERS = \
Eina.h \
eina_hash.h \
eina_lalloc.h \
eina_inlist.h \
eina_list.h \
eina_sheap.h \
eina_tree.h \
eina_types.h

installed_headersdir = $(prefix)/include/eina
installed_headers_DATA = $(EINAHEADERS)


lib_LTLIBRARIES = libeina.la

libeina_la_SOURCES = \
eina_hash.c \
eina_lalloc.c \
eina_inlist.c \
eina_list.c \
eina_sheap.c \
eina_tree.c \
eina_value.c

libeina_la_LIBADD = -lm @dlopen_libs@ @create_shared_lib@
libeina_la_LDFLAGS = -version-info 1:0:0
