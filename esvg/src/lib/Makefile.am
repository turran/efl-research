MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
-I. \
@EINA_CFLAGS@ \
@ENESIM_CFLAGS@ \
@EVG_CFLAGS@ \
@EXML_CFLAGS@

lib_LTLIBRARIES = libesvg.la
include_HEADERS = \
ESVG.h

libesvg_la_SOURCES =
#main.c \
#units.c \
#polygon.c \
#group.c \
#document.c
 
libesvg_la_LIBADD = @create_shared_lib@ \
@EINA_LIBS@ \
@ENESIM_LIBS@ \
@EVG_LIBS@ \
@EXML_LIBS@

libesvg_la_LDFLAGS = -version-info 1:0:0
