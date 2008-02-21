## Process this file with automake to produce Makefile.in

.o.so:  
	$(CC) $(INCLUDES) $(LDFLAGS) $< -o $@

# A list of all the files in the current directory which can be regenerated
MAINTAINERCLEANFILES = Makefile.in

CLEANFILES = \
	libcodelang_la-codelang.h \
	libcodelang_la-codelang.c \
	codelang.c

lib_LTLIBRARIES = libcodelang.la

CODELANGHEADERS = \
	access.h \
	class.h \
	Codelang.h \
	method.h \
	list.h \
	memory_allocator.h \
	parser.h \
	type.h \
	var.h \
	member.h \
	hash.h


CODELANGFILES = \
	codelang.y \
	codelang.l \
	access.c \
	class.c \
	method.c \
	list.c \
	memory_allocator.c \
	parser.c \
	type.c \
	var.c \
	member.c \
	hash.c \
	$(CODELANGHEADERS)

libcodelang_la_SOURCES = $(CODELANGFILES)

installed_headersdir = $(prefix)/include/codelang
installed_headers_DATA = $(CODELANGHEADERS)

libcodelang_la_YFLAGS		= -d
libcodelang_la_LIBADD		= @LEXLIB@ -lm
libcodelang_la_DEPENDENCIES	= $(top_builddir)/src/config.h
libcodelang_la_LDFLAGS		= -version-info 1:0:1
libcodelang_la_CFLAGS		= -Wall
