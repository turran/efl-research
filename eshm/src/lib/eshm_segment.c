#include "Eshm.h"
#include "eshm_private.h"
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
struct _Eshm_Segment
{
	char *id;
	void *data;
	int shmid;
	Eina_Bool locked;
};

static void _error_to_eina(Eshm_Error err)
{
	int error = 0;
	
	switch (err)
	{			
		case ESHM_ERR_ACCESS:
			error = ESHM_ERROR_ACCESS;
			break;
		case ESHM_ERR_EXIST:
			error = ESHM_ERROR_EXIST;
			break;
		case ESHM_ERR_NEXIST:
			error = ESHM_ERROR_NEXIST;
			break;
		case ESHM_ERR_CODEC:
			error = ESHM_ERROR_CODEC;
			break;
	}
	if (error)
	{
		eina_error_set(error);
	}
}
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
	Eshm_Message_Segment_New m;
	Eshm_Reply_Segment_New *r;
	Eshm_Error error;
	
	m.id = id;
	m.size = size;
	
	/* send the message to the daemon */
	error = eshm_message_server_send(ESHM_MSG_TYPE_SEGMENT_NEW, &m, 0, (void **)&r);
	if (error)
	{
		_error_to_eina(error);
		EINA_ERROR_PWARN("Unable to create new segment with id = %s\n", id);
		return NULL;
	}
	/* allocate the new segment and give it back to user */
	s = calloc(1, sizeof(Eshm_Segment));
	
	s->shmid = r->shmid;
	s->id = strdup(id);
	s->data = shmat(s->shmid, NULL, 0);
	
	free(r);
	
	EINA_ERROR_PDBG("New segment of id %s created with numeric id %d\n", s->id, r->shmid);
	return s;
}
/**
 * 
 */
EAPI Eshm_Segment * eshm_segment_get(const char *id)
{
	Eshm_Segment *s;
	Eshm_Message_Segment_Get m;
	Eshm_Reply_Segment_Get *r;
	Eshm_Error error;
		
	m.id = id;
		
	/* send the message to the daemon */
	error = eshm_message_server_send(ESHM_MSG_TYPE_SEGMENT_GET, &m, 0, (void **)&r);
	if (error)
	{
		_error_to_eina(error);
		EINA_ERROR_PWARN("Unable to request a segment with id = %s\n", id);
		return NULL;
	}
	/* allocate the new segment and give it back to user */
	EINA_ERROR_PDBG("Requested segment id = %d\n", r->shmid);

	s = calloc(1, sizeof(Eshm_Segment));

	s->shmid = r->shmid;
	s->id = strdup(id);
	s->data = shmat(s->shmid, NULL, 0);

	free(r);

	return s;	
}
/**
 * 
 */
EAPI void eshm_segment_delete(Eshm_Segment *s)
{
	Eshm_Message_Segment_Delete m;
	
	assert(s);
	m.id = s->id;
	
	eshm_message_server_send(ESHM_MSG_TYPE_SEGMENT_DELETE, &m, 0, NULL);
	EINA_ERROR_PDBG("Segment with id \"%s\" deleted\n", s->id);
}
/**
 *  Locks the segment for read or write
 * 
 * Writes the data into the segment, skipping the size of the header
 * Every client that has requested the same will be notified when the new
 * data has been written.
 */
EAPI Eina_Bool eshm_segment_lock(Eshm_Segment *s, Eina_Bool write)
{
	Eshm_Message_Segment_Lock m;
	Eshm_Error error;

	assert(s);
	if (s->locked == EINA_TRUE)
		return EINA_TRUE;
	
	m.id = s->id;
	m.write = write;
	error = eshm_message_server_send(ESHM_MSG_TYPE_SEGMENT_LOCK, &m, 0, NULL);
	
	if (error)
	{
		_error_to_eina(error);
		EINA_ERROR_PWARN("Unable to lock segment with id \"%s\"\n", s->id);
		return EINA_FALSE;
	}
	else
	{
		EINA_ERROR_PDBG("Segment with id \"%s\" locked\n", s->id);
		s->locked = EINA_TRUE;
		return EINA_TRUE;
	}
}
/**
 *  Unlocks the segment for read or write
 */
EAPI void eshm_segment_unlock(Eshm_Segment *s)
{
	Eshm_Message_Segment_Unlock m;
	
	assert(s);
	if (s->locked != EINA_TRUE)
		return;
	
	m.id = s->id;
	eshm_message_server_send(ESHM_MSG_TYPE_SEGMENT_UNLOCK, &m, 0, NULL);
	EINA_ERROR_PDBG("Segment with id \"%s\" unlocked\n", s->id);
}
/**
 * Writes the header information into the segment. The signature is composed
 * of:
 */
EAPI int eshm_segment_header_set(Eshm_Segment *s, const char *signature, void *data)
{
	assert(s);
}
/**
 * 
 */
EAPI void * eshm_segment_data_get(Eshm_Segment *s)
{
	assert(s);
	
	if (s->locked != EINA_TRUE)
		return NULL;
	return s->data;
}
