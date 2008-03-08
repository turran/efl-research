#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>

#include "Etch.h"

/* Helper utility to test properties, animations, etc */

/* Properties callbacks */
void _position_x_uint32(void *odata, void *pdata)
{
	printf("_position_x_uint32 callback: value = %d\n", *(int *)pdata);
}

void _position_y_uint32(void *odata, void *pdata)
{
	printf("_position_y_uint32 callback: value = %d\n", *(int *)pdata);
}

/* Simple object class to test all properties */
Etch_Object_Property oproperties[] = {
	{
		.type = ETCH_POSITION_X_UINT32,
		.set = &_position_x_uint32, 
	},
	{
		.type = ETCH_POSITION_Y_UINT32,
		.set = &_position_y_uint32,
	},
};

Etch_Object_Class oclass = {
	.props = oproperties,
};

/* Timer function */
void signal_timer_cb(int s)
{
	/* TODO send a tick to etch :) and wait for events */
	printf("timer cb called\n");
	/* TODO to exit the main loop we should check that the etch
	 * animation has finished */
}


int main(void)
{
	Etch *e;
	Etch_Object *eo;
	int vuint32;
	struct sigaction sact;
	struct itimerval value;
	
	
	e = etch_new();
	eo = etch_object_add(e, &oclass, "object1", NULL);

	/* test simple set/get properties */
	vuint32 = 10;
	etch_object_property_set(eo, ETCH_POSITION_X_UINT32, &vuint32);
	etch_object_property_get(eo, ETCH_POSITION_X_UINT32, &vuint32);
	printf("property = %d\n", vuint32);

	/* create the timer callback */
	sact.sa_flags = 0;
	sact.sa_handler = signal_timer_cb;
	
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_usec = 20000;
	value.it_value.tv_sec = 0;
	value.it_value.tv_usec = 500000;
	sigaction(SIGALRM, &sact, NULL);
	setitimer(ITIMER_REAL, &value, NULL);
	
	/* iterate forever until some timer event gets triggered */
	while (1);

	
	etch_object_delete(eo);
	etch_free(e);

	return 0;
}
