#ifndef _ECORE_LI_PRIVATE_H
#define _ECORE_LI_PRIVATE_H

#include <assert.h>
#include <linux/input.h>

#include "kbd_layout.h" 	// Default keyboard layout
#define CLICK_THRESHOLD 0.25 	// Default double click threshold

enum
{
	ECORE_FB_MOD_PLAIN,
	ECORE_FB_MOD_SHIFT,
	ECORE_FB_MOD_ALTGR,
	ECORE_FB_MOD_CONTROL,
	ECORE_FB_MOD_ALT,
	ECORE_FB_MOD_SHIFTL,
	ECORE_FB_MOD_SHIFTR,
	ECORE_FB_MOD_CONTROLL,
	ECORE_FB_MOD_CONTROLR,
	ECORE_FB_MODS
};

struct _Ecore_Li_Device
{
	int fd;
	Ecore_Fd_Handler *handler;
	int enabled;
	struct {
		int caps;
		char *name;
		char *dev;
	} info;
	struct
	{
		/* common mouse */
		int x,y;
		int w,h;
		
		double last;
		double prev;
		double threshold;
		/* absolute axis */
		int min_w, min_h;
		double rel_w, rel_h;

	} mouse;
	struct
	{
		Ecore_Li_Keyboard_Layout *layout;
		int mods;
		int capslock;
	} keyboard;
};

int device_fd_callback(void *data, Ecore_Fd_Handler *fdh);



#endif
