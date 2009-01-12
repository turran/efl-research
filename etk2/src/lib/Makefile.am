MAINTAINERCLEANFILES = Makefile.in

AUTOMAKE_OPTIONS = 1.4 foreign

INCLUDES	= \
-I. -I../.. -I$(top_srcdir)/src/include -Wall -g \
@EINA_CFLAGS@ @EVAS_CFLAGS@ @ECORE_CFLAGS@ @EDJE_CFLAGS@

lib_LTLIBRARIES = libetk2.la

EXTRA_DIST = 

libetk2_la_SOURCES = \
etk2_value.c \
etk2_main.c \
etk2_object.c \
etk2_property.c \
etk2_event.c \
etk2_type.c \
etk2_renderable.c \
etk2_canvas.c \
etk2_button.c \
etk2_widget.c \
etk2_label.c

libetk2_la_LIBADD  = @EINA_LIBS@ @EVAS_LIBS@ @ECORE_LIBS@ @EDJE_LIBS@

libetk2_la_DEPENDENCIES = 

libetk2_la_LDFLAGS = -version-info 1:0:0

