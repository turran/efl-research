AUTOMAKE_OPTIONS     = 1.4 foreign

MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
-I. \
-I$(top_srcdir)/src/lib 

noinst_LTLIBRARIES      = libemage_op.la

libemage_op_la_SOURCES  = \
emage_op_add_main_.c \
emage_op_copy_main_.c \
emage_op_mul_main_.c \
emage_op_blend_main_.c \
emage_op_mask_main_.c \
emage_op_sub_main_.c

libemage_op_la_DEPENDENCIES = \
$(top_builddir)/src/lib/config.h
