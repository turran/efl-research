#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <Evas_Data.h>
#include <Ecore_Data.h>

/* to run this with the new lib just use:
 *  LD_PRELOAD=./lib.so ./ee_test
 */

void append_element(Ecore_DList *list)
{
	char data[256];
	static char num = 0;

	sprintf(data, "element %d", num);
	ecore_dlist_append(list, strdup(data));

	num++;
}


int main(void)
{
	Ecore_DList *list = NULL;
	char *data;

	list = ecore_dlist_new();

	append_element(list);	
	append_element(list);	

	ecore_dlist_first_goto(list);
	data = ecore_dlist_current(list);
	if (data)
		printf("%s\n", data);
	append_element(list);	
	data = ecore_dlist_current(list);
	if (data)
		printf("%s\n", data);
	data = ecore_dlist_last_goto(list);
	if (data)
		printf("%s\n", data);
	data = ecore_dlist_index_goto(list, 1);
	if (data)
		printf("%s\n", data);
	data = ecore_dlist_next(list);
	if (data)
		printf("%s\n", data);
	data = ecore_dlist_previous(list);
	if (data)
		printf("%s\n", data);

	return 1;
}

