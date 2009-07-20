#include "Ecore_D.h"
#include "ecore_data_private.h"

#include <sys/mman.h>

/* double pointer struct to hide the real memory pointer
 * user mem:
 * +-----------+------+-----------+
 * | user data | free | user data | ...
 * +-----------+------+-----------+---
 *
 * policy mem (item headers):
 * +-------------+===========+----------------------+----------------------+
 * | real_ptr -> | user data | size, policy_data -> | prototype item struct |
 * +-------------+===========+----------------------+----------------------+
 *                    size
 * 
 * algorithm:
 * we have two current pointers, one for the user mem (memmanager received)
 * and one for the policy mem (the headers)
 * the idea is to keep the policy mem like this:
 * +-----------------+--------------------+-----------+
 * | allocated ..... | allocated but free | free .... |
 * +-----------------+--------------------+-----------+
 *                   O cur_pointer
 *                   
 * same on the user mem, but the free space isnt ordered at all, 
 * to keep it faster. on init we allocate one free item, same 
 * on the user mem.
 *
 * when we create a new item, we just have to allocate a new header on 
 * the position where the current pointer is, if the allocated data 
 * will overflow
 *
 * the memory space check if we still have space left on the chunk.
 * if we have space left means that the policy mem isnt ordered, 
 * so we order it and set the current pointer to the position where 
 * the allocated space finish.
 *
 *
 */



#define POLICY_OFFSET sizeof(void*) + sizeof(int) + sizeof(void*)
#define PM_ITEM_SIZE sizeof(Efb_Memmanager) + sizeof(Prototype_Item)

#define PM_ITEM_NEW(item, fitem) \
	item = (Efb_Memmanager_Item *)f->pm_cur; \
	item->real_pointer = f->um_cur; \
	item->size = f->pm_size; \
	item->policy_data =  f->pm_cur + POLICY_OFFSET; \
	fitem = (Prototype_Item *)item->policy_data; \
	fitem->prev = NULL; \
	fitem->next = NULL; \
	fitem->free = 1; \
	fitem->data_free = 1;



typedef struct _Prototype
{
	Efb_Memmanager *mm;
	/* pm = policy mem */
	void *pm;
	void *pm_cur;
	int   pm_size_left;
	int   pm_size;
	/* um = user mem */
	void *um;
	void *um_cur;
	int   um_size_left;
	int   um_size;

	int   is_ordered :1;

} Prototype;

typedef struct _Prototype_Item
{
	Efb_Memmanager_Item *prev; 
	/* next - cur = free space between */
	Efb_Memmanager_Item *next;
	unsigned char free :1;
	unsigned char data_free :1;
} Prototype_Item;

/* local functions */
static void _prototype_item_create(Prototype *f);

void *
prototype_policy_init(Efb_Memmanager *manager)
{
	Prototype *f;
	Prototype_Item *fitem;
	Efb_Memmanager_Item *item;

	void *mmaped = NULL;
	size_t pgsize;
	size_t mmsize;


	pgsize = getpagesize();
	/* reserve space for 100 items and make it page sizeable*/
	mmsize = sizeof(Efb_Memmanager_Item) * 100;
	mmsize = (mmsize + pgsize -1 ) & ~(pgsize-1);

	//fprintf(stdout, "[efb_memmanger:prototype_policy_init] using anonymous mapping %lu,%lu\n",(unsigned long)pgsize,(unsigned long)mmsize);

	if((mmaped = mmap(NULL, mmsize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == (void *)-1)
	{
		fprintf(stderr, "[efb_memmanger:prototype_policy_init] can't mmap %s\n", strerror(errno));
		return NULL;
	}
	f = calloc(1, sizeof(Prototype));
	
	f->mm = manager;
	f->um_size_left = manager->size;
	f->um = f->um_cur = manager->top;
	
	f->pm_size_left = mmsize;
	f->pm_size = mmsize;
	f->pm = f->pm_cur = mmaped;
	
	manager->policy_data = f;
	/* create a new item with free data of all the chunk size */
	item = (Efb_Memmanager_Item *)f->pm_cur;
	item->real_pointer = f->um_cur;
	item->size = f->pm_size;
	item->policy_data =  f->pm_cur + POLICY_OFFSET;
	fitem = (Prototype_Item *)item->policy_data;
	fitem->prev = NULL;
	fitem->next = NULL;
	fitem->free = 1;
	fitem->data_free = 1;
	return f;
}

void
prototype_policy_shutdown(Efb_Memmanager *manager, void *data)
{
	Prototype *f;

	f = (Prototype *)data;
	free(f);
}

Efb_Memmanager_Item * 
prototype_item_create(void *data, int size)
{
	Prototype *f;
	Efb_Memmanager_Item *item;
	Prototype_Item *fitem;

	f = (Prototype *)data;
	item = (Efb_Memmanager_Item *)f->pm_cur;
	fitem = (Prototype_Item *)item->policy_data;

	/* if the size left is lesser than requested we cant allocate a new item */
	if((f->um_size_left < size) || (f->pm_size_left < PM_ITEM_SIZE))
	{
		printf("se excedio!!!!\n");
		return NULL;
	}
	/* if there's space left and the cur 
	 * position = max ofsset, the chunk is unordered
	 * order it
	 */

	if(fitem->data_free)
	{
		/* redirect the real pointer */
		//fitem->real_pointer = ;
		/* increase the size */
		f->um_size_left -= size;
		
		/* append a new item at the end */
		if(!fitem->next)
		{
			
		}
		/* create a new item */
		Efb_Memmanager_Item *n_item;
		Prototype_Item *n_fitem;
		
		/* allocate a new one */
		f->pm_size_left -= PM_ITEM_SIZE;
		/*f->pm_cur += 
		f->um_cur +=

		PM_ITEM_NEW(n_item, n_fitem);
		n_fitem->prev = fitem;
*/

	
	}
	else
	{
	}
#if 0 
	/* FIXME when we alloc the last item, make the pointer point to it, instead of the next mem position */
	/* create the header */
	item->size = size;
	item->real_pointer = f->um_cur;
	item->policy_data = f->pm_cur + POLICY_OFFSET;

	fitem = (Prototype_Item *)item->policy_data;
	fitem->data_free = 0;
	fitem->free = 0;
	fitem->next = NULL;
	
	f->um_cur += item->size;
	/* check if we are the first item */
	if(f->pm_cur == f->pm)
		fitem->prev = NULL;
	else
	{
		Efb_Memmanager_Item *prev;
		Prototype_Item *fprev;

		fitem->prev = f->pm_cur - (sizeof(Efb_Memmanager_Item) + sizeof(Prototype_Item));
		/* update the next field of the prev item */
		prev = fitem->prev;
		fprev = prev->policy_data;
		fprev->next = f->pm_cur;
	}
	/* move the currrent pointer */
	f->pm_cur +=  sizeof(Efb_Memmanager_Item) + sizeof(Prototype_Item);
#endif
	return item;
}

void
prototype_item_delete(void *data, Efb_Memmanager_Item *item)
{

}
/* on prototype, the idler should only defragment the memory space (condense)
 * all used space first then all unused space
 */
void
prototype_policy_idler(void *data)
{


}


