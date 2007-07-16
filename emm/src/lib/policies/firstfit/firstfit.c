#include "Emm.h"
#include "emm_private.h"

#include "firstfit.h"

/**
 * First-Fit Policy
 * 
 * Each item has something like this:
 * 
 * Emm_Item:
 * +------------------
 * | Firstfit_Item:
 * | +------------------
 * | |
 * | +--------
 * |
 * +-------------------
 */


/* NOTE: the unsigned char is because C when adding an integer to pointer
 * it adds as many bytes as the size of the data the pointer points to
 */

Emm_Policy policy_firstfit = 
{
	firstfit_init,
	firstfit_shutdown,
	firstfit_malloc,
	firstfit_calloc,
	firstfit_free,
	NULL, /* resize */
	NULL, /* before */
	NULL  /* after */

};

typedef struct _Firstfit
{
	int size_left; 		/* The free space we have in the memory block */
	Emm *mm;			/* A pointer to the Emm struct */
	Emm_Item *first; 	/* A pointer to the first item on the memory block */

} Firstfit;

typedef struct _Firstfit_Item
{
	Emm_Item *prev;		/* A pointer to the prevous item */
	Emm_Item *next;		/* A pointer to the next item */
} Firstfit_Item;

#define ITEM_SIZE sizeof(Emm_Item) + sizeof(Firstfit_Item)
#define POLICY_OFFSET sizeof(Emm_Item)

void *
firstfit_init(Emm *manager)
{
	Firstfit *f;
	Firstfit_Item *fitem;
	Emm_Item *item;

	/* setup the variables from the manager data */	
	f = malloc(sizeof(Firstfit));
	f->size_left = manager->size;
	f->mm = manager;

 	/* On initialization the block has one free item that fits on all the block. */
	item = f->mm->top;
	item->size = f->mm->size;
	item->data = (unsigned char *)item + POLICY_OFFSET;
	item->free = 1;
	item->ptr = (unsigned char *)item + ITEM_SIZE;

	fitem = item->data;
	fitem->prev = NULL;
	fitem->next = NULL;

	f->first = item;

	manager->item_size = ITEM_SIZE;
	return f;
}

void
firstfit_shutdown(Emm *manager, void *data)
{
	Firstfit *f;

	f = (Firstfit *)data;
	free(f);
}

Emm_Item * 
firstfit_malloc(void *data, int size)
{
	Firstfit *f;
	Firstfit_Item *fitem;
	Emm_Item *item;

	
	f = (Firstfit *)data;
	item = f->first;
	
	/* can we allocate a new item without overflow? */
	if(f->size_left <= ITEM_SIZE + size)
	{
		_emm_resize(f->mm);
		printf("need to resize the pool %d %d\n", f->size_left, ITEM_SIZE + size);
		return NULL;
	}
	while(item)
	{
		fitem = item->data;
		if((item->free) && (item->size >= size))
		{
			Firstfit_Item *n_fitem;
			Emm_Item *n_item;
			
			/* if the size of the prev item and the requested size is equal
			 * dont create a new item */
			if(item->size == size)
				return item;
			printf("create\n");
			/* if not create a new item */
			
			/* TODO first check if we can create a new item between 
			 * the current and the next item */
			if((item->size - size) < ITEM_SIZE)
			{
				/* TODO if not, make the new item the size of the free 
				 * ie, dont split space */
				printf("spank!!!\n");
				return NULL;
			}
			n_item = (unsigned char *)item + ITEM_SIZE + size;
			n_item->size = item->size - size;
			n_item->ptr = (unsigned char *)item + ITEM_SIZE;
			n_item->data = (unsigned char*)item + POLICY_OFFSET;
			n_item->free = 1;
			
			n_fitem = n_item->data;
			n_fitem->prev = item;
			n_fitem->next = fitem->next;
			/* update the prev item */
			item->size = size;
			item->free = 0;
			fitem->next = n_item;

			f->size_left -= ITEM_SIZE + size;
			return item;
		}
		item = fitem->next;
	}
	/* this should never happen, if so, mean the list isnt joined */
	firstfit_idler(f);
	return firstfit_malloc(f, size);
}

Emm_Item * 
firstfit_calloc(void *data, int size)
{
	return firstfit_malloc(data, size);
}

void
firstfit_free(void *data, Emm_Item *item)
{
	Firstfit *f;
	
	f = (Firstfit *)data;
	f->size_left += ITEM_SIZE + item->size;
	item->free = 1;
}

/* on firstfit, the idler should only defragment 
 * the memory space (condense) all unused space
 */
void
firstfit_idler(void *data)
{
	Firstfit *f;
	Firstfit_Item *fitem;
	Emm_Item *item;
	
	f = (Firstfit *)data;
	item = f->first;

	while(item)
	{
		fitem = item->data;
		if((item->free) && (fitem->next) && (((Emm_Item *)(fitem->next))->free))
		{
			Emm_Item *next;

			next = fitem->next;
			/* join them */
			printf("joining\n");
			item->size += next->size + ITEM_SIZE; 
			continue;
		}
		item = fitem->next;
	}
}
