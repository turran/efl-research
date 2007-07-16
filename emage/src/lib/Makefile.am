SUBDIRS = object 

MAINTAINERCLEANFILES = \
Makefile.in \
config.h.in \
config.h \
stamp-h1

INCLUDES = -I. \
-I$(top_srcdir)/src/lib

lib_LTLIBRARIES = libemage.la

libemage_la_SOURCES = \
emage_blend.c \
emage_blit.c \
emage_context.c \
emage_cpu.c

libemage_la_LIBADD = \
object/libemage_object.la

libemage_la_DEPENDENCIES = \
$(top_builddir)/src/lib/config.h \
object/libemage_object.la

libemage_la_LDFLAGS      = -version-info 1:0:0
