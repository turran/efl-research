#include <stdlib.h>

#include "list.h"

static int _list_alloc_error = 0;

int list_alloc_error()
{
	return _list_alloc_error;
}

list_t *list_append(list_t *list, void *data)
{
	if (!data)
		return list;

	if (!list)
	{
		list = calloc(1, sizeof(list_t));
		list->prev = NULL;
		list->next = NULL;
		list->data = data;
		list->acct = calloc(1, sizeof(list_acct_t));
		list->acct->end = list;
		list->acct->count = 1;
	}
	else
	{
		list->acct->end->next = calloc(1, sizeof(list_t));
		list->acct->end->next->next = NULL;
		list->acct->end->next->prev = list->acct->end;
		list->acct->end->next->data = data;
		list->acct->end->next->acct = list->acct;
		list->acct->end = list->acct->end->next;
		list->acct->count++;
	}

	return list;
}

int list_count(list_t *list)
{
	if (!list || !list->acct)
		return 0;

	return list->acct->count;
}

list_t *list_delete(list_t *list, void *data)
{
	list_t *l;

	if (!list || !data)
		return list;

	for (l = list; l; l = l->next)
	{
		if (l->data == data)
			return list_delete_list(list, l);
	}
	return list;	
}

list_t *list_delete_list(list_t *list, list_t *remove_list)
{
	list_t *return_l;

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

void *list_get_at(list_t *list, int place)
{
	list_t *l;

	l = list_get_list_at(list, place);
	return l ? l->data : NULL;
}

list_t *list_get_list_at(list_t *list, int place)
{
	int i;
	const list_t *l;

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
			if (i == place) return (list_t *)l;
		}
	}
	else
	{
		for (i = 0, l = list; l; l = l->next, i++)
		{
			if (i == place) return (list_t *)l;
		}
	}
	return NULL;
}

void *list_replace_at(list_t *list, int place, void *data)
{
	int i;
	list_t *l;

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

list_t *list_prepend(list_t *list, const void *data)
{
	list_t *new_l;

	_list_alloc_error = 0;
	new_l = calloc(1, sizeof(list_t));
	if (!new_l)
	{
		_list_alloc_error = 1;
		return list;
	}
	new_l->prev = NULL;
	new_l->data = (void *)data;
	if (!list)
	{
		new_l->next = NULL;
		new_l->acct = calloc(1, sizeof(list_acct_t));
		if (!new_l->acct)
		{
			_list_alloc_error = 1;
			free(new_l);
			return list;
		}
		new_l->acct->end = new_l;
		new_l->acct->count = 1;
		return new_l;
	}
	new_l->next = list;
	list->prev = new_l;
	new_l->acct = list->acct;
	list->acct->count++;
	return new_l;
}

list_t *list_remove(list_t *list, const void *data)
{
	list_t *l;

	for (l = list; l; l = l->next)
	{
		if (l->data == data)
			return list_remove_list(list, l);
	}
	return list;
}

list_t *list_remove_list(list_t *list, list_t *remove_list)
{
	list_t *return_l;

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

