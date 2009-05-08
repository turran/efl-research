MAINTAINERCLEANFILES = Makefile.in

AUTOMAKE_OPTIONS = 1.4 foreign

EXTRA_DIST =

INCLUDES = \
-I. -I../.. -I$(top_srcdir)/src/include -Wall -g \
@ETK_CFLAGS@

bin_PROGRAMS = etk2_test
if HAVE_EXML
bin_PROGRAMS += etk2_testsuite
endif

etk2_test_DEPENDENCIES = $(top_builddir)/src/lib/libetk2.la

etk2_test_SOURCES = \
etk2_test.h \
etk2_test.c 

etk2_test_LDADD = $(top_builddir)/src/lib/libetk2.la \
@ETK_LIBS@

if BUILD_ENGINE_SDL
etk2_test_LDADD += @ECORE_SDL_LIBS@ @SDL_LIBS@ 
endif

## Testsuite
etk2_testsuite_DEPENDENCIES = $(top_builddir)/src/lib/libetk2.la
 
etk2_testsuite_SOURCES = \
etk2_testsuite.c 

etk2_testsuite_LDADD = $(top_builddir)/src/lib/libetk2.la \
@ETK_LIBS@

if BUILD_ENGINE_SDL
etk2_testsuite_LDADD += @ECORE_SDL_LIBS@ @SDL_LIBS@ @EXML_LIBS@
endif
