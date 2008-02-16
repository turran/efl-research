export CFLAGS=-g
gcc $CFLAGS -c Array.c 
gcc $CFLAGS -c Object.c 
gcc $CFLAGS -c String.c
gcc $CFLAGS -c garbage_collector.c
gcc $CFLAGS -c clist.c
gcc $CFLAGS -c objects_list.c
gcc $CFLAGS -c memory_allocator.c

gcc $CFLAGS -I. \
Array.o \
Object.o \
String.o \
garbage_collector.o \
clist.o \
objects_list.o \
memory_allocator.o \
main.c -o main

gcc $CFLAGS -I. \
Array.o \
Object.o \
String.o \
garbage_collector.o \
clist.o \
objects_list.o \
memory_allocator.o \
tests.c -o tests
