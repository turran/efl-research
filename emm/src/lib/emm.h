#ifndef _E_MEMMANAGER_H
#define _E_MEMMANAGER_H

/* Memory Manager:
 * ===============
 * It has a modular approach to use different policies, like
 * best fit, first fit, etc.
 */


typedef struct _Ecore_D_Memmanager        Ecore_D_Memmanager;
typedef struct _Ecore_D_Memmanager_Policy Ecore_D_Memmanager_Policy;
typedef struct _Ecore_D_Memmanager_Item   Ecore_D_Memmanager_Item;

struct _Ecore_D_Memmanager
{
	int                    size;
	void                  *top;

	Ecore_D_Memmanager_Policy *policy;
	char                  *name;
	void                  *data;
	int                    item_size;
};

struct _Ecore_D_Memmanager_Policy
{
	void *               (*init)(Ecore_D_Memmanager *);
	void                 (*shutdown)(Ecore_D_Memmanager *, void *data);
	Ecore_D_Memmanager_Item *(*malloc)(void *data, int size);
	Ecore_D_Memmanager_Item *(*calloc)(void *data, int size);
	void                 (*free)(void *data, Ecore_D_Memmanager_Item *);
	void                 (*realloc)(void *data, Ecore_D_Memmanager_Item *, int size);
	void                 (*resize_before)(void *data);
	void                 (*resize_after)(void *data);
};

/* a basic header for a memory item */
struct _Ecore_D_Memmanager_Item
{
	void *ptr; /* to enable double de-referencing */

	int   size;
	unsigned char free :1;
	
	void *data;
};

#endif
