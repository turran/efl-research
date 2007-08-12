#ifndef _CPU_H
#define _CPU_H

/* check if we have at least one backend if not, there's an eror */
#ifndef BUILD_MMX
# ifndef BUILD_SSE
#  ifndef BUILD_C
#   error "Please Read the README"
#  endif
# endif
#endif

#if defined BUILD_MMX || defined BUILD_SSE
#include "emage_mmx.h"
#endif

#if defined HAVE_ALTIVEC_H
#include <altivec.h>
#ifdef CONFIG_DARWIN
	#define AVV(x...) (x)
#else
	#define AVV(x...) {x}
#endif
#endif

#ifndef WORDS_BIGENDIAN
/* x86 */
#define A_VAL(p) ((DATA8 *)(p))[3]
#define R_VAL(p) ((DATA8 *)(p))[2]
#define G_VAL(p) ((DATA8 *)(p))[1]
#define B_VAL(p) ((DATA8 *)(p))[0]
#define AR_VAL(p) ((DATA16 *)(p)[1])
#define GB_VAL(p) ((DATA16 *)(p)[0])
#else
/* ppc */
#define A_VAL(p) ((DATA8 *)(p))[0]
#define R_VAL(p) ((DATA8 *)(p))[1]
#define G_VAL(p) ((DATA8 *)(p))[2]
#define B_VAL(p) ((DATA8 *)(p))[3]
#define AR_VAL(p) ((DATA16 *)(p)[0])
#define GB_VAL(p) ((DATA16 *)(p)[1])
#endif


typedef enum _CPU_Feature
{
   CPU_FEATURE_C       = 0,
   CPU_FEATURE_MMX     = (1 << 0),
   CPU_FEATURE_MMX2    = (1 << 1),
   CPU_FEATURE_SSE     = (1 << 2), /* what about SSE2 ?? */
   CPU_FEATURE_ALTIVEC = (1 << 3),
   CPU_FEATURE_VIS     = (1 << 4),
   CPU_FEATURE_VIS2    = (1 << 5)
} CPU_Feature;

extern CPU_Feature Emage_CPU_Features;

#endif
