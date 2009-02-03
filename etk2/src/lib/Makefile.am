MAINTAINERCLEANFILES = Makefile.in

AUTOMAKE_OPTIONS = 1.4 foreign

INCLUDES	= \
-I. -I../.. -I$(top_srcdir)/src/include -Wall -g \
@ETK_CFLAGS@

lib_LTLIBRARIES = libetk2.la

EXTRA_DIST = 

libetk2_la_SOURCES = \
etk2_value.c \
etk2_main.c \
etk2_document.c
#
#etk2_button.c
#etk2_widget.c
#etk2_label.c

libetk2_la_LIBADD  = @ETK_LIBS@

libetk2_la_DEPENDENCIES = 

libetk2_la_LDFLAGS = -version-info 1:0:0

