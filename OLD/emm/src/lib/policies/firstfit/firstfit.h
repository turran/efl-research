#ifndef _EMM_FIRSTFIT_H
#define _EMM_FIRSTFIT_H

Emm_Item * firstfit_malloc(void *data, int size);
Emm_Item * firstfit_calloc(void *data, int size);
void                  firstfit_free(void *data, Emm_Item *);
void *                firstfit_init(Emm *manager);
void                  firstfit_shutdown(Emm *manager, void *data);
void                  firstfit_idler(void *data);

extern Emm_Policy policy_firstfit;

#endif
