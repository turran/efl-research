MAINTAINERCLEANFILES = Makefile.in

AUTOMAKE_OPTIONS = 1.4 foreign

EXTRA_DIST =

INCLUDES = \
-I. -I../.. -I$(top_srcdir)/src/include -Wall -g \
@ETK_CFLAGS@

bin_PROGRAMS = eon_test
if HAVE_EXML
bin_PROGRAMS += eon_testsuite
endif

eon_test_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la

eon_test_SOURCES = \
eon_test.h \
eon_test.c

eon_test_LDADD = $(top_builddir)/src/lib/libeon.la \
@ETK_LIBS@

if BUILD_ENGINE_SDL
eon_test_LDADD += @ECORE_SDL_LIBS@ @SDL_LIBS@
endif

## Testsuite
eon_testsuite_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la

eon_testsuite_SOURCES = \
eon_testsuite.c

eon_testsuite_LDADD = $(top_builddir)/src/lib/libeon.la \
@ETK_LIBS@ \
-lm

if BUILD_ENGINE_SDL
eon_testsuite_LDADD += @ECORE_SDL_LIBS@ @SDL_LIBS@ @EXML_LIBS@
endif
