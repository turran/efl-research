AUTOMAKE_OPTIONS     = 1.4 foreign

MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
-I. \
-I$(top_srcdir)/src/lib \
-Werror

noinst_LTLIBRARIES      = libemage_object.la

libemage_object_la_SOURCES  = \
line.c \
polygon.c \
rectangle.c 

libemage_object_la_DEPENDENCIES = \
$(top_builddir)/src/lib/config.h
