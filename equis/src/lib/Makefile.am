SUBDIRS = \
data \
generator

MAINTAINERCLEANFILES = \
Makefile.in

INCLUDES = -I. \
-I$(top_srcdir)/src/lib

lib_LTLIBRARIES = libequis.la

libequis_la_SOURCES = 

libequis_la_LIBADD = \
 -lm \
data/libequis_data.la \
generator/libequis_generator.la

libequis_la_DEPENDENCIES = \
data/libequis_data.la \
generator/libequis_generator.la

libequis_la_LDFLAGS      = -version-info 1:0:0
