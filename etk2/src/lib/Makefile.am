MAINTAINERCLEANFILES = Makefile.in

AUTOMAKE_OPTIONS = 1.4 foreign

INCLUDES	= \
-I. -I../.. -I$(top_srcdir)/src/lib -Wall -g \
@EINA_CFLAGS@ @EVAS_CFLAGS@ @ECORE_CFLAGS@ @EDJE_CFLAGS@

lib_LTLIBRARIES = libetk2.la

EXTRA_DIST = 

ETK2HEADERS = \
Etk2.h \
etk2_types.h \
etk2_private.h \
etk2_button.h \
etk2_object.h \
etk2_type.h \
etk2_widget.h 

libetk2_la_SOURCES = \
etk2_button.c \
etk2_object.c \
etk2_type.c \
etk2_widget.c \
$(ETK2HEADERS)

installed_headersdir = $(prefix)/include/etk2
installed_headers_DATA = $(ETK2HEADERS)

libetk2_la_LIBADD  = @EINA_LIBS@ @EVAS_LIBS@ @ECORE_LIBS@ @EDJE_LIBS@

libetk2_la_DEPENDENCIES = 

libetk2_la_LDFLAGS = -version-info 1:0:0

