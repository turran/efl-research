/**
 * input api
 *
 */
#include <string.h>

#include "Ecore_Data.h"
#include "Ecore_File.h"
#include "Ecore_Fb.h"
#include "ecore_fb_private.h"
/* default keyboard layout */
#include "ecore_fb_input_default_layout.h"


Ecore_List *_devices = NULL;
/* ==== Local ==== */
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

/* ==== API ==== */

/**
 *
 *
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
 * Opens an input device
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

EAPI const Ecore_List *
ecore_fb_input_device_get_list(void)
{
	return _devices;
}

/**
 *
 * Gets the module used by this input device
 *
 */
EAPI const char * 
ecore_fb_input_device_module_get(Ecore_Fb_Input_Device *d)
{
	if (!d) return NULL;
	
	return ecore_fb_module_name_get(d->module.module);
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

#if 0
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
#endif
