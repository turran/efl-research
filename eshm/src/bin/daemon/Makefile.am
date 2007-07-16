## Process this file with automake to produce Makefile.in

INCLUDES = -Wall -g \
-I$(top_srcdir) \
-I$(top_srcdir)/src/lib \
@ESHM_CFLAGS@ 


bin_PROGRAMS = eshmd

eshmd_SOURCES = \
eshmd.c

eshmd_LDADD = \
@ESHM_LIBS@

