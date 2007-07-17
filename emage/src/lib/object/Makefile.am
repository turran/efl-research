AUTOMAKE_OPTIONS     = 1.4 foreign

MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
-I. \
-I$(top_srcdir)/src/lib \
-Werror

noinst_LTLIBRARIES      = libemage_object.la

#emage_gradient_main.c 
#emage_gradient_linear.c 
#emage_gradient_radial.c
#emage_gradient_angular.c
#emage_gradient_rectangular.c
#emage_gradient_sinusoidal.c
 
libemage_object_la_SOURCES  = \
emage_line_main.c \
emage_polygon_main.c \
emage_rectangle_main.c 

libemage_object_la_DEPENDENCIES = \
$(top_builddir)/src/lib/config.h
