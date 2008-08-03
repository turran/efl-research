MAINTAINERCLEANFILES = \
Makefile.in

AM_CPPFLAGS = \
-I. \
@ENESIM_CFLAGS@

bin_PROGRAMS = enesim_generator

enesim_generator_LDADD = \
@COVERAGE_LIBS@

enesim_generator_DEPENDENCIES =
enesim_generator_SOURCES = \
enesim_generator.c \
enesim_generator_core.c \
enesim_generator_drawer.c \
common.c \
fixed.c