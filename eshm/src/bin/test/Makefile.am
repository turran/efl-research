## Process this file with automake to produce Makefile.in

INCLUDES = -Wall -g \
-I$(top_srcdir) \
-I$(top_srcdir)/src/lib \
@ESHM_CFLAGS@ 

bin_PROGRAMS = eshm_writer eshm_reader eshm_client

eshm_writer_SOURCES = \
eshm_writer.c

eshm_writer_LDADD = \
@ESHM_LIBS@

eshm_reader_SOURCES = \
eshm_reader.c

eshm_reader_LDADD = \
@ESHM_LIBS@

eshm_client_SOURCES = \
eshm_client.c

eshm_client_LDADD = \
$(top_builddir)/src/lib/libeshm.la \
@ESHM_LIBS@

eshm_client_DEPENDENCIES = \
$(top_builddir)/src/lib/libeshm.la 

