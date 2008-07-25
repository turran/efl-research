#ifndef ESHM_H_
#define ESHM_H_

#include <Eina.h>

/* Possible API */

extern int ESHM_ERROR_ACCESS;  /* not enough permission for such request */
extern int ESHM_ERROR_EXIST;   /* a request for new segment that already exists */
extern int ESHM_ERROR_NEXIST;  /* a request for a segment that no longer exists */
extern int ESHM_ERROR_CODEC;   /* bad encoding or decoding */
extern int ESHM_ERROR_TIMEOUT; /* timeout error */

EAPI int eshm_init(void);
EAPI int eshm_shutdown(void);

typedef struct _Eshm_Segment Eshm_Segment;

EAPI Eshm_Segment * eshm_segment_new(const char *id, unsigned int size);
EAPI Eshm_Segment * eshm_segment_get(const char *id);
EAPI Eshm_Segment * eshm_segment_load(const char *id);
EAPI Eina_Bool eshm_segment_lock(Eshm_Segment *s, Eina_Bool write);
EAPI void eshm_segment_unlock(Eshm_Segment *s);
EAPI int eshm_segment_header_set(Eshm_Segment *s, const char *signature, void *data);
EAPI void * eshm_segment_data_get(Eshm_Segment *s);

#endif /*ESHM_H_*/
