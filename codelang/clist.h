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
CList *clist_delete(CList *list, void *data);
CList *clist_delete_list(CList *list, CList *remove_list);

#endif
