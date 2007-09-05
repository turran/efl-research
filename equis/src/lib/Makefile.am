SUBDIRS = \
generator

MAINTAINERCLEANFILES = \
Makefile.in

INCLUDES = -I. \
-I$(top_srcdir)/src/lib \
@EIX_CFLAGS@

lib_LTLIBRARIES = libequis.la

libequis_la_SOURCES = 

libequis_la_LIBADD = \
 -lm \
generator/libequis_generator.la

libequis_la_DEPENDENCIES = \
generator/libequis_generator.la

libequis_la_LDFLAGS      = -version-info 1:0:0
