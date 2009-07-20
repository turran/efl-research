#ifndef _EMM_H
#define _EMM_H

/**
 * Memory Manager:
 * It has a modular approach to use different policies, like
 * best fit, first fit, etc.
 * 
 */

typedef struct _Emm        Emm;
typedef struct _Emm_Policy Emm_Policy;
typedef struct _Emm_Item   Emm_Item;

Emm*	emm_new(void *heap, const char *name, int size); /* */
void	emm_delete(Emm *manager); /* */
void**	emm_malloc(Emm *manager, int size); /* */
void 	emm_free(Emm *manager, void *i); /* */

#endif
