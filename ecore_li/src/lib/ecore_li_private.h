#ifndef _ECORE_LI_PRIVATE_H
#define _ECORE_LI_PRIVATE_H

struct _Ecore_Li_Device
{
	int fd;
	Ecore_Fd_Handler *handler;
	int listen;
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
	} keyboard;
};

#endif
