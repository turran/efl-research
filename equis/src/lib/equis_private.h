#ifndef _PRIVATE_H
#define _PRIVATE_H

#include <stdlib.h>
#include <stdio.h>

#define SSE

#ifdef SSE
#include "x86_sse.h"
#endif

#ifdef MMX 
#include "x86_mmx.h"
#endif

#include "path.h"

#endif
