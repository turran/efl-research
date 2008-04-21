#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>

#include "Etch.h"

/* Helper utility to test properties, animations, etc */

int _timer_event = 0;

/* Properties callbacks */
void _position_x_uint32(void *odata, void *pdata)
{
	//printf("_position_x_uint32 callback: value = %d\n", *(unsigned int *)pdata);
	printf("%d,\n", *(unsigned int *)pdata);
}

void _position_y_uint32(void *odata, void *pdata)
{
	printf("_position_y_uint32 callback: value = %d\n", *(unsigned int *)pdata);
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
void timer_signal_cb(int s)
{
	_timer_event = 1;
}

void timer_setup(void)
{
	struct sigaction sact;
	struct itimerval value;
			
	/* create the timer callback */
	sact.sa_flags = 0;
	sact.sa_handler = timer_signal_cb;
	
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_usec = 33333; /* every 33333 (1/30fps) usecs */
	value.it_value.tv_sec = 0;
	value.it_value.tv_usec = 500000; /* wait 500 usecs, before triggering the first event */
	sigaction(SIGALRM, &sact, NULL);
	setitimer(ITIMER_REAL, &value, NULL);	
}

/* test simple set/get properties */
void object_property_test(Etch_Object *eo)
{
	int vuint32 = 10;
	
	etch_object_property_set(eo, ETCH_POSITION_X_UINT32, &vuint32);
	etch_object_property_get(eo, ETCH_POSITION_X_UINT32, &vuint32);
	printf("property = %d\n", vuint32);	
}

void object_animation_setup(Etch_Object *eo)
{
	Etch_Animation *ea;
	Etch_Animation_Keyframe *ek;
	
	ea = etch_animation_new(ETCH_UINT32);
	/* first keyframe */
	ek = etch_animation_keyframe_add(ea);
	/* quadratic example */
	etch_animation_keyframe_type_set(ek, ETCH_ANIMATION_LINEAR);
	etch_animation_keyframe_value_set(ek, 10, 15);
	etch_animation_keyframe_time_set(ek, 3, 3015);
	/* second keyframe */
	ek = etch_animation_keyframe_add(ea);
	etch_animation_keyframe_type_set(ek, ETCH_ANIMATION_LINEAR);
	etch_animation_keyframe_value_set(ek,40);
	etch_animation_keyframe_time_set(ek, 25, 1237);
	/* third keyframe */
	ek = etch_animation_keyframe_add(ea);
	etch_animation_keyframe_type_set(ek, ETCH_ANIMATION_LINEAR);
	etch_animation_keyframe_value_set(ek, ETCH_ANIMATION_LINEAR, 30);
	etch_animation_keyframe_time_set(ek, 15, 2530);
	/* fourth keyframe */
	ek = etch_animation_keyframe_add(ea);
	etch_animation_keyframe_type_set(ek, ETCH_ANIMATION_LINEAR);
	etch_animation_keyframe_value_set(ek, ETCH_ANIMATION_LINEAR, 25);
	etch_animation_keyframe_time_set(ek, 1, 2530);
	/* append the animation to the object */
	etch_object_animation_set(eo, ETCH_POSITION_X, ea);
}

int main(void)
{
	Etch *e;
	Etch_Object *eo;
	
	e = etch_new();
	etch_timer_fps_set(e, 30);
	
	eo = etch_object_add(e, &oclass, "object1", NULL);

	object_property_test(eo);
	object_animation_setup(eo);
	
	timer_setup();
	/* to exit the main loop we should check that the etch animation has finished */
	while (!(etch_timer_has_end(e)))
	{
		if (_timer_event)
		{
			/* send a tick to etch :) and wait for events */
			etch_timer_tick(e);
			_timer_event = 0;
		}
	}
	etch_object_delete(eo);
	etch_free(e);

	return 0;
}
