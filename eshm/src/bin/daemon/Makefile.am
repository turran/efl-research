## Process this file with automake to produce Makefile.in

INCLUDES = -Wall -g \
-I$(top_srcdir) \
-I$(top_srcdir)/src/lib \
@ESHM_CFLAGS@ 


bin_PROGRAMS = eshmd

eshmd_SOURCES = \
eshmd.c

eshmd_DEPENDENCIES = $(top_builddir)/src/lib/libeshm.la

eshmd_LDADD = \
$(top_builddir)/src/lib/libeshm.la \
@ESHM_LIBS@

