SUBDIRS = object compositor data 
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
surface.c \
context.c \
compositor.c \
main.c \
cpu.c
#emage_blend.c
#emage_blit.c

libemage_la_LIBADD = \
 -lm \
object/libemage_object.la \
data/libemage_data.la \
compositor/libemage_compositor.la
#op/libemage_op.la
#scale/libemage_scale.la
#convert/libemage_convert.la

libemage_la_DEPENDENCIES = \
$(top_builddir)/src/lib/config.h \
object/libemage_object.la \
data/libemage_data.la \
compositor/libemage_compositor.la
# op/libemage_op.la
# convert/libemage_convert.la
# scale/libemage_scale.la

libemage_la_LDFLAGS      = -version-info 1:0:0
