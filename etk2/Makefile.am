## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src 

MAINTAINERCLEANFILES = Makefile.in aclocal.m4 config.guess \
		config.h.in config.sub configure install-sh \
		ltconfig ltmain.sh missing mkinstalldirs \
		stamp-h.in build-stamp configure-stamp depcomp \
		etk2_docs.tar.gz \
		etk2.pc \
		debian/changelog

bin_SCRIPTS = 

EXTRA_DIST = \
AUTHORS \
COPYING \
gendoc \
Doxyfile \
README \
ABOUT-NLS \
etk2.pc.in 

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = etk2.pc
