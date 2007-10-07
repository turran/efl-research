#if 0
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
Ecore_List *_devices = NULL;

static int 
_devices_get_cb(const Ecore_Fb_Input_Device *d, const char *n)
{
	if (!strcmp(d->info.device, n))
		return 0;
	else return 1;
}
static int 
_keyboard_modifiers_unset(int *mods, char *keyname)
{
	int ret = 0;

	if (!strcmp(keyname, "Shift"))
	{
		*mods &= ~(1 << (ECORE_FB_MOD_SHIFT - 1));
		ret = 1;
	}
	else if (!strcmp(keyname, "AltGr"))
	{
		*mods &= ~(1 << (ECORE_FB_MOD_ALTGR - 1));
		ret = 1;
	}
	else if (!strcmp(keyname, "Control"))
	{
		*mods &= ~(1 << (ECORE_FB_MOD_CONTROL - 1));
		ret = 1;
	}
	else if (!strcmp(keyname, "Alt"))
	{
		*mods &= ~(1 << (ECORE_FB_MOD_ALT - 1));
		ret = 1;
	}
	else if (!strcmp(keyname, "ShiftL"))
	{
		*mods &= ~(1 << (ECORE_FB_MOD_SHIFTL - 1));
		ret = 1;
	}
	else if (!strcmp(keyname, "ShiftR"))
	{
		*mods &= ~(1 << (ECORE_FB_MOD_SHIFTR - 1));
		ret = 1;
	}
	else if (!strcmp(keyname, "CtrlL"))
	{
		*mods &= ~(1 << (ECORE_FB_MOD_CONTROLL - 1));
		ret = 1;
	}
	else if (!strcmp(keyname, "CtrlR"))
	{
		*mods &= ~(1 << (ECORE_FB_MOD_CONTROLR - 1));
		ret = 1;
	}
	return ret;
}

static int 
_keyboard_modifiers_set(int *mods, char *keyname)
{
	int ret = 0;

	if (!strcmp(keyname, "Shift"))
	{
		*mods |= 1 << (ECORE_FB_MOD_SHIFT - 1);
		ret = 1;
	}
	else if (!strcmp(keyname, "AltGr"))
	{
		*mods |= 1 << (ECORE_FB_MOD_ALTGR - 1);
		ret = 1;
	}
	else if (!strcmp(keyname, "Control"))
	{
		*mods |= 1 << (ECORE_FB_MOD_CONTROL - 1);
		ret = 1;
	}
	else if (!strcmp(keyname, "Alt"))
	{
		*mods |= 1 << (ECORE_FB_MOD_ALT - 1);
		ret = 1;
	}
	else if (!strcmp(keyname, "ShiftL"))
	{
		*mods |= 1 << (ECORE_FB_MOD_SHIFTL - 1);
		ret = 1;
	}
	else if (!strcmp(keyname, "ShiftR"))
	{
		*mods |= 1 << (ECORE_FB_MOD_SHIFTR - 1);
		ret = 1;
	}
	else if (!strcmp(keyname, "CtrlL"))
	{
		*mods |= 1 << (ECORE_FB_MOD_CONTROLL - 1);
		ret = 1;
	}
	else if (!strcmp(keyname, "CtrlR"))
	{
		*mods |= 1 << (ECORE_FB_MOD_CONTROLR - 1);
		ret = 1;
	}
	return ret;
}

static int 
_keyboard_locks_toggle(int *locks, char *keyname)
{
	int ret = 0;
	if (!strcmp(keyname, "Caps_Lock"))
	{
		*locks = !*locks;
		ret = 1;
	}
	return ret;
}

static Ecore_Fb_Input_Device * 
_device_new(const char *name)
{
	Ecore_Fb_Input_Device *d;

	d = calloc(1, sizeof(Ecore_Fb_Input_Device));
	if (!d) return NULL;

	/* common values */
	d->info.device = strdup(name);
	d->mouse.threshold = ECORE_FB_CLICK_THRESHOLD_DEFAULT;
	d->keyboard.layout = &kbd_layout_default;

	return d;
}
/* ==== Subsystem ==== */

/**
 *
 *
 */
void 
ecore_fb_event_key_up_free(Ecore_Fb_Event_Key_Up *e)
{
	if (!e) return;

	free(e->keyname);
	if (e->keysymbol) free(e->keysymbol);
	if (e->key_compose) free(e->key_compose);
	free(e);
}

/**
 *
 *
 */
void 
ecore_fb_event_key_down_free(Ecore_Fb_Event_Key_Down *e)
{
	if (!e) return;
	free(e->keyname);
	if (e->keysymbol) free(e->keysymbol);
	if (e->key_compose) free(e->key_compose);
	free(e);
}

/**
 *
 *
 */
Ecore_Fb_Event_Key_Down *
ecore_fb_event_key_down_get(Ecore_Fb_Input_Device *d, int code)
{
	Ecore_Fb_Event_Key_Down *ev;
	char *tmp;
	char *keyname;

	if (!d) return NULL;

	ev = calloc(1, sizeof(Ecore_Fb_Event_Key_Down));
	if (!ev) return NULL;

	/* on modifiers/locks we aren't strict, just check on the
	 * "plain" element to see if it's a modifier
	 */
	tmp = d->keyboard.layout->codes[code].mod[ECORE_FB_MOD_PLAIN].name;
	/* check if its a modifier */
	if (tmp && _keyboard_modifiers_set(&d->keyboard.mods, tmp))
		keyname = tmp;
	/* check if its a capslock and toggle it */
	else if (tmp && _keyboard_locks_toggle(&d->keyboard.capslock, tmp))
		keyname = tmp;
	else
	{
		int mods = d->keyboard.mods;
		/* check the capslock, if it set and the 
		 * key supports it we behave like a shift
		 * or ~shift in case shift is already pressed 
		 * if the key doesnt support it we use the
		 * actual keyname
		 */
		if (d->keyboard.layout->codes[code].mod[mods].letter && d->keyboard.capslock)
		{
			if (mods & (1 << (ECORE_FB_MOD_SHIFT - 1)))
				mods &= ~(1 << (ECORE_FB_MOD_SHIFT - 1));
			else
				mods |= (1 << (ECORE_FB_MOD_SHIFT - 1));
		}
		keyname = d->keyboard.layout->codes[code].mod[mods].name;
	}
	ev->dev = d;
	ev->keyname = strdup(keyname);

	return ev;
}

/**
 *
 */
Ecore_Fb_Event_Key_Up *
ecore_fb_event_key_up_get(Ecore_Fb_Input_Device *d, int code)
{
	Ecore_Fb_Event_Key_Up *ev;
	char *keyname;
	char *tmp;
	int caps = 0;
	
	if (!d) return NULL;
			
	ev = calloc(1, sizeof(Ecore_Fb_Event_Key_Up));
	if (!ev) return NULL;

	
	tmp = d->keyboard.layout->codes[code].mod[ECORE_FB_MOD_PLAIN].name;
	if (tmp && _keyboard_modifiers_unset(&d->keyboard.mods, tmp))
		keyname = tmp;
	else if (tmp && _keyboard_locks_toggle(&caps, tmp))
		keyname = tmp;
	else
	{
		int mods = d->keyboard.mods;

		if (d->keyboard.layout->codes[code].mod[mods].letter && d->keyboard.capslock)
		{
			if (mods & (1 << (ECORE_FB_MOD_SHIFT - 1)))
				mods &= ~(1 << (ECORE_FB_MOD_SHIFT - 1));
			else
				mods |= (1 << (ECORE_FB_MOD_SHIFT - 1));
		}
		keyname = d->keyboard.layout->codes[code].mod[mods].name;

	}
	ev->dev = d;
	ev->keyname = strdup(keyname);
	
	return ev;
}

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
#if 0
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI int 
ecore_fb_input_device_module_set(Ecore_Fb_Input_Device *d, const char *name)
{
	Ecore_Fb_Module_Input *m;
	Ecore_Fb_Input_Device *tmp;

	if (!name || !d) return 0;
	
	m = ecore_fb_module_get(name);
	if (!m) return 0;


	/* TODO close the other module */
	if (d->module.module)
	{

	}
	/* open the new one */
	if (!m->open(d))
	{
		free(d);
		return 0;
	}
	d->module.module = m;
	return 1;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ecore_Fb_Input_Device *
ecore_fb_input_device_open(const char *dev, const char *module)
{
	Ecore_Fb_Input_Device *d;
	Ecore_Fb_Module_Input *m;

	if (!dev || !module) 
		return NULL;
		
	/* TODO check is the device name is a symbolink link,
	 * if so, use the linked path
	 */
	if (!ecore_file_exists(dev))
		return NULL;

	/* check if we already have an 
	 * input device with the same device
	 * path
	 */
	if (_devices)
	{
		Ecore_Fb_Input_Device *d_found;
		
		d_found = ecore_list_find(_devices, ECORE_COMPARE_CB(_devices_get_cb), dev);
		if (d_found)
		{
			/* if the module requested is different
			 * try to set it up, in any case return the
			 * device found
			 */
			if (strcmp(d_found->module.module->name, module))
				ecore_fb_input_device_module_set(d_found, module);
			return d_found;
		}
	}
	m = ecore_fb_module_get(module);
	if (!m) return NULL;

	d = _device_new(dev);
	
	if (!m->open(d))
	{
		free(d->info.device);
		free(d);
		return NULL;
	}
	d->module.module = m;

	if (!_devices)
		_devices = ecore_list_new();

	ecore_list_append(_devices, d);
	return d;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI const Ecore_List *
ecore_fb_input_device_get_list(void)
{
	return _devices;
}

/**
 * Gets the type of input events the device will send
 *
 */
EAPI int 
ecore_fb_input_device_type_get(Ecore_Fb_Input_Device *d)
{
	if (!d) return ECORE_FB_INPUT_DEVICE_TYPE_NONE;
	return d->info.type;
}

/**
 * Gets the name of the device
 *
 */
EAPI const char *
ecore_fb_input_device_name_get(Ecore_Fb_Input_Device *d)
{
	if (!d)
		return NULL;
	return d->info.name;
}

/**
 *
 *
 */

EAPI const char *
ecore_fb_input_device_device_get(Ecore_Fb_Input_Device *d)
{
	if (!d) return NULL;
	return d->info.device;
}

/**
 * Starts getting events from the input device 
 *
 */
EAPI void 
ecore_fb_input_device_enable(Ecore_Fb_Input_Device *d, int enable)
{
	if (!d) return;
	if (enable == d->enabled) return;

	if (d->module.module->enable)
		d->module.module->enable(d, enable);
}


/**
 *
 *
 *
 */
EAPI void
ecore_fb_input_device_close(Ecore_Fb_Input_Device *d)
{
	if (d->module.module->close)
		d->module.module->close(d);
	/* remove the element from the list */
	if (ecore_list_goto(_devices, d))
		ecore_list_remove(_devices);
	free(d);
}



/**
 *
 *
 */
EAPI void
ecore_fb_input_device_threshold_click_set(Ecore_Fb_Input_Device *dev, double threshold)
{
	if (!dev) return;
	if (threshold == dev->mouse.threshold || threshold == 0) return;
	dev->mouse.threshold = threshold;
}

/**
 *
 *
 */
EAPI double
ecore_fb_input_device_threshold_click_get(Ecore_Fb_Input_Device *dev)
{
	if (!dev) return 0;
	return dev->mouse.threshold;
}

/**
 *
 *
 */
EAPI void
ecore_fb_input_device_keyboard_layout_set(Ecore_Fb_Input_Device *dev, Ecore_Fb_Keyboard_Layout *l)
{
	if (!dev) return;
	if (dev->keyboard.layout);
	dev->keyboard.layout = l;
}

/*
 * If the device is a relative input device, 
 * we must set a width and height for it. If its
 * absolute set the ioctl correctly, if not, unsupported
 * device
 */
EAPI void
ecore_fb_input_device_axis_size_set(Ecore_Fb_Input_Device *dev, int w, int h)
{
	if(!dev)
		return;
	if(w < 0 || h < 0)
		return;
	/* FIXME 
	 * this code is for a touchscreen device, 
	 * make it configurable (ABSOLUTE | RELATIVE)
	 */
	if(dev->info.cap & ECORE_FB_INPUT_DEVICE_CAP_ABSOLUTE)
	{
		/* FIXME looks like some kernels dont include this struct */
		struct input_absinfo abs_features;

		ioctl(dev->fd, EVIOCGABS(ABS_X), &abs_features);
		dev->mouse.min_w = abs_features.minimum;
		dev->mouse.rel_w = (double)(abs_features.maximum - abs_features.minimum)/(double)(w);

		ioctl(dev->fd, EVIOCGABS(ABS_Y), &abs_features);
		dev->mouse.min_h = abs_features.minimum;
		dev->mouse.rel_h = (double)(abs_features.maximum - abs_features.minimum)/(double)(h);
	}
	else if(!(dev->info.cap & ECORE_FB_INPUT_DEVICE_CAP_RELATIVE))
		return;

	/* update the local values */
	if(dev->mouse.x > w - 1)
		dev->mouse.x = w -1;
	if(dev->mouse.y > h - 1)
		dev->mouse.y = h -1;
	dev->mouse.w = w;
	dev->mouse.h = h;
}

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
#define TEST_BIT(bit, array)    (array[bit/8] & (1<<(bit%8)))

#define CLICK_THRESHOLD_DEFAULT 0.25

static Ecore_List *_ecore_fb_li_devices = NULL;

static const char *_ecore_fb_li_kbd_syms[128 * 6] =
{
#include "ecore_fb_keytable.h"
};


static void 
_ecore_fb_li_event_free_key_down(void *data, void *ev)
{

	Ecore_Fb_Event_Key_Up *e;

	e = ev;
	free(e->keyname);
	if (e->keysymbol) free(e->keysymbol);
	if (e->key_compose) free(e->key_compose);
	free(e);
}

static void 
_ecore_fb_li_event_free_key_up(void *data, void *ev)
{

	Ecore_Fb_Event_Key_Up *e;

	e = ev;
	free(e->keyname);
	if (e->keysymbol) free(e->keysymbol);
	if (e->key_compose) free(e->key_compose);
	free(e);
}

static void
_ecore_fb_li_keyboard_modifiers_unset(int *mods, char *keyname)
{
	if (!strcmp(keyname, "Shift"))
	{
		*mods |= (1 << ECORE_FB_MOD_SHIFT);
	}
	else if (!strcmp(keyname, "AltGr"))
	{
		*mods |= (1 << ECORE_FB_MOD_ALTGR);
	}
	else if (!strcmp(keyname, "Control"))
	{
		*mods |= (1 << ECORE_FB_MOD_CONTROL);
	}
	else if (!strcmp(keyname, "Alt"))
	{
		*mods |= (1 << ECORE_FB_MOD_ALT);
	}
	else if (!strcmp(keyname, "ShiftL"))
	{
		*mods |= (1 << ECORE_FB_MOD_SHIFTL);
	}
	else if (!strcmp(keyname, "ShiftR"))
	{
		*mods |= (1 << ECORE_FB_MOD_SHIFTR);
	}
	else if (!strcmp(keyname, "CtrlL"))
	{
		*mods |= (1 << ECORE_FB_MOD_CONTROLL);
	}
	else if (!strcmp(keyname, "CtrlR"))
	{
		*mods |= (1 << ECORE_FB_MOD_CONTROLR);
	}
}

static void
_ecore_fb_li_keyboard_modifiers_set(int *mods, char *keyname)
{
	if (!strcmp(keyname, "Shift"))
	{
		*mods |= (1 << ECORE_FB_MOD_SHIFT);
	}
	else if (!strcmp(keyname, "AltGr"))
	{
		*mods |= (1 << ECORE_FB_MOD_ALTGR);
	}
	else if (!strcmp(keyname, "Control"))
	{
		*mods |= (1 << ECORE_FB_MOD_CONTROL);
	}
	else if (!strcmp(keyname, "Alt"))
	{
		*mods |= (1 << ECORE_FB_MOD_ALT);
	}
	else if (!strcmp(keyname, "ShiftL"))
	{
		*mods |= (1 << ECORE_FB_MOD_SHIFTL);
	}
	else if (!strcmp(keyname, "ShiftR"))
	{
		*mods |= (1 << ECORE_FB_MOD_SHIFTR);
	}
	else if (!strcmp(keyname, "CtrlL"))
	{
		*mods |= (1 << ECORE_FB_MOD_CONTROLL);
	}
	else if (!strcmp(keyname, "CtrlR"))
	{
		*mods |= (1 << ECORE_FB_MOD_CONTROLR);
	}
}

static void
_ecore_fb_li_keyboard_locks_toggle(int *locks, char *keyname)
{
	if (!strcmp(keyname, "Caps_Lock"))
	{
		*locks = !*locks;
	}
}

static void
_ecore_fb_li_keyboard_locks_set(int *locks, char *keyname)
{
	if (!strcmp(keyname, "Caps_Lock"))
	{
		*locks = 1;
	}
}

static void
_ecore_fb_li_device_event_key(Ecore_Fb_Input_Device *dev, struct input_event *iev)
{
	if (!dev->listen)
		return;

	/* check for basic keyboard keys */
	if (iev->code >= KEY_ESC && iev->code <= KEY_COMPOSE)
	{
		/* if we have a value is a down */
		if (iev->value)
		{
			Ecore_Fb_Event_Key_Down *ev = NULL;
			int mod;
			char *keyname;

//			ev = calloc(1, sizeof(Ecore_Fb_Event_Key_Down));

			mod = dev->keyboard.mods;
			keyname = dev->keyboard.layout->codes[iev->code].mod[mod].name;
			/* check the caps lock, if it set and the 
			 * key supports it we behave like a shift
			 * or ~shift in case shift is already pressed 
			 * if the key doesnt support it we use the
			 * actual keyname
			 */
			if (dev->keyboard.capslock && dev->keyboard.layout->codes[iev->code].mod[mod].letter)
			{
				/* invert the shift modifier */
				if (mod & (1 << ECORE_FB_MOD_SHIFT))
					mod &= ~(1 << ECORE_FB_MOD_SHIFT);
				else
					mod |= (1 << ECORE_FB_MOD_SHIFT);
				/* get the other keyname */
				keyname = dev->keyboard.layout->codes[iev->code].mod[mod].name;
			}
			if (keyname)
			{
				_ecore_fb_li_keyboard_modifiers_set(&dev->keyboard.mods, keyname);
				_ecore_fb_li_keyboard_locks_toggle(&dev->keyboard.mods, keyname);
			}
			printf("down %s\n", keyname);
#if 0
			//mod = _ecore_fb_li_keyboard_modifiers_get(&dev->keyboard.modifiers[0]);
			int offset = 0;

			ev = calloc(1, sizeof(Ecore_Fb_Event_Key_Down));
			if(dev->keyboard.shift) offset = 1;
			else if(dev->keyboard.lock) offset = 2;
			ev->keyname = strdup(_ecore_fb_li_kbd_syms[iev->code * 6]);

			ev->keysymbol = strdup(_ecore_fb_li_kbd_syms[(iev->code * 6) + offset]);
			ev->key_compose = strdup(_ecore_fb_li_kbd_syms[(iev->code * 6) + 3 + offset]);
			ev->dev = dev;
			ecore_event_add(ECORE_FB_EVENT_KEY_DOWN, ev, _ecore_fb_li_event_free_key_down, NULL);
			/* its a repeated key, dont increment */
			if(iev->value == 2)
				return;
			if (!strcmp(ev->keyname, "Control_L"))
				dev->keyboard.ctrl++;
			else if (!strcmp(ev->keyname, "Control_R"))
				dev->keyboard.ctrl++;
			else if (!strcmp(ev->keyname, "Alt_L"))
				dev->keyboard.alt++;
			else if (!strcmp(ev->keyname, "Alt_R"))
				dev->keyboard.alt++;
			else if (!strcmp(ev->keyname, "Shift_L"))
				dev->keyboard.shift++;
			else if (!strcmp(ev->keyname, "Shift_R"))
				dev->keyboard.shift++;
			else if (!strcmp(ev->keyname, "Caps_Lock"))
				dev->keyboard.lock++;
			if (dev->keyboard.ctrl > 2) dev->keyboard.ctrl = 2;
			if (dev->keyboard.alt > 2) dev->keyboard.alt = 2;
			if (dev->keyboard.shift > 2) dev->keyboard.shift = 2;
			if (dev->keyboard.lock > 1) dev->keyboard.lock = 1;
#endif
		}
		/* else is an up */
		else
		{
			Ecore_Fb_Event_Key_Up *ev;
			int mod;
			char *keyname;
			
			mod = dev->keyboard.mods;
			keyname = dev->keyboard.layout->codes[iev->code].mod[mod].name;
			if (dev->keyboard.capslock && dev->keyboard.layout->codes[iev->code].mod[mod].letter)
			{
				/* invert the shift modifier */
				if (mod & (1 << ECORE_FB_MOD_SHIFT))
					mod &= ~(1 << ECORE_FB_MOD_SHIFT);
				else
					mod |= (1 << ECORE_FB_MOD_SHIFT);
				/* get the other keyname */
				keyname = dev->keyboard.layout->codes[iev->code].mod[mod].name;
			}
			/* check modifiers, if they are set, unset them */
			if (keyname)
			{
				_ecore_fb_li_keyboard_modifiers_unset(&dev->keyboard.mods, keyname);
			}
			printf("up %s\n", keyname);
#if 0
			int offset = 0;

			ev = calloc(1, sizeof(Ecore_Fb_Event_Key_Up));
			if(dev->keyboard.shift) offset = 1;
			else if(dev->keyboard.lock) offset = 2;
			ev->keyname = strdup(_ecore_fb_li_kbd_syms[iev->code * 6]);

			ev->keysymbol = strdup(_ecore_fb_li_kbd_syms[(iev->code * 6) + offset]);
			ev->key_compose = strdup(_ecore_fb_li_kbd_syms[(iev->code * 6) + 3 + offset]);
			ev->dev = dev;
			ecore_event_add(ECORE_FB_EVENT_KEY_UP, ev, _ecore_fb_li_event_free_key_up, NULL);
			if (!strcmp(ev->keyname, "Control_L"))
				dev->keyboard.ctrl--;
			else if (!strcmp(ev->keyname, "Control_R"))
				dev->keyboard.ctrl--;
			else if (!strcmp(ev->keyname, "Alt_L"))
				dev->keyboard.alt--;
			else if (!strcmp(ev->keyname, "Alt_R"))
				dev->keyboard.alt--;
			else if (!strcmp(ev->keyname, "Shift_L"))
				dev->keyboard.shift--;
			else if (!strcmp(ev->keyname, "Shift_R"))
				dev->keyboard.shift--;
			else if (!strcmp(ev->keyname, "Caps_Lock"))
				dev->keyboard.lock--;
			if (dev->keyboard.ctrl < 0) dev->keyboard.ctrl = 0;
			if (dev->keyboard.alt < 0) dev->keyboard.alt = 0;
			if (dev->keyboard.shift < 0) dev->keyboard.shift = 0;
			if (dev->keyboard.lock < 0) dev->keyboard.lock = 0;
#endif
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
_ecore_fb_li_device_event_rel(Ecore_Fb_Input_Device *dev, struct input_event *iev)
{
	if(!dev->listen)
		return;
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
_ecore_fb_li_device_event_abs(Ecore_Fb_Input_Device *dev, struct input_event *iev)
{
	if(!dev->listen)
		return;
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
_ecore_fb_li_device_fd_callback(void *data, Ecore_Fd_Handler *fdh)
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
				_ecore_fb_li_device_event_abs(dev, &ev[i]);
				break;
			case EV_REL:
				_ecore_fb_li_device_event_rel(dev, &ev[i]);
				break;
			case EV_KEY:
				_ecore_fb_li_device_event_key(dev, &ev[i]);
				break;
			default:
				break;
		}
	}
	return 1;
}

/**
 * Starts getting events from the input device 
 *
 */
EAPI void 
ecore_fb_input_device_listen(Ecore_Fb_Input_Device *dev, int listen)
{
	if(!dev) return;
	if((listen && dev->listen) || (!listen && !dev->listen)) return;
	if(listen)
	{
		/* if the device already had a handler */
		if(!dev->handler)
			dev->handler = ecore_main_fd_handler_add(dev->fd, ECORE_FD_READ, _ecore_fb_li_device_fd_callback, dev, NULL, NULL);

	}
	dev->listen = listen;
}

/**
 * Opens an input device
 */
EAPI Ecore_Fb_Input_Device *
ecore_fb_input_device_open(const char *dev)
{
	Ecore_Fb_Input_Device *device;
	char event_type_bitmask[EV_MAX/8 + 1];
	int event_type;
	int fd;

	if(!dev) return NULL;
	device = calloc(1, sizeof(Ecore_Fb_Input_Device));
	if(!device) return NULL;

	if(!_ecore_fb_li_devices)
		_ecore_fb_li_devices = ecore_list_new();

	if((fd = open(dev, O_RDONLY, O_NONBLOCK)) < 0)
	{
		fprintf(stderr, "[ecore_fb_li:device_open] %s %s", dev, strerror(errno));
		goto error_open;
	}
	/* query capabilities */
	if(ioctl(fd, EVIOCGBIT(0, EV_MAX), event_type_bitmask) < 0)
	{
		fprintf(stderr,"[ecore_fb_li:device_open] query capabilities %s %s", dev, strerror(errno));
		goto error_caps;
	}
	/* query name */
	device->info.name = calloc(256, sizeof(char));
	if(ioctl(fd, EVIOCGNAME(sizeof(char) * 256), device->info.name) < 0)
	{
		fprintf(stderr, "[ecore_fb_li:device_open] get name %s %s", dev, strerror(errno));
		strcpy(device->info.name, "Unknown");
	}
	device->fd = fd;
	device->info.dev = strdup(dev);
	/* common */
	device->mouse.threshold = CLICK_THRESHOLD_DEFAULT;
	device->keyboard.layout = &kbd_layout_default;

	/* set info */
	for(event_type = 0; event_type < EV_MAX; event_type++)
	{
		if(!TEST_BIT(event_type, event_type_bitmask))
			continue;
		switch(event_type)
		{
			case EV_SYN :
			break;

			case EV_KEY:
			device->info.cap |= ECORE_FB_INPUT_DEVICE_CAP_KEYS_OR_BUTTONS;
			break;

			case EV_REL:
			device->info.cap |= ECORE_FB_INPUT_DEVICE_CAP_RELATIVE;
			break;

			case EV_ABS:
			device->info.cap |= ECORE_FB_INPUT_DEVICE_CAP_ABSOLUTE;
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
	ecore_list_append(_ecore_fb_li_devices, device);
	return device;

	error_caps:
	close(fd);
	error_open:
	free(device);
	return NULL;

}

/**
 *
 *
 *
 */
EAPI void
ecore_fb_input_device_close(Ecore_Fb_Input_Device *dev)
{
	/* close the fd */
	close(dev->fd);
	/* remove the element from the list */
	if(ecore_list_goto(_ecore_fb_li_devices, dev))
		ecore_list_remove(_ecore_fb_li_devices);
	free(dev);
}


/*
 * If the device is a relative input device, 
 * we must set a width and height for it. If its
 * absolute set the ioctl correctly, if not, unsupported
 * device
 */
EAPI void
ecore_fb_input_device_axis_size_set(Ecore_Fb_Input_Device *dev, int w, int h)
{
	if(!dev)
		return;
	if(w < 0 || h < 0)
		return;
	/* FIXME 
	 * this code is for a touchscreen device, 
	 * make it configurable (ABSOLUTE | RELATIVE)
	 */
	if(dev->info.cap & ECORE_FB_INPUT_DEVICE_CAP_ABSOLUTE)
	{
		/* FIXME looks like some kernels dont include this struct */
		struct input_absinfo abs_features;

		ioctl(dev->fd, EVIOCGABS(ABS_X), &abs_features);
		dev->mouse.min_w = abs_features.minimum;
		dev->mouse.rel_w = (double)(abs_features.maximum - abs_features.minimum)/(double)(w);

		ioctl(dev->fd, EVIOCGABS(ABS_Y), &abs_features);
		dev->mouse.min_h = abs_features.minimum;
		dev->mouse.rel_h = (double)(abs_features.maximum - abs_features.minimum)/(double)(h);
	}
	else if(!(dev->info.cap & ECORE_FB_INPUT_DEVICE_CAP_RELATIVE))
		return;

	/* update the local values */
	if(dev->mouse.x > w - 1)
		dev->mouse.x = w -1;
	if(dev->mouse.y > h - 1)
		dev->mouse.y = h -1;
	dev->mouse.w = w;
	dev->mouse.h = h;
}

/**
 *
 *
 */
EAPI const char *
ecore_fb_input_device_name_get(Ecore_Fb_Input_Device *dev)
{
	if (!dev)
		return NULL;
	return dev->info.name;
}

/**
 *
 *
 */
EAPI Ecore_Fb_Input_Device_Cap
ecore_fb_input_device_cap_get(Ecore_Fb_Input_Device *dev)
{
	if (!dev)
		return ECORE_FB_INPUT_DEVICE_CAP_NONE;
	return dev->info.cap;
}

/**
 *
 *
 */
EAPI void
ecore_fb_input_device_threshold_click_set(Ecore_Fb_Input_Device *dev, double threshold)
{
	if (!dev) return;
	if (threshold == dev->mouse.threshold || threshold == 0) return;
	dev->mouse.threshold = threshold;
}

/**
 *
 *
 */
EAPI double
ecore_fb_input_device_threshold_click_get(Ecore_Fb_Input_Device *dev)
{
	if (!dev) return 0;
	return dev->mouse.threshold;
}

/**
 *
 *
 */
EAPI void
ecore_fb_input_device_keyboard_layout_set(Ecore_Fb_Input_Device *dev, Ecore_Fb_Keyboard_Layout *l)
{
	if (!dev) return;
	if (dev->keyboard.layout);
	dev->keyboard.layout = l;
}
#endif
#endif
