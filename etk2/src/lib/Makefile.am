MAINTAINERCLEANFILES = Makefile.in

AUTOMAKE_OPTIONS = 1.4 foreign

INCLUDES	= \
-I. -I../.. -I$(top_srcdir)/src/include -Wall -g \
@ETK_CFLAGS@

lib_LTLIBRARIES = libeon.la

EXTRA_DIST = 

libeon_la_SOURCES = \
eon_value.c \
eon_coord.c \
eon_main.c \
eon_engine.c \
eon_document.c \
eon_canvas.c \
eon_shape.c \
eon_square.c \
eon_rect.c \
eon_circle.c \
eon_polygon.c \
eon_image.c \
eon_animation.c \
eon_anim_basic.c \
eon_anim_matrix.c \
eon_filter.c \
eon_fe_displace.c
#eon_button.c
#eon_widget.c
#eon_label.c

libeon_la_LIBADD = @ETK_LIBS@ \
$(top_builddir)/src/engines/enesim/libeon_engine_enesim.la

if BUILD_ENGINE_SDL
libeon_la_LIBADD += $(top_builddir)/src/engines/sdl/libeon_engine_sdl.la
endif

libeon_la_DEPENDENCIES = $(top_builddir)/src/engines/enesim/libeon_engine_enesim.la
if BUILD_ENGINE_SDL
libeon_la_DEPENDENCIES += $(top_builddir)/src/engines/sdl/libeon_engine_sdl.la
endif
 

libeon_la_LDFLAGS = -version-info 1:0:0

