#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

#include <Ecore.h>
#include <Ecore_Data.h>
#include <E_DBus.h>

#include "Eshm.h"


void _element_new_cb(void *mdata, void *udata)
{
	Eshm_Element *e = mdata;
	void *data;

	printf("NEW ELEMENT!!!!\n");
	data = eshm_element_data_get(e);
	printf("element at %s with size %d at address %p\n",
		eshm_element_name_get(e), eshm_element_size_get(e),
		data);
	printf("%c\n", data);
}

void _segment_new_cb(void *mdata, void *udata)
{
	Eshm_Segment *s = mdata;

	printf("NEW SEGMENT!!!!\n");
	printf("segment at %s with size %d!!!\n",
		eshm_segment_name_get(s), eshm_segment_size_get(s));
#define ELEMENT_FILE "/usr/share/ewl/images/e-logo.png"
#define ELEMENT_NOT_FOUND "/usr/share/etk/images/e_logo.png"
	eshm_element_new_from_file(s, ELEMENT_FILE, &_element_new_cb, NULL);
}

int main(int argc, char **argv)
{
	Ecore_List *s;

	ecore_init();
	eshm_init();

#define SIZE 1024 * 1024
	/* create a new segment */
	eshm_segment_new("/org/e/shm/Cache", SIZE, &_segment_new_cb, NULL);
	ecore_main_loop_begin();

	eshm_shutdown();
	ecore_shutdown();
	return 0;
}
