MAINTAINERCLEANFILES = Makefile.in

if BUILD_ENGINE_SDL

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/include \
@ECORE_SDL_CFLAGS@ @SDL_CFLAGS@ @ETK_CFLAGS@

noinst_LTLIBRARIES = libeon_engine_sdl.la

libeon_engine_sdl_la_SOURCES = \
sdl_canvas.c \
sdl_document.c \
sdl_engine.c \
sdl_shape.c

endif