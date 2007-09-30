#include <limits.h>

#include "Ecore_Data.h"
#include "Ecore_File.h"
#include "Ecore_Fb.h"
#include "ecore_fb_private.h"

/**
 * TODO:
 * - create a hacked input.h in case the kernel doesnt has a correct version
 * - same for broken kernel types
 */
/* a fix for broken kernels */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,15)) && (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18))
 #define kernel_ulong_t unsigned long 
 #define BITS_PER_LONG 32
 #include <linux/input.h>
 #undef kernel_ulong_t <-added
 #undef BITS_PER_LONG <-added
#else
 #include <linux/input.h>
#endif


#define TEST_BIT(bit, array)    (array[bit/8] & (1<<(bit%8)))

static void module_init(void);
static void module_shutdown(void);
static int module_open(Ecore_Fb_Input_Device *d);
static int module_close(Ecore_Fb_Input_Device *d);
static int module_config(Ecore_Fb_Input_Device *d, const char *option, ...);
static void module_enable(Ecore_Fb_Input_Device *d, int enable);

Ecore_Fb_Module_Input module_linux_input = {
	.name = "linux_input",
	.description = "",
	.init = module_init,
	.open = module_open,
	.close = module_close,
	.enable = module_enable,
	.config = module_config,
};


/* ==== local ==== */
static void 
_li_event_key_down_free(void *data, void *ev)
{

	ecore_fb_event_key_up_free((Ecore_Fb_Event_Key_Up *)ev);
}

static void 
_li_event_key_up_free(void *data, void *ev)
{
	ecore_fb_event_key_down_free((Ecore_Fb_Event_Key_Down *)ev);
}

static void
_li_device_event_key(Ecore_Fb_Input_Device *dev, struct input_event *iev)
{
	/* check for basic keyboard keys */
	if (iev->code >= KEY_ESC && iev->code <= KEY_COMPOSE)
	{
		/* if we have a value is a down */
		if (iev->value)
		{
			Ecore_Fb_Event_Key_Down *ev;

			ev = ecore_fb_event_key_down_get(dev, iev->code);
			if (!ev) return;
			
			ecore_event_add(ECORE_FB_EVENT_KEY_DOWN, ev, _li_event_key_down_free, NULL);

		}
		/* else is an up */
		else
		{
			Ecore_Fb_Event_Key_Up *ev;

			ev = ecore_fb_event_key_up_get(dev, iev->code);
			if (!ev) return;
			
			ecore_event_add(ECORE_FB_EVENT_KEY_UP, ev, _li_event_key_up_free, NULL);
		}
	}
	/* check for mouse button events */
	else if (iev->code >= BTN_MOUSE && iev->code < BTN_JOYSTICK)
	{
		int button;

		button = ((iev->code & 0x00F) + 1);
		if(iev->value)
		{
			Ecore_Fb_Event_Mouse_Button_Down *ev;
			double current;

			ev = calloc(1, sizeof(Ecore_Fb_Event_Mouse_Button_Down));
			ev->dev = dev;
			ev->button = button;
			ev->x = dev->mouse.x;
			ev->y = dev->mouse.y;

			current = ecore_time_get();
			if((current - dev->mouse.prev) <= dev->mouse.threshold)
			{
				ev->double_click = 1;
			}
			if((current - dev->mouse.last) <= (2 * dev->mouse.threshold))
			{
				ev->triple_click = 1;
				/* reset */
				dev->mouse.prev = 0;
				dev->mouse.last = 0;
				current = 0;
			}
			else
			{
				/* update values */
				dev->mouse.last = dev->mouse.prev;
				dev->mouse.prev = current;
			}
			ecore_event_add(ECORE_FB_EVENT_MOUSE_BUTTON_DOWN, ev, NULL ,NULL);
		}
		else
		{
			Ecore_Fb_Event_Mouse_Button_Up *ev;

			ev = calloc(1,sizeof(Ecore_Fb_Event_Mouse_Button_Up));
			ev->dev = dev;
			ev->button = button;
			ev->x = dev->mouse.x;
			ev->y = dev->mouse.y;

			ecore_event_add(ECORE_FB_EVENT_MOUSE_BUTTON_UP, ev, NULL ,NULL);
		}
	}
}

static void
_li_device_event_rel(Ecore_Fb_Input_Device *dev, struct input_event *iev)
{
	/* dispatch the button events if they are queued */
	switch(iev->code)
	{
		case REL_X:
		case REL_Y:
		{
			Ecore_Fb_Event_Mouse_Move *ev;
			if(iev->code == REL_X)
			{
				dev->mouse.x += iev->value;
				if(dev->mouse.x > dev->mouse.w - 1)
					dev->mouse.x = dev->mouse.w;
				else if(dev->mouse.x < 0)
					dev->mouse.x = 0;
			}
			else
			{
				dev->mouse.y += iev->value;
				if(dev->mouse.y > dev->mouse.h - 1)
					dev->mouse.y = dev->mouse.h;
				else if(dev->mouse.y < 0)
					dev->mouse.y = 0;
			}
			ev = calloc(1,sizeof(Ecore_Fb_Event_Mouse_Move));
			ev->x = dev->mouse.x;
			ev->y = dev->mouse.y;
			ev->dev = dev;

			ecore_event_add(ECORE_FB_EVENT_MOUSE_MOVE,ev,NULL,NULL);
			break;
		}
		case REL_WHEEL:
		case REL_HWHEEL:
		{
			Ecore_Fb_Event_Mouse_Wheel *ev;
			ev = calloc(1, sizeof(Ecore_Fb_Event_Mouse_Wheel));

			ev->x = dev->mouse.x;
			ev->y = dev->mouse.y;
			if(iev->code == REL_HWHEEL)
				ev->direction = 1;
			ev->wheel = iev->value;
			ev->dev = dev;
			ecore_event_add(ECORE_FB_EVENT_MOUSE_WHEEL, ev, NULL, NULL);
			break;
		}
		default:
			break;
	}
}

static void
_li_device_event_abs(Ecore_Fb_Input_Device *dev, struct input_event *iev)
{
	switch(iev->code)
	{
		case ABS_X:
		case ABS_Y:
		{
			Ecore_Fb_Event_Mouse_Move *ev;
			if((iev->code == ABS_X) && (dev->mouse.w != 0))
			{
				int tmp;

				tmp = (int)((double)(iev->value - dev->mouse.min_w) / dev->mouse.rel_w);
				if(tmp < 0)
					dev->mouse.x = 0;
				else if(tmp > dev->mouse.w)
					dev->mouse.x = dev->mouse.w;
				else
					dev->mouse.x = tmp;
			}
			else if((iev->code == ABS_Y) && (dev->mouse.h != 0))
			{
				int tmp;

				tmp = (int)((double)(iev->value - dev->mouse.min_h) / dev->mouse.rel_h);
				if(tmp < 0)
					dev->mouse.y = 0;
				else if(tmp > dev->mouse.h)
					dev->mouse.y = dev->mouse.h;
				else
					dev->mouse.y = tmp;
			}
			ev = calloc(1,sizeof(Ecore_Fb_Event_Mouse_Move));
			ev->x = dev->mouse.x;
			ev->y = dev->mouse.y;
			ev->dev = dev;

			ecore_event_add(ECORE_FB_EVENT_MOUSE_MOVE, ev, NULL, NULL);
			break;
		}
		case ABS_PRESSURE:
			/* TODO emulate a button press */
			break;
	}
}

static int
_device_fd_callback(void *data, Ecore_Fd_Handler *fdh)
{
	Ecore_Fb_Input_Device *dev;
	struct input_event ev[64];
	int len;
	int i;

	dev = (Ecore_Fb_Input_Device*)data;

	/* read up to 64 events at once */
	len = read(dev->fd, &ev, sizeof(ev));
	// printf("[ecore_fb_li_device:fd_callback] received %d data\n", len);
	for(i = 0; i < len/sizeof(ev[0]); i++)
	{
		switch(ev[i].type)
		{
			case EV_ABS:
				_li_device_event_abs(dev, &ev[i]);
				break;
			case EV_REL:
				_li_device_event_rel(dev, &ev[i]);
				break;
			case EV_KEY:
				_li_device_event_key(dev, &ev[i]);
				break;
			default:
				break;
		}
	}
	return 1;
}

static void
_create_device_cb(const char *file, const char *dir)
{
	/* create an input device in case the device name is event* */
	if (!strncmp(file, "event", 5))
	{
		char buf[PATH_MAX];
		/* we dont care about the return value, the device is
		 * stored on the device list
		 */
		snprintf(buf, PATH_MAX, "%s/%s", dir, file);
		ecore_fb_input_device_open(buf, module_linux_input.name);
	}
}

/* ==== module API ==== */


static void
module_init(void)
{
	Ecore_List *files;

	/* find all devices with name /dev/input/event* */
	files = ecore_file_ls("/dev/input/");

	if (files)
		ecore_list_for_each(files, ECORE_FOR_EACH(_create_device_cb), "/dev/input");
}

static void
module_shutdown(void)
{

}

/**
 * an open call has to set up the following
 * fields:
 * - fd
 * - info.type
 * - info.name
 * also preparese the ecore_fd_handlers
 * - handler
 */
int module_open(Ecore_Fb_Input_Device *d)
{
	char event_type_bitmask[EV_MAX/8 + 1];
	int event_type;
	int fd;

	if ((fd = open(d->info.device, O_RDONLY, O_NONBLOCK)) < 0)
	{
		fprintf(stderr, "[ecore_fb_li:device_open] %s %s\n", d->info.device, strerror(errno));
		goto error;
	}
	/* query typeabilities */
	if (ioctl(fd, EVIOCGBIT(0, EV_MAX), event_type_bitmask) < 0)
	{
		fprintf(stderr,"[ecore_fb_li:device_open] query typeabilities %s %s\n", d->info.device, strerror(errno));
		goto error;
	}
	/* query name */
	d->info.name = calloc(256, sizeof(char));
	if (ioctl(fd, EVIOCGNAME(sizeof(char) * 256), d->info.name) < 0)
	{
		fprintf(stderr, "[ecore_fb_li:device_open] get name %s %s\n", d->info.device, strerror(errno));
		strcpy(d->info.name, "Unknown");
	}
	d->fd = fd;

	/* set info */
	for (event_type = 0; event_type < EV_MAX; event_type++)
	{
		if(!TEST_BIT(event_type, event_type_bitmask))
			continue;
		switch(event_type)
		{
			case EV_SYN :
			break;

			case EV_KEY:
			d->info.type |= ECORE_FB_INPUT_DEVICE_TYPE_KEYS_OR_BUTTONS;
			break;

			case EV_REL:
			d->info.type |= ECORE_FB_INPUT_DEVICE_TYPE_RELATIVE;
			break;

			case EV_ABS:
			d->info.type |= ECORE_FB_INPUT_DEVICE_TYPE_ABSOLUTE;
			break;

			case EV_MSC:
			case EV_LED:
			case EV_SND:
			case EV_REP:
			case EV_FF :
			case EV_FF_STATUS:
			case EV_PWR:
			default:
			break;
		}
	}
	return 1;

	error:
	close(fd);
	return 0;
}

int module_close(Ecore_Fb_Input_Device *d)
{
	/* close everything */
	close(d->fd);
	
	if (d->handler)
		ecore_main_fd_handler_del(d->handler);
	return 1;
}



void module_enable(Ecore_Fb_Input_Device *d, int enable)
{
	if (enable)
		d->handler = ecore_main_fd_handler_add(d->fd, ECORE_FD_READ, _device_fd_callback, d, NULL, NULL);
	else
	{
		ecore_main_fd_handler_del(d->handler);
		d->handler = NULL;
	}
}


int module_config(Ecore_Fb_Input_Device *d, const char *option, ...)
{
	return 1;
}
