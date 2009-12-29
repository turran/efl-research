#include "Eshm.h"
#include "eshm_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
Eet_Data_Descriptor_Class _eddc = {
	.version = EET_DATA_DESCRIPTOR_CLASS_VERSION,
	.name = "eshm",
};

Eet_Data_Descriptor *_descriptors[ESHM_MSG_NAMES];

static Eshm_Message * _new(Eshm_Message_Type type)
{
	static int id = 0;
	Eshm_Message *m;

	m = malloc(sizeof(Eshm_Message));
	m->id = id;
	m->type = type;
	/* TODO this will cause an overflow sometime */
	id++;
	return m;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
inline Eshm_Message_Name eshm_message_name_get(Eshm_Message_Type t)
{
	return (t & ~1) >> 1;
}

inline Eina_Bool eshm_message_reply_has(Eshm_Message_Type t)
{
	if (t & ESHM_MSG_REPLY)
		return EINA_TRUE;
	else
		return EINA_FALSE;
}
/**
 * Given a message type return the name of the reply. Note that the reply's
 * name is always the message plus one.
 */
inline Eina_Bool eshm_message_reply_name_get(Eshm_Message_Type t, Eshm_Message_Name *n)
{
	if (eshm_message_reply_has(t) == EINA_FALSE)
		return EINA_FALSE;
	*n = eshm_message_name_get(t) + 1;
	return EINA_TRUE;
}


void eshm_message_init(void)
{
	Eet_Data_Descriptor *edd;
	/* create all the messages' data descriptors */

	/* segment new */
	edd = eet_data_descriptor2_new(&_eddc);
	_descriptors[ESHM_MSG_NAME_SEGMENT_NEW] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Message_Segment_New, "id", id, EET_T_INLINED_STRING);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Message_Segment_New, "size", size, EET_T_UINT);
	/* segment new reply */
	edd = eet_data_descriptor2_new(&_eddc);
	_descriptors[ESHM_MSG_NAME_SEGMENT_NEWR] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Reply_Segment_New, "shmid", shmid, EET_T_UINT);
	/* segment get */
	edd = eet_data_descriptor2_new(&_eddc);
	_descriptors[ESHM_MSG_NAME_SEGMENT_GET] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Message_Segment_Get, "id", id, EET_T_INLINED_STRING);
	/* segment get reply */
	edd = eet_data_descriptor2_new(&_eddc);
	_descriptors[ESHM_MSG_NAME_SEGMENT_GETR] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Reply_Segment_Get, "shmid", shmid, EET_T_UINT);
	/* segment lock */
	edd = eet_data_descriptor2_new(&_eddc);
	_descriptors[ESHM_MSG_NAME_SEGMENT_LOCK] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Message_Segment_Lock, "id", id, EET_T_INLINED_STRING);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Message_Segment_Lock, "write", write, EET_T_UCHAR);
	/* segment unlock */
	edd = eet_data_descriptor2_new(&_eddc);
	_descriptors[ESHM_MSG_NAME_SEGMENT_UNLOCK] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Message_Segment_Lock, "id", id, EET_T_INLINED_STRING);
	/* segment delete */
	edd = eet_data_descriptor2_new(&_eddc);
	_descriptors[ESHM_MSG_NAME_SEGMENT_DELETE] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Message_Segment_Delete, "id", id, EET_T_INLINED_STRING);
}

void eshm_message_shutdown(void)
{
	int i;

	/* remove all the messages's data descriptors */
	for (i = 0; i < ESHM_MSG_NAMES; i++)
		eet_data_descriptor_free(_descriptors[i]);
}

void * eshm_message_encode(Eshm_Message_Name name, const void *data, int *size)
{
	return eet_data_descriptor_encode(_descriptors[name], data, size);
}

void * eshm_message_decode(Eshm_Message_Name name, const void *data, int size)
{
	return eet_data_descriptor_decode(_descriptors[name], data, size);
}

Eshm_Error eshm_message_server_send(Eshm_Message_Type type, void *data, double timeout, void **rdata)
{
	Eshm_Message *m;
	void *body;

	m = _new(type);
	body = eshm_message_encode(eshm_message_name_get(m->type), data, &m->size);
	if (!body)
		return ESHM_ERR_CODEC;

	return eshm_server_send(m, body, timeout, rdata);
}

#if 0
#define EET_SIG_CHAR             'C'
#define EET_SIG_SHORT            'S'
#define EET_SIG_INT              'I'
#define EET_SIG_LONG_LONG        'L'
#define EET_SIG_FLOAT            'f'
#define EET_SIG_DOUBLE           'D'
#define EET_SIG_UCHAR            'c'
#define EET_SIG_USHORT           's'
#define EET_SIG_UINT             'i'
#define EET_SIG_ULONG_LONG       'l'
#define EET_SIG_STRING           'T'
#define EET_SIG_INLINED_STRING   'N'

#define EET_SIG_ARRAY_START   '['
#define EET_SIG_ARRAY_END     ']'
#define EET_SIG_LIST_START    '('
#define EET_SIG_LIST_END      ')'
#define EET_SIG_HASH_START    '{'
#define EET_SIG_HASH_END      '}'


EAPI int eet_descriptor_new(Eet_Data_Descriptor_Class *c, const char *signature)
{
	Eet_Descriptor *eet;
	char *tmp = signature;

	if (!signature)
		return NULL;
	eet = eet_descriptor2_new(c);
	/* parse the signature */
	while (*tmp)
	{
		switch (*tmp)
		{
			//case
		}
		tmp++;
	}

	return eet;
}
#endif

