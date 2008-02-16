#ifndef _CODE_CLIST_H
#define _CODE_CLIST_H

typedef struct _CList_Acct CList_Acct;
typedef struct _CList CList;

struct _CList_Acct
{
	int count;
	CList *end;
};

struct _CList
{
	CList *prev;
	CList *next;
	void *data;
	CList_Acct *acct;
};

CList *clist_append(CList *list, void *data);
int clist_count(CList *list);
CList *clist_delete(CList *list, void *data);
CList *clist_delete_list(CList *list, CList *remove_list);
void *clist_get_at(CList *list, int place);
CList *clist_get_list_at(CList *list, int place);
void *clist_replace_at(CList *list, int place, void *data);

#endif
