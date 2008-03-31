#ifndef EINA_INLIST_H_
#define EINA_INLIST_H_

/**
 * @defgroup Inline_List_Group Inline List
 * @{
 */
typedef struct _Eina_Inlist Eina_Inlist;

struct _Eina_Inlist
{
   Eina_Inlist *next;
   Eina_Inlist *prev;
   Eina_Inlist *last;
};

EAPI void * eina_inlist_append(void *in_list, void *in_item);
EAPI void * eina_inlist_prepend(void *in_list, void *in_item);
EAPI void * eina_inlist_append_relative(void *in_list, void *in_item, void *in_relative);
EAPI void * eina_inlist_prepend_relative(void *in_list, void *in_item, void *in_relative);
EAPI void * eina_inlist_remove(void *in_list, void *in_item);
EAPI void * eina_inlist_find(void *in_list, void *in_item);

/** } */
#endif /*EINA_INLIST_H_*/
