#ifndef _CPU_H
#define _CPU_H

/** 
 * @file
 * @brief CPU
 * @defgroup CPU_Internal CPU
 * @ingroup Internal_Group Internal
 *
 * @todo
 * - Fix the functions and clean them
 *
 * @{
 */

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

#if defined(__ARM_ARCH_3M__) || defined(__ARM_ARCH_4__) || \
    defined(__ARM_ARCH_4T__)
# define __ARM_ARCH__ 4
#endif

#if defined(__ARM_ARCH_5__) || defined(__ARM_ARCH_5T__) || \
    defined(__ARM_ARCH_5E__) || defined(__ARM_ARCH_5TE__) || \
    defined(__ARM_ARCH_5TEJ__)
# define __ARM_ARCH__ 5
#endif

#if defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || \
    defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || \
    defined(__ARM_ARCH_6ZK__) || defined(__ARM_ARCH_6T2__)
# define __ARM_ARCH__ 6
#endif

#if defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || \
    defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__)
# define __ARM_ARCH__ 7
#endif


#if defined(__ARM_ARCH__) && (__ARM_ARCH__ >= 5)
/* tested on ARMv6 (arm1136j-s), Nokia N800 CPU */
#define pld(addr, off)                                                  \
   __asm__("pld [%[address], %[offset]]"::                              \
           [address] "r" (addr), [offset] "i" (off))
#else
#define pld(addr, off)
#endif /* __ARMEL__ */


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

/** @} */

#endif
