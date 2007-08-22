AUTOMAKE_OPTIONS     = 1.4 foreign

MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
-I. \
-I$(top_srcdir)/src/lib 

noinst_LTLIBRARIES      = libemage_converter.la

libemage_converter_la_SOURCES  = \
gray_1.c \
gray_4.c \
gray_8.c \
rgb_8.c \
rgb_16.c \
rgb_24.c \
rgb_32.c

libemage_converter_la_DEPENDENCIES = \
$(top_builddir)/src/lib/config.h
