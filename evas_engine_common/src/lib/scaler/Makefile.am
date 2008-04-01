SUBDIRS = sampled

INCLUDES = \
-I. \
-I$(top_srcdir)/src/lib 

noinst_LTLIBRARIES      = libemage_scaler.la

libemage_scaler_la_SOURCES = 

libemage_scaler_la_DEPENDENCIES = \
$(top_builddir)/src/lib/config.h \
sampled/libemage_scaler_sampled.la

libemage_scaler_la_LIBADD = \
sampled/libemage_scaler_sampled.la
