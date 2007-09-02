#ifndef _DATA_H
#define _DATA_H

typedef struct _Equis_List      Equis_List;
struct _Equis_List
{
   Equis_List  *next, *prev;
   Equis_List  *last;
};

/* Object List */
void *equis_list_append           (void *in_list, void *in_item);
void *equis_list_prepend          (void *in_list, void *in_item);
void *equis_list_append_relative  (void *in_list, void *in_item, void *in_relative);
void *equis_list_prepend_relative (void *in_list, void *in_item, void *in_relative);
void *equis_list_remove           (void *in_list, void *in_item);
void *equis_list_find             (void *in_list, void *in_item);

#endif
