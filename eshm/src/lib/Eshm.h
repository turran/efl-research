#ifndef _ESHM_H
#define _ESHM_H

typedef struct _Eshm Eshm;
typedef struct _Eshm_Segment Eshm_Segment;
typedef struct _Eshm_Element Eshm_Element;

typedef void (*Eshm_Callback) (void *method_data, void *user_data);

/* TODO error types and status */
int 	eshm_init(void);
void 	eshm_shutdown(void);

E_DBus_Connection * eshm_manager_connection_get(void);

void 	eshm_segment_new(char *path, int size, Eshm_Callback cb, void *data);
char * 	eshm_segment_name_get(Eshm_Segment *s);
int 	eshm_segment_size_get(Eshm_Segment *s);

void 	eshm_element_new_from_file(Eshm_Segment *s, char *file, Eshm_Callback cb, void *data);
void 	eshm_element_new(Eshm_Segment *s, int size, Eshm_Callback cb, void *data)
void * 	eshm_element_data_get(Eshm_Element *e);
char * 	eshm_element_name_get(Eshm_Element *e);
void * 	eshm_element_size_get(Eshm_Element *e);

#endif
