AUTOMAKE_OPTIONS     = 1.4 foreign

MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
-I. \
-I$(top_srcdir)/src/lib 

noinst_LTLIBRARIES      = libemage_data.la

libemage_data_la_SOURCES  = \
emage_object_list.c

libemage_data_la_DEPENDENCIES = \
$(top_builddir)/src/lib/config.h
