#ifndef _EMM_PRIVATE_H
#define _EMM_PRIVATE_H

//#define EMM_DEBUG 

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "config.h"

#ifdef EMM_DEBUG
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

/**
 * 
 * 
 */
struct _Emm
{
	int 		size;
	void 		*top;

	Emm_Policy 	*policy;
	char 		*name;
	void 		*data;
	int 		item_size;
#ifdef EMM_DEBUG
	int			fifo_fd;
	char 		fifo_path[PATH_MAX];			
#endif
};

/**
 * 
 * 
 */
struct _Emm_Policy
{
	void *               (*init)(Emm *);
	void                 (*shutdown)(Emm *, void *data);
	Emm_Item *(*malloc)(void *data, int size);
	Emm_Item *(*calloc)(void *data, int size);
	void                 (*free)(void *data, Emm_Item *);
	void                 (*realloc)(void *data, Emm_Item *, int size);
	void                 (*resize_before)(void *data);
	void                 (*resize_after)(void *data);
};

/** a basic header for a memory item
 * 
 */
struct _Emm_Item
{
	/* data info */
	void *ptr; 				/* A pointer to the real data. To enable double de-referencing */
	int   size; 			/* The data size */
	/* item info */
	unsigned char free :1; 	/* Is this item free? */
	/* policy info */
	void *data; 			/* The per-item data setup by the policy */
};


void _emm_resize(Emm *manager);

#endif
