#ifndef _EFB_PROTOTYPE_H
#define _EFB_PROTOTYPE_H



Efb_Memmanager_Item* prototype_item_create(void *data, int size);
void*                prototype_policy_init(Efb_Memmanager *manager);
void                 prototype_policy_shutdown(Efb_Memmanager *manager, void *data);

Efb_Memmanager_Policy policy_prototype = 
{
	prototype_policy_init,
	prototype_policy_shutdown,
	prototype_item_create,
	NULL, /* destroy */
	NULL, /* resize */
	NULL, /* before */
	NULL  /* after */

};


#endif
