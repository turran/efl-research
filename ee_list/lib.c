#include <stdio.h>
#include <stdlib.h>

#include <Evas.h>

//typedef Evas_List *Ecore_DList;

typedef struct _Ecore_DList
{
	Evas_List *curr;
	Evas_List *first;
} Ecore_DList;

/*============================================================================*
 *              Creating and initializing new list structures                 *
 *============================================================================*/
Ecore_DList * ecore_dlist_new()
{
	Ecore_DList *l;

	l = malloc(sizeof(Ecore_DList));
	l->curr = NULL;
	l->first = NULL;
	//printf("list new %p %p\n", l, *l);

	return l;
}

int ecore_dlist_init(Ecore_DList *list)
{

}

void ecore_dlist_destroy(Ecore_DList *list)
{

}

/*============================================================================*
 *                        Adding items to the list                            *
 *============================================================================*/
int ecore_dlist_append(Ecore_DList *list, void *data)
{
	list->first = evas_list_append(list->first, data);
	//printf("list append first = %p curr = %p\n", list->first, list->curr);

	return 1;
}

int ecore_dlist_prepend(Ecore_DList * list, void *_data)
{

}

int ecore_dlist_insert(Ecore_DList * list, void *_data)
{

}

int ecore_dlist_append_list(Ecore_DList * list, Ecore_DList * append)
{


}

int ecore_dlist_prepend_list(Ecore_DList * list, Ecore_DList * prepend)
{


}



void * ecore_dlist_current(Ecore_DList *list)
{
	//printf("current %p\n", list);

	if (list->curr)
		return list->curr->data;
	else 
		return NULL;
}

/*============================================================================*
 *                             Traversing the list                            *
 *============================================================================*/
void * ecore_dlist_first_goto(Ecore_DList *list)
{
	//printf("first goto %p\n", el);
	
	list->curr = list->first;
	if (list->curr)
		return list->curr->data;
	else
		return NULL;
}
   
void * ecore_dlist_last_goto(Ecore_DList * list)
{
	list->curr = evas_list_last(list->first);
	if (list->curr)
		return list->curr->data;
	else
		return NULL;

}

void *ecore_dlist_index_goto(Ecore_DList * list, int index)
{
	list->curr = evas_list_nth_list(list->first, index);
	if (list->curr)
		return list->curr->data;
	else
		return NULL;
}


void *ecore_dlist_goto(Ecore_DList * list, void *data)
{
	list->curr = evas_list_find_list(list->first, data);
	if (list->curr)
		return list->curr->data;
	else
		return NULL;
}
   
/*============================================================================*
 *                  Traversing the list and returning data                    *
 *============================================================================*/
void *ecore_dlist_next(Ecore_DList * list)
{
	void *data = NULL;

	if (list->curr)
	{
		data = list->curr->data;
		list->curr = evas_list_next(list->curr);
	}
	return data;
}

void *ecore_dlist_previous(Ecore_DList * list)
{
	void *data = NULL;

	if (list->curr)
	{
		data = list->curr->data;
		list->curr = evas_list_prev(list->curr);
	}
	return data;

}


