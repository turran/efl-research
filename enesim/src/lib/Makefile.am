SUBDIRS = raster util vector

MAINTAINERCLEANFILES = \
Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/lib

lib_LTLIBRARIES = libenesim.la
include_HEADERS = Enesim.h

libenesim_la_SOURCES =

libenesim_la_LIBADD = \
raster/libenesim_raster.la \
util/libenesim_util.la \
vector/libenesim_vector.la \
@EINA_LIBS@

libenesim_la_DEPENDENCIES = \
raster/libenesim_raster.la \
util/libenesim_util.la \
vector/libenesim_vector.la 

libenesim_la_LDFLAGS = @create_shared_lib@ -version-info @version_info@
