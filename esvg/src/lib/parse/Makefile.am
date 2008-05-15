MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
-I. \
-I$(top_srcdir)/src/include \
@ESVG_CFLAGS@


noinst_LTLIBRARIES = libesvg_parse.la

# make all the sources but main.c to be optional
libesvg_parse_la_SOURCES = \
svg.c \
group.c \
main.c \
polygon.c \
rect.c \
shape.c 
 

libesvg_parse_la_LIBADD = \
@ESVG_LIBS@

libesvg_parse_la_DEPENDENCIES =