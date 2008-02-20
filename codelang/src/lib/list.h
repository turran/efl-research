#ifndef _CODELANG_LIST_H
#define _CODELANG_LIST_H

typedef struct list_acct_s list_acct_t;
typedef struct list_s list_t;

struct list_acct_s
{
	int count;
	list_t *end;
};

struct list_s
{
	list_t *prev;
	list_t *next;
	void *data;
	list_acct_t *acct;
};

list_t *list_append(list_t *list, void *data);
int list_count(list_t *list);
list_t *list_delete(list_t *list, void *data);
list_t *list_delete_list(list_t *list, list_t *remove_list);
void *list_get_at(list_t *list, int place);
list_t *list_get_list_at(list_t *list, int place);
void *list_replace_at(list_t *list, int place, void *data);

#endif
