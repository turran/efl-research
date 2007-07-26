SUBDIRS = blend

AUTOMAKE_OPTIONS     = 1.4 foreign

MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
-I. \
-I$(top_srcdir)/src/lib 

noinst_LTLIBRARIES      = libemage_compositor.la

libemage_compositor_la_SOURCES  = 

libemage_compositor_la_DEPENDENCIES = \
$(top_builddir)/src/lib/config.h \
blend/libemage_compositor_blend.la

libemage_compositor_la_LIBADD = \
blend/libemage_compositor_blend.la
