AUTOMAKE_OPTIONS       = 1.7 foreign

MAINTAINERCLEANFILES = Makefile.in

INCLUDES             = -I. -I$(top_srcdir) -I$(top_srcdir)/src/include

lib_LTLIBRARIES = libeirc.la

eirc_headers = Eirc.h

libeirc_la_INCLUDES = \
libircclient.h \
libirc_dcc.h \
libirc_doc_faq.h \
libirc_doc.h \
libirc_errors.h \
libirc_events.h \
libirc_options.h \
libirc_params.h \
libirc_portable.h \
libirc_rfcnumeric.h \
libirc_session.h \
libirc_sockets.h \
libirc_utils.h \
$(eirc_headers)

libeirc_la_SOURCES = \
utils.c \
dcc.c \
errors.c \
portable.c \
sockets.c \
colors.c \
libircclient.c \
eirc.c \
$(libeirc_la_INCLUDES)

installed_headersdir = $(prefix)/include
installed_headers_DATA = $(eirc_headers)

libeirc_la_CFLAGS = @EIRC_CFLAGS@
libeirc_la_LIBADD = $(INTLLIBS) @EIRC_LIBS@
