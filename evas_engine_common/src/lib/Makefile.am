SUBDIRS = compositor converter data geometry scaler
#scale convert op

MAINTAINERCLEANFILES = \
Makefile.in \
config.h.in \
config.h \
stamp-h1

INCLUDES = -I. \
-I$(top_srcdir)/src/lib

lib_LTLIBRARIES = libemage.la

libemage_la_SOURCES = \
color.c \
compositor.c \
context.c \
converter.c \
cpu.c \
main.c \
scaler.c \
surface.c
#emage_blend.c
#emage_blit.c

libemage_la_LIBADD = \
 -lm \
compositor/libemage_compositor.la \
converter/libemage_converter.la \
data/libemage_data.la \
geometry/libemage_geometry.la \
scaler/libemage_scaler.la

libemage_la_DEPENDENCIES = \
$(top_builddir)/src/lib/config.h \
compositor/libemage_compositor.la \
converter/libemage_converter.la \
data/libemage_data.la \
geometry/libemage_geometry.la \
scaler/libemage_scaler.la

libemage_la_LDFLAGS      = -version-info 1:0:0
