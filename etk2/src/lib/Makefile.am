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
etk2_engine.c \
etk2_document.c \
etk2_canvas.c \
etk2_shape.c \
etk2_rect.c \
etk2_animation.c
#
#etk2_button.c
#etk2_widget.c
#etk2_label.c

libetk2_la_LIBADD = @ETK_LIBS@ \
$(top_builddir)/src/engines/enesim/libetk_engine_enesim.la

if BUILD_ENGINE_SDL
libetk2_la_LIBADD += $(top_builddir)/src/engines/sdl/libetk_engine_sdl.la
endif

libetk2_la_DEPENDENCIES = $(top_builddir)/src/engines/enesim/libetk_engine_enesim.la
if BUILD_ENGINE_SDL
libetk2_la_DEPENDENCIES += $(top_builddir)/src/engines/sdl/libetk_engine_sdl.la
endif
 

libetk2_la_LDFLAGS = -version-info 1:0:0

