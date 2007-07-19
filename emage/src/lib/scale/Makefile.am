AUTOMAKE_OPTIONS     = 1.4 foreign

MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
-I. \
-I$(top_srcdir)/src/lib 

noinst_LTLIBRARIES      = libemage_scale.la

libemage_scale_la_SOURCES  = \
emage_scale_sample.c \
emage_scale_smooth.c \
emage_scale_span.c

libemage_scale_la_DEPENDENCIES = \
$(top_builddir)/src/lib/config.h
