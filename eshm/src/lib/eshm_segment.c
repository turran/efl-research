#include "Eshm.h"
#include "eshm_private.h"
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
struct _Eshm_Segment
{
	void *data;
};
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * Creates a new segment
 * This function is a blocking function, it will block until the request is done
 */
EAPI Eshm_Segment * eshm_segment_new(const char *id, unsigned int size)
{
	Eshm_Segment *s;
	Eshm_Message_Segment_New msg_sn;
	Eshm_Reply_Segment_New *r;
	Eshm_Error error;
	
	msg_sn.id = id;
	msg_sn.size = size;
	
	/* send the message to the daemon */
	error = eshm_message_server_send(ESHM_MSG_TYPE_SEGMENT_NEW, &msg_sn, 0, &r);
	/* allocate the new segment and give it back to user */
	printf("segment id = %d\n", r->shmid);
	s = calloc(1, sizeof(Eshm_Segment));
	s->data = shmat(r->shmid, NULL, 0);
	printf("data allocated at %p\n", s->data);
	return s;
}
/**
 *  Locks the segment for read or write
 */
EAPI int eshm_segment_lock(Eshm_Segment *s)
{
	
}
/**
 *  Unlocks the segment for read or write
 */
EAPI int eshm_segment_unlock(Eshm_Segment *s)
{
	
}
/**
 * Writes the header information into the segment. The signature is composed
 * of:
 */
EAPI int eshm_segment_header_set(Eshm_Segment *s, const char *signature, void *data)
{
	
}
/**
 * Writes the data into the segment, skipping the size of the header
 * Every client that has requested the same will be notified when the new
 * data has been written.
 */
EAPI int eshm_segment_data_set(Eshm_Segment *s, void *data)
{
	
}
