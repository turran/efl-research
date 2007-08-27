AUTOMAKE_OPTIONS     = 1.4 foreign

MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
-I. \
-I$(top_srcdir)/src/lib

noinst_LTLIBRARIES      = libemage_geometry_object.la

libemage_geometry_object_la_SOURCES  = \
rectangle.c \
polygon.c 
#line.c 

libemage_geometry_object_la_DEPENDENCIES = \
$(top_builddir)/src/lib/config.h
