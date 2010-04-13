#include <stdio.h>
#include <Ecore.h>
#include <Ecore_Li.h>

static int _move_cb(void *data, int type, void *event)
{
	Ecore_Li_Event_Mouse_Move *ev;

	ev = event;
	printf("MOUSE MOVE EVENT\n");
	printf("+ NAME = %s\n", ecore_li_device_name_get(ev->dev));
	printf("+ X = %d\n", ev->x);
	printf("+ Y = %d\n", ev->y);
}

static int _wheel_cb(void *data, int type, void *event)
{
	Ecore_Li_Event_Mouse_Wheel *ev;

	ev = event;
	printf("MOUSE WHEEL EVENT\n");
	printf("+ NAME = %s\n", ecore_li_device_name_get(ev->dev));
	printf("+ X = %d\n", ev->x);
	printf("+ Y = %d\n", ev->y);
	printf("+ DIRECTION = %d, WHEEL = %d\n", ev->direction, ev->wheel);
}

static int _button_up_cb(void *data, int type, void *event)
{
	Ecore_Li_Event_Button_Up *ev;

	ev = event;
	printf("BUTTON UP\n");
	printf("+ NAME = %s\n", ecore_li_device_name_get(ev->dev));
	printf("+ BUTTON = %d\n", ev->button);
	printf("+ X = %d\n", ev->x);
	printf("+ Y = %d\n", ev->y);
}

static int _button_down_cb(void *data, int type, void *event)
{
	Ecore_Li_Event_Button_Down *ev;

	ev = event;
	printf("BUTTON DOWN\n");
	printf("+ NAME = %s\n", ecore_li_device_name_get(ev->dev));
	printf("+ BUTTON = %d\n", ev->button);
	printf("+ X = %d\n", ev->x);
	printf("+ Y = %d\n", ev->y);
	printf("+ DOUBLE = %d, TRIPLE = %d\n", ev->double_click, ev->triple_click);
}

static int _key_up_cb(void *data, int type, void *event)
{
	Ecore_Li_Event_Key_Up *ev;

	ev = event;
	printf("KEY UP\n");
	printf("+ NAME = %s\n", ecore_li_device_name_get(ev->dev));
	printf("+ KEYNAME = %s\n", ev->keyname);
	printf("+ KEYSYMBOL = %s\n", ev->keysymbol);
	printf("+ KEYCOMPOSE = %s\n", ev->key_compose);
}

static int _key_down_cb(void *data, int type, void *event)
{
	Ecore_Li_Event_Key_Down *ev;

	ev = event;
	printf("KEY DOWN\n");
	printf("+ NAME = %s\n", ecore_li_device_name_get(ev->dev));
	printf("+ KEYNAME = %s\n", ev->keyname);
	printf("+ KEYSYMBOL = %s\n", ev->keysymbol);
	printf("+ KEYCOMPOSE = %s\n", ev->key_compose);
}

int main(void)
{
	Ecore_Li_Device *d1, *d2;

	ecore_init();
	ecore_li_init();
	
	d1 = ecore_li_device_new("/dev/input/event1");
	ecore_li_device_axis_size_set(d1, 320, 320);
	d2 = ecore_li_device_new("/dev/input/event0");
	/* mouse */
	ecore_event_handler_add(ECORE_LI_EVENT_MOUSE_MOVE, _move_cb, d1);
	ecore_event_handler_add(ECORE_LI_EVENT_MOUSE_WHEEL, _wheel_cb, d1);
	ecore_event_handler_add(ECORE_LI_EVENT_BUTTON_UP, _button_up_cb, d1);
	ecore_event_handler_add(ECORE_LI_EVENT_BUTTON_DOWN, _button_down_cb, d1);
	/* keyboard */
	ecore_event_handler_add(ECORE_LI_EVENT_KEY_DOWN, _key_down_cb, d2);
	ecore_event_handler_add(ECORE_LI_EVENT_KEY_UP, _key_up_cb, d2);
	ecore_li_device_enable(d1, 1);
	ecore_li_device_enable(d2, 1);
	ecore_main_loop_begin();	
	ecore_li_device_delete(d1);
	ecore_li_device_delete(d2);
	ecore_li_shutdown();
	ecore_shutdown();
}
