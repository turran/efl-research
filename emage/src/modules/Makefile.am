MAINTAINERCLEANFILES = \
Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/include \
@EMAGE_CFLAGS@ \
@PNG_CFLAGS@

#providerdir = $(libdir)/emage/
#provider_LTLIBRARIES = emage_png.la

#emage_png_la_SOURCES = \
#emage_png.c

#emage_png_la_LIBADD       = $(top_builddir)/src/lib/libemage.la @EMAGE_LIBS@ @PNG_LIBS@
#emage_png_la_LDFLAGS      = -no-undefined -module -avoid-version
#emage_png_la_DEPENDENCIES = $(top_builddir)/src/lib/libemage.la


noinst_LTLIBRARIES = libemage_providers.la

libemage_providers_la_SOURCES = \
emage_png.c
