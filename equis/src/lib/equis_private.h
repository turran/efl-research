#ifndef _PRIVATE_H
#define _PRIVATE_H

#include <stdlib.h>
#include <stdio.h>

#if 0
//#define SSE
#define C 

#ifdef SSE
#include "x86_sse.h"
#endif

#ifdef MMX 
#include "x86_mmx.h"
#endif

#ifdef C
#include "c.h"
#endif
#endif

#include "path.h"

#endif
