#include "Eshm.h"
#include "eshm_private.h"

/**
 * TODO
 * Add a buffer for the message received
 */
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
typedef struct _Eshm
{
	Ecore_Con_Server *svr;
	Eshm_Message *msg; /* last message sent */
	Eshm_Reply *reply; /* reply for the last message sent */
	void *rbody; /* decoded body of the reply message */
	unsigned char *buffer; /* buffer where the data received from server is stored */
	unsigned int length; /* length of the buffer */
} Eshm;

static Eshm _eshm;

static int _init = 0;

static int _server_data(void *data, int type, void *event)
{
	Ecore_Con_Event_Server_Data *e = event;
	Eshm_Message_Name rname;
	unsigned int m_length;
	
	if (!_eshm.msg)
	{
		EINA_ERROR_PERR("How do we receive a reply with no msg first??\n");
		return 0;
	}
	if (!_eshm.buffer)
	{
		_eshm.buffer = e->data;
		_eshm.length = e->size;
		e->data = NULL;
	}
	else
	{
		_eshm.buffer = realloc(_eshm.buffer, _eshm.length + e->size);
		memcpy(((unsigned char *)_eshm.buffer) + _eshm.length, e->data, e->size);
		_eshm.length += e->size;
		e->data = NULL;
	}
	if (_eshm.length < sizeof(Eshm_Reply))
		return 0;
	
	_eshm.reply = (Eshm_Reply *)_eshm.buffer;
	m_length = sizeof(Eshm_Reply) + _eshm.reply->size;
	if (_eshm.length < m_length)
		return 0;
	
	/* ok we have a full message */
	if (_eshm.reply->size)
	{
		eshm_message_reply_name_get(_eshm.msg->type, &rname);
		_eshm.rbody = eshm_message_decode(rname, _eshm.buffer + sizeof(Eshm_Reply), _eshm.reply->size);
	}
	/* copy the reply into a new buffer */
	_eshm.reply = malloc(sizeof(Eshm_Reply));
	memcpy(_eshm.reply, _eshm.buffer, sizeof(Eshm_Reply));
	/* realloc the tmp buffer */
	if (_eshm.length > m_length)
	{
		unsigned char *tmp;
		unsigned int n_length;
			
		tmp = _eshm.buffer;
		n_length = _eshm.length - m_length;
			
		_eshm.buffer = malloc(n_length);
		memcpy(_eshm.buffer, tmp + m_length, n_length);
		free(tmp);
	}
	else
	{
		free(_eshm.buffer);
		_eshm.buffer = NULL;
	}

	return 0;
}

static int _timeout_cb(void *data)
{
	printf("timer!!!\n");
	return 0;
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
/**
 * Sends a message to the server
 * 
 * The message is encoded and then sent to the server, if the message needs
 * a reply this function will wait @timeout ms for it. The encoding and decoding
 * of the message is transparent.
 *  
 * @param mhdr A completely message to send to the server
 * @param timeout Number of ms to wait for the reply in case of needed
 * @param rhdr The reply will be stored here
 * @return
 * 
 * TODO the reply can be embedded on the header type itself?
 * TODO Encode the message with eet!!
 */
Eshm_Error eshm_server_send(Eshm_Message *m, void *data, double timeout, void **rdata)
{
	Eshm_Error error;
	Ecore_Timer *timer;
	int ret;
	
	_eshm.msg = m;
	
	EINA_ERROR_PDBG("Sending message of type %d and id %d\n", m->type, m->id);
	ret = ecore_con_server_send(_eshm.svr, m, sizeof(Eshm_Message));
	ret = ecore_con_server_send(_eshm.svr, data, m->size);
	
	if (eshm_message_reply_has(m->type) == EINA_FALSE)
		return ESHM_ERR_NONE;
	
	/* TODO register a timeout callback */
	if (timeout)
		timer = ecore_timer_add(timeout, _timeout_cb, NULL);
	/* wait for the response */
	while (!_eshm.reply)
	{
		ecore_main_loop_iterate();
	}
	if (timeout)
		ecore_timer_del(timer);

	EINA_ERROR_PDBG("Finished lock reply of type %d\n", _eshm.reply->id);
	
	error = _eshm.reply->error;
	
	if (rdata)
		*rdata = _eshm.rbody;
	free(_eshm.reply);
	free(_eshm.msg);
	_eshm.msg = NULL;
	_eshm.reply = NULL;
	
	return error;
}
void eshm_common_init(void)
{	
	ecore_init();
	ecore_con_init();
	eshm_message_init();
}
void eshm_common_shutdown(void)
{
	eshm_message_shutdown();
	ecore_con_shutdown();
	ecore_shutdown();
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
int ESHM_ERROR_ACCESS;
int ESHM_ERROR_EXIST;
int ESHM_ERROR_NEXIST;
int ESHM_ERROR_CODEC;
int ESHM_ERROR_TIMEOUT;
/**
 *  Initialize the library
 */
EAPI int eshm_init(void)
{
	if (!_init)
	{	
		eina_error_init();
		ecore_init();
		ecore_con_init();
		eshm_message_init();
		/* try to connect to the daemon */
		_eshm.svr = ecore_con_server_connect(ECORE_CON_LOCAL_USER, ESHMD_NAME, ESHMD_PORT, NULL);
		if (!_eshm.svr)
		{
			EINA_ERROR_PERR("The server does not exist\n");
			eshm_message_shutdown();
			ecore_con_shutdown();
			ecore_shutdown();
			eina_error_shutdown();
			return 0;
		}
		ecore_event_handler_add(ECORE_CON_EVENT_SERVER_DATA, _server_data, NULL);
		ESHM_ERROR_ACCESS = eina_error_register("Not enough permission for the request");
		ESHM_ERROR_EXIST = eina_error_register("Segment already exists");
		ESHM_ERROR_NEXIST = eina_error_register("Segment does not exist");
		ESHM_ERROR_CODEC = eina_error_register("Encoding / Decoding failed");
		ESHM_ERROR_TIMEOUT = eina_error_register("Timeout waiting for response");
	}
	return ++_init;
	
}
/**
 * Shutdowns the library
 */
EAPI int eshm_shutdown(void)
{
	if (!--_init)
	{
		eshm_message_shutdown();
		ecore_con_shutdown();
		ecore_shutdown();
	}
	return _init;
}