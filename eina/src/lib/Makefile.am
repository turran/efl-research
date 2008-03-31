MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I$(top_srcdir)/src/include

lib_LTLIBRARIES = libeina.la

libeina_la_SOURCES = \
eina_hash.c \
eina_lalloc.c \
eina_inlist.c \
eina_list.c \
eina_sheap.c \
eina_tree.c \
eina_module.c \
eina_value.c

libeina_la_LIBADD = -lm
libeina_la_LDFLAGS = @create_shared_lib@ -version-info @version_info@
