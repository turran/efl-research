MAINTAINERCLEANFILES = \
Makefile.in

AM_CPPFLAGS = \
-I. \
@ENESIM_CFLAGS@

bin_PROGRAMS = enesim_generator enesim_bench

enesim_generator_DEPENDENCIES =
enesim_generator_SOURCES = \
enesim_generator.c \
enesim_generator_core.c \
enesim_generator_drawer.c \
common.c \
fixed.c


enesim_bench_DEPENDENCIES = $(top_builddir)/src/lib/libenesim.la
enesim_bench_LDADD = \
$(top_builddir)/src/lib/libenesim.la \
@ENESIM_LIBS@

enesim_bench_SOURCES = \
enesim_bench.c \
common.c