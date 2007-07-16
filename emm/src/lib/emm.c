#include "Emm.h"
#include "emm_private.h"

#include "firstfit.h"

#ifdef EMM_DEBUG
/* the fifo protocol is:
 * R = free
 * M = malloc
 * N = new manager
 * T = delete manager
 */
static int _fifo_nr = 0;


/* ==== subsystem functions ==== */

int
_emm_fifo_create(Emm *manager)
{
	snprintf(manager->fifo_path, PATH_MAX, "/tmp/emm%d-%d", getpid(), _fifo_nr);
	if(mkfifo(manager->fifo_path, 0666) < 0)
	{
		printf("[emm] error creating the fifo: %s\n", strerror(errno));
		return -1;	
	}
	printf("[emm] created a blocking fifo on %s, now connect a frontend\n", manager->fifo_path);
	if((manager->fifo_fd = open(manager->fifo_path, O_WRONLY)) < 0)
	{
		printf("[emm] error opening the fifo: %s (%s)\n", strerror(errno), manager->fifo_path);
		return -1;	
	}	
	_fifo_nr++;
	return 0;
}

#endif

void
_emm_resize(Emm *manager)
{
	
}

/* ==== api functions ==== */

/**
 * 
 * 
 */
Emm *
emm_new(void *heap, const char *name, int size)
{
	Emm *mm;

	if(!heap || !name || !size) return NULL;
	
	mm = calloc(1, sizeof(Emm));
	if(!mm) return NULL;
#ifdef EMM_DEBUG
	{
		char msg[256];
		int length;
		if(_emm_fifo_create(mm) < 0)
		{
			free(mm);
			return NULL;	
		}
		length = snprintf(msg, 256, "N %p %d", heap, size);
		write(mm->fifo_fd, msg, length);
	}
#endif
	mm->top = heap;
	mm->size = size;

	if(!strcmp(name, "firstfit"))
		mm->policy = &policy_firstfit;
	if(mm->policy)
	{
		mm->name = strdup(name);
		mm->data = mm->policy->init(mm);
	}
	return mm;
}

void
emm_delete(Emm *manager)
{
#ifdef EMM_DEBUG
	close(manager->fifo_fd);
#endif
	free(manager->name);
	free(manager);	
}

/**
 *  
 * 
 */
void **
emm_malloc(Emm *manager, int size)
{

	if(!size) return NULL;
	if(!manager || !manager->policy) return NULL;
#ifdef EMM_DEBUG
	{
		char msg[256];
		int length;
		void **ret;
		
		ret = manager->policy->malloc(manager->data, size);
		length = snprintf(msg, 256, "M %p %d %p %d", ret, manager->item_size, ret? *ret : NULL, size);
		write(manager->fifo_fd, msg, length);
		return ret;
	}
#endif

	return (void **)manager->policy->malloc(manager->data, size);
}

/**
 * 
 * 
 * 
 */
void
emm_free(Emm *manager, void *i)
{
	Emm_Item *item;

	if(!i) return;

	item = i;
	if(!manager || !manager->policy) return;

	manager->policy->free(manager->data, item);
}
