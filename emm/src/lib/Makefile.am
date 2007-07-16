MAINTAINERCLEANFILES = Makefile.in

SUBDIRS = policies 


lib_LTLIBRARIES	= libemm.la
include_HEADERS	= Emm.h

INCLUDES = \
 -I. \
 -Ipolicies/firstfit \
 -I$(top_srcdir)/src/lib

libemm_la_LDFLAGS = \
 -version-info 1:0:0 \
 -L$(top_builddir)/src/lib/.libs

libemm_la_SOURCES = \
emm.c 

libemm_la_DEPENDENCIES = \
policies/libemm_policies.la

libemm_la_LIBADD = \
policies/libemm_policies.la 

