#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include "Object.h"

#include "clist.h"
#include "objects_list.h"
#include "garbage_collector.h"

static unsigned int gc_on = 0;
static unsigned int gc_sweeping = 0;

static void gc_alarm_handler(int sig);
static void gc_sweep(void);

void gc_start(void)
{
	gc_on = 1;
	signal(SIGALRM, gc_alarm_handler);
	alarm(5);
}

void gc_stop(void)
{
	gc_on = 0;
}

void gc_run(void)
{
	gc_stop();
	gc_sweep();
	gc_start();
}

static void gc_alarm_handler(int sig)
{	
	if (gc_on)
	{
		signal(SIGALRM, gc_alarm_handler);
	}

	if (!gc_sweeping)
	{
		gc_sweep();
	}
	else
	{
		return;
	}

	alarm(5);
}

static void gc_sweep(void)
{
	CList *l;
	CList *objects;

	gc_sweeping = 1;
	printf("GC_SWEEP()\n");

	objects = objects_list_get();
	for (l = objects; l; l = l->next)
	{		
		Object *object = OBJECT(l->data);
		if (object->ref_count == 0)
		{
			printf("cleaning obj: %p\n", object);
			objects_list_delete(object);
			mem_free(object);
		}
		else
		{
			printf("keeping obj: %p\n", object);
		}
	}

	gc_sweeping = 0;
}
