SUBDIRS = object

INCLUDES = \
-I. \
-I$(top_srcdir)/src/lib 

noinst_LTLIBRARIES      = libemage_geometry.la

libemage_geometry_la_SOURCES = \
scanline.c

libemage_geometry_la_DEPENDENCIES = \
$(top_builddir)/src/lib/config.h \
object/libemage_geometry_object.la

libemage_geometry_la_LIBADD = \
object/libemage_geometry_object.la
