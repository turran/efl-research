SUBDIRS = object compositor data scaler convert
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
cpu.c \
main.c \
scaler.c \
scanline.c \
surface.c
#emage_blend.c
#emage_blit.c

libemage_la_LIBADD = \
 -lm \
object/libemage_object.la \
data/libemage_data.la \
compositor/libemage_compositor.la \
scaler/libemage_scaler.la \
convert/libemage_convert.la

#op/libemage_op.la

libemage_la_DEPENDENCIES = \
$(top_builddir)/src/lib/config.h \
object/libemage_object.la \
data/libemage_data.la \
compositor/libemage_compositor.la \
scaler/libemage_scaler.la \
convert/libemage_convert.la
# op/libemage_op.la

libemage_la_LDFLAGS      = -version-info 1:0:0
