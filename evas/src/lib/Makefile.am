MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I$(top_srcdir)/src/include \
-I. \
@EVAS_CFLAGS@

lib_LTLIBRARIES = libevas.la

libevas_la_SOURCES = \
evas_main.c

libevas_la_LIBADD = \
@EVAS_LIBS@

libevas_la_DEPENDENCIES = 

libevas_la_LDFLAGS = @create_shared_lib@ -version-info @version_info@
