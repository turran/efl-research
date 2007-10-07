#include <Edata.h>
#include <Ecore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <linux/input.h>

#include "Ecore_Li.h"
#include "ecore_li_private.h"
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static int _count = 0;
Edata_List *_devices = NULL;
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI int ecore_li_init(void)
{
	if(!_count)
        {
		ECORE_LI_EVENT_KEY_DOWN = ecore_event_type_new();
		ECORE_LI_EVENT_KEY_UP = ecore_event_type_new();
		ECORE_LI_EVENT_BUTTON_UP = ecore_event_type_new();
		ECORE_LI_EVENT_BUTTON_DOWN = ecore_event_type_new();
		ECORE_LI_EVENT_MOUSE_MOVE = ecore_event_type_new();
		ECORE_LI_EVENT_MOUSE_WHEEL = ecore_event_type_new();
	}
	return ++_count;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ecore_li_shutdown(void)
{
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ecore_Li_Device *ecore_li_device_new(const char *path)
{
	Ecore_Li_Device *device;
	char event_type_bitmask[EV_MAX/8 + 1];
	int event_type;
	int fd;

	if (!path) return NULL;
	device = calloc(1, sizeof(Ecore_Li_Device));
	if (!device) return NULL;

	if (!_devices)
		_devices = edata_list_new();

	if ((fd = open(path, O_RDONLY, O_NONBLOCK)) < 0)
	{
		goto error_open;
	}
	/* query capabilities */
	if (ioctl(fd, EVIOCGBIT(0, EV_MAX), event_type_bitmask) < 0)
	{
		goto error_caps;
	}
	/* query name */
	device->info.name = calloc(256, sizeof(char));
	if (ioctl(fd, EVIOCGNAME(sizeof(char) * 256), device->info.name) < 0)
	{
		strcpy(device->info.name, "Unknown");
	}
	device->fd = fd;
	device->info.dev = strdup(path);
	/* common */
	//device->mouse.threshold = CLICK_THRESHOLD_DEFAULT;
	//device->keyboard.layout = &kbd_layout_default;

	/* set info */
	for (event_type = 0; event_type < EV_MAX; event_type++)
	{
		if (!TEST_BIT(event_type, event_type_bitmask))
			continue;
		switch (event_type)
		{
			case EV_SYN:
			break;

			case EV_KEY:
			device->info.caps |= ECORE_LI_DEVICE_CAP_KEYS_OR_BUTTONS;
			break;

			case EV_REL:
			device->info.caps |= ECORE_LI_DEVICE_CAP_RELATIVE;
			break;

			case EV_ABS:
			device->info.caps |= ECORE_LI_DEVICE_CAP_ABSOLUTE;
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
	edata_list_append(_devices, device);
	return device;
error_caps:
	close(fd);
error_open:
	free(device);
	return NULL;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ecore_li_device_listen(Ecore_Li_Device *dev, int listen)
{
	if(!dev) return;
	if((listen && dev->listen) || (!listen && !dev->listen)) return;
	if(listen)
	{
		/* if the device already had a handler */
		if(!dev->handler)
			//dev->handler = ecore_main_fd_handler_add(dev->fd, ECORE_FD_READ, _ecore_fb_li_device_fd_callback, dev, NULL, NULL);
	}
}
