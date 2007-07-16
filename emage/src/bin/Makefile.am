INCLUDES = -Wall -g \
-I$(top_srcdir)/src/lib

bin_PROGRAMS = emage_test

emage_test_SOURCES = \
emage_object.c \
emage_png.c \
emage_test.c

emage_test_LDADD = \
$(top_builddir)/src/lib/libemage.la \
-lpng12

emage_test_DEPENDENCIES = \
$(top_builddir)/src/lib/libemage.la


