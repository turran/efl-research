## Process this file with automake to produce Makefile.in
AUTOMAKE_OPTIONS = 1.5 foreign

SUBDIRS = src 

MAINTAINERCLEANFILES = Makefile.in aclocal.m4 config.guess \
                       config.h.in config.sub configure install-sh \
		       ltconfig ltmain.sh missing mkinstalldirs \
		       stamp-h.in 

EXTRA_DIST = README AUTHORS COPYING codelang.pc.in

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = codelang.pc
