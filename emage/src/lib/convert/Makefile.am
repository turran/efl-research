AUTOMAKE_OPTIONS     = 1.4 foreign

SUBDIRS = emage_op_add emage_op_blend emage_op_copy emage_op_mask emage_op_mul emage_op_sub 

MAINTAINERCLEANFILES = Makefile.in

INCLUDES	     = -I. \
		       -I$(top_srcdir)/src/lib \
		       -I$(top_srcdir)/src/lib/include \
		       @FREETYPE_CFLAGS@ @VALGRIND_CFLAGS@ \
		       @EET_CFLAGS@ @pthread_cflags@

noinst_LTLIBRARIES      = libemage_object.la
libemage_engine_common_la_SOURCES  = \
emage_op_copy_main_.c \
emage_op_blend_main_.c \
emage_op_add_main_.c \
emage_op_sub_main_.c \
emage_op_mask_main_.c \
emage_op_mul_main_.c \
emage_blend_main.c \
emage_blit_main.c \
emage_convert_color.c \
emage_convert_gry_1.c \
emage_convert_gry_4.c \
emage_convert_gry_8.c \
emage_convert_main.c \
emage_convert_rgb_16.c \
emage_convert_rgb_24.c \
emage_convert_rgb_32.c \
emage_convert_rgb_8.c \
emage_convert_yuv.c \
emage_cpu.c \
emage_draw_main.c \
emage_font_draw.c \
emage_font_load.c \
emage_font_main.c \
emage_font_query.c \
emage_gradient_main.c \
emage_gradient_linear.c \
emage_gradient_radial.c \
emage_gradient_angular.c \
emage_gradient_rectangular.c \
emage_gradient_sinusoidal.c \
emage_image_load.c \
emage_image_save.c \
emage_image_main.c \
emage_line_main.c \
emage_polygon_main.c \
emage_rectangle_main.c \
emage_scale_main.c \
emage_scale_sample.c \
emage_scale_smooth.c \
emage_scale_span.c \
emage_tiler.c \
emage_regionbuf.c \
emage_array_hash.c \
emage_pipe.c

EXTRA_DIST = \
emage_scale_smooth_scaler.c \
emage_scale_smooth_scaler_down.c \
emage_scale_smooth_scaler_downx.c \
emage_scale_smooth_scaler_downx_downy.c \
emage_scale_smooth_scaler_downy.c \
emage_scale_smooth_scaler_noscale.c \
emage_scale_smooth_scaler_up.c

libemage_engine_common_la_DEPENDENCIES = \
$(top_builddir)/config.h
