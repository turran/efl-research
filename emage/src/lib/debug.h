#ifndef _DEBUG_H
#define _DEBUG_H

/* FIXME place the DEBUG define in config.h.in */
//#define DEBUG
#ifdef DEBUG
#define PRINTF(fmt, args...)  printf("[emage:%s]  " fmt, __FUNCTION__ , ## args)
#else
#define NDEBUG
#define PRINTF(fmt, args...)
#endif

#include <assert.h>

#endif
