MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I.

lib_LTLIBRARIES = libetch.la

include_HEADERS = Etch.h 

libetch_la_SOURCES = \
etch.c \
etch_animation.c \
etch_object.c

libetch_la_LIBADD = -lm

libetch_la_DEPENDENCIES =

libetch_la_LDFLAGS = @create_shared_lib@ -version-info @version_info@
