MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
-I$(top_srcdir)/src/include

lib_LTLIBRARIES = libuio.la

libuio_la_SOURCES = \
main.c

libuio_la_LIBADD =

libuio_la_LDFLAGS = @create_shared_lib@ -version-info @version_info@
