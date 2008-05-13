MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
-I. \
-I$(top_srcdir)/src/include \
@EINA_CFLAGS@

lib_LTLIBRARIES = libeobj.la

libeobj_la_SOURCES = \
eobj_class.c \
eobj_object.c \
eobj_property.c \
eobj_signal.c \
eobj_marshallers.c \
eobj_signal_callback.c

libeobj_la_LIBADD = \
@EINA_LIBS@

libeobj_la_DEPENDENCIES =
