#include <stdlib.h>

#include "Object.h"
#include "clist.h"

CList *clist_append(CList *list, void *data)
{
	if (!data)
		return list;

	if (!list)
	{
		list = calloc(1, sizeof(CList));
		list->prev = NULL;
		list->next = NULL;
		list->data = data;
		list->acct = calloc(1, sizeof(CList_Acct));
		list->acct->end = list;
		list->acct->count = 1;
	}
	else
	{
		list->acct->end->next = calloc(1, sizeof(CList));
		list->acct->end->next->next = NULL;
		list->acct->end->next->prev = list->acct->end;
		list->acct->end->next->data = data;
		list->acct->end->next->acct = list->acct;
		list->acct->end = list->acct->end->next;
		list->acct->count++;
	}

	return list;
}

int clist_count(CList *list)
{
	if (!list || !list->acct)
		return 0;

	return list->acct->count;
}

CList *clist_delete(CList *list, void *data)
{
	CList *l;

	if (!list || !data)
		return list;

	for (l = list; l; l = l->next)
	{
		if (l->data == data)
			return clist_delete_list(list, l);
	}
	return list;	
}

CList *clist_delete_list(CList *list, CList *remove_list)
{
	CList *return_l;

	if (!list) return NULL;
	if (!remove_list) return list;
	if (remove_list->next) remove_list->next->prev = remove_list->prev;
	if (remove_list->prev)
	{
		remove_list->prev->next = remove_list->next;
		return_l = list;
	}
	else
		return_l = remove_list->next;

	if (remove_list == list->acct->end)
		list->acct->end = remove_list->prev;

	list->acct->count--;
	if (list->acct->count == 0)
		free(list->acct);
	free(remove_list);
	return return_l;
}

void *clist_get_at(CList *list, int place)
{
	CList *l;

	l = clist_get_list_at(list, place);
	return l ? l->data : NULL;
}

CList *clist_get_list_at(CList *list, int place)
{
	int i;
	const CList *l;

	/* check for non-existing nodes */
	if ((!list) || (place < 0) ||
			(place > (list->acct->count - 1)))
		return NULL;

	/* if the node is in the 2nd half of the list, search from the end
	 * else, search from the beginning.
	 */
	if (place > (list->acct->count / 2))
	{
		for (i = list->acct->count - 1,
				l = list->acct->end;
				l;
				l = l->prev, i--)
		{
			if (i == place) return (CList *)l;
		}
	}
	else
	{
		for (i = 0, l = list; l; l = l->next, i++)
		{
			if (i == place) return (CList *)l;
		}
	}
	return NULL;
}

void *clist_replace_at(CList *list, int place, void *data)
{
	int i;
	CList *l;

	/* check for non-existing nodes */
	if ((!list) || (place < 0) ||
			(place > (list->acct->count - 1)))
		return NULL;

	/* if the node is in the 2nd half of the list, search from the end
	 * else, search from the beginning.
	 */
	if (place > (list->acct->count / 2))
	{
		for (i = list->acct->count - 1,
				l = list->acct->end;
				l;
				l = l->prev, i--)
		{
			if (i == place) 
			{
				void *tmp = l->data;
				l->data = data;
				return tmp;
			}
		}
	}
	else
	{
		for (i = 0, l = list; l; l = l->next, i++)
		{
			if (i == place) 
			{
				void *tmp = l->data;
				l->data = data;
				return tmp;
			}
		}
	}
	return NULL;
}

