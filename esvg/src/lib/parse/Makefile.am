MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
-I. \
-I$(top_srcdir)/src/include \
@ESVG_CFLAGS@


noinst_LTLIBRARIES = libesvg_parse.la

# make all the sources but main.c to be optional
libesvg_parse_la_SOURCES = \
main.c \
group.c \
document.c \
rect.c \
polygon.c 

libesvg_parse_la_LIBADD = \
@ESVG_LIBS@

libesvg_parse_la_DEPENDENCIES =