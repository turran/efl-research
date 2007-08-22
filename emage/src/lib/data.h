#ifndef _DATA_H
#define _DATA_H

/* FIXME rename this
 */
typedef struct _Evas_Object_List      Evas_Object_List;
struct _Evas_Object_List
{
   Evas_Object_List  *next, *prev;
   Evas_Object_List  *last;
};

/* Object List */
void *evas_object_list_append           (void *in_list, void *in_item);
void *evas_object_list_prepend          (void *in_list, void *in_item);
void *evas_object_list_append_relative  (void *in_list, void *in_item, void *in_relative);
void *evas_object_list_prepend_relative (void *in_list, void *in_item, void *in_relative);
void *evas_object_list_remove           (void *in_list, void *in_item);
void *evas_object_list_find             (void *in_list, void *in_item);

#endif
