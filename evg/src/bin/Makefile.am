MAINTAINERCLEANFILES = Makefile.in

INCLUDES = -Wall -g \
-I$(top_srcdir)/src/lib \
@ENESIM_CFLAGS@ \
@EDATA_CFLAGS@

bin_PROGRAMS = vgTest1

vgTest1_SOURCES = \
vgTest1.c

vgTest1_LDADD = \
$(top_builddir)/src/lib/libopenvg.la \
@ENESIM_LIBS@ \
@EDATA_LIBS@

vgTest1_DEPENDENCIES = \
$(top_builddir)/src/lib/libopenvg.la
