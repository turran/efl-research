#ifndef ESHM_H_
#define ESHM_H_

#include <Eina.h>

/* Possible API */
EAPI int eshm_init(void);
EAPI int eshm_shutdown(void);


typedef struct _Eshm_Segment Eshm_Segment;

EAPI Eshm_Segment * eshm_segment_new(const char *id, unsigned int size);
EAPI int eshm_segment_lock(Eshm_Segment *s);
EAPI int eshm_segment_unlock(Eshm_Segment *s);
EAPI int eshm_segment_header_set(Eshm_Segment *s, const char *signature, void *data);
EAPI int eshm_segment_data_set(Eshm_Segment *s, void *data);

#endif /*ESHM_H_*/
