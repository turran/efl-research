## Process this file with automake to produce Makefile.in
MAINTAINERCLEANFILES = Makefile.in

INCLUDES             = -I. @ESHM_CFLAGS@
lib_LTLIBRARIES      = libeshm.la
include_HEADERS      = Eshm.h

libeshm_la_SOURCES  = \
eshm.c \
eshm_element.c \
eshm_segment.c \
eshm_manager.c \
eshm_private.h

libeshm_la_LIBADD       =
#libeshm_la_DEPENDENCIES = $(top_builddir)/config.h
libeshm_la_LDFLAGS      = -version-info 0:1:0
