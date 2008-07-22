#include "Eshm.h"
#include "eshm_private.h"

/**
 * TODO
 * Add a buffer for the message received
 */


typedef struct _Eshmd
{
	Ecore_Con_Server *srv;
	void *buffer;
	int length;
} Eshmd;

Eshmd _eshmd;

static key_t _key_new(void)
{
	static num; 	
	char path[PATH_MAX];
	char *tmp;
	
	/* get the tmp dir */
	if (!(tmp = getenv("TMPDIR")))
	{
		tmp = strdup("/tmp");
	}
	snprintf(path, PATH_MAX, "%s/eshmd%d", tmp, num);
	return ftok(path, 'e');
}


int _segment_new(Eshm_Message_Segment_New *sn, void **reply)
{
	Eshm_Reply_Segment_New *rsn;
	struct shmid_ds ds;
	key_t key;
	
	printf("requesting segment new of name %s\n", sn->id);
	/* TODO check if the segment already exists on the hash of segments */
	*reply = calloc(1, sizeof(Eshm_Reply_Segment_New));
	rsn = *reply;
	/* if not, create a new one */
	key = _key_new();
	rsn->shmid = shmget(key, sn->size, IPC_CREAT | 0);
	if (rsn->shmid < 0)
	{
		printf("error getting the segment of name %s [%s] with key %d\n", sn->id, strerror(errno), key);
	}
	/* set the permissions */
	shmctl(rsn->shmid, IPC_STAT, &ds);
	ds.shm_perm.mode = 0666;
	shmctl(rsn->shmid, IPC_SET, &ds);
	printf("New segment created with id %d\n", rsn->shmid);
	
	
	return 0;
}

int _segment_lock(void)
{
	
}


int _client_add(void *data, int type, void *event)
{
	printf("client add\n");
}

int _client_del(void *data, int type, void *event)
{
	printf("client del\n");
}

int _client_data(void *data, int type, void *event)
{
	Ecore_Con_Event_Client_Data *cdata = event;
	Eshm_Message *m;
	void *body;
	void *reply = NULL;
	unsigned int m_length;
	int ret;

	/* check if we got a full message */
	if (!_eshmd.buffer)
	{
		_eshmd.buffer = cdata->data;
		_eshmd.length = cdata->size;
		cdata->data = NULL;
	}
	else
	{
		_eshmd.buffer = realloc(_eshmd.buffer, _eshmd.length + cdata->size);
		memcpy(((unsigned char *)_eshmd.buffer) + _eshmd.length, cdata->data, cdata->size);
		_eshmd.length += cdata->size;
		cdata->data = NULL;
	}
	if (_eshmd.length < sizeof(Eshm_Message)) 
		return 0;
	/* ok, we have at least a message header */
	m = _eshmd.buffer;
	m_length = sizeof(Eshm_Message) + m->size;
	if (_eshmd.length < m_length)
		return 0;
	/* parse the header */
	printf("client message of type %d with msg num %d of size %d\n", m->type, m->id, m->size);
	
	body = eshm_message_decode(eshm_message_name_get(m->type), (unsigned char *)m + sizeof(Eshm_Message), m->size);
	if (!body)
	{
		printf("Error Decoding\n");
		/* TODO check if the message needed a reply and if so
		 * send it the error number */
		goto shift;
	}
	/* check for the reply */
	switch (m->type)
	{
		case ESHM_MSG_TYPE_SEGMENT_NEW:
			ret = _segment_new(body, &reply);
			break;
		case ESHM_MSG_TYPE_SEGMENT_LOCK:
		case ESHM_MSG_TYPE_SEGMENT_UNLOCK:
		case ESHM_MSG_TYPE_SEGMENT_DELETE:
			break;
	}
shift:
	/* free in case we have served a complete message */
	if (_eshmd.length > m_length)
	{
		unsigned char *tmp;
		unsigned int n_length;
		
		tmp = _eshmd.buffer;
		n_length = _eshmd.length - m_length;
		
		_eshmd.buffer = malloc(n_length);
		memcpy(_eshmd.buffer, tmp + m_length, n_length);
		free(tmp);
	}
	if (reply)
	{
		Eshm_Reply r;
		Eshm_Message_Name rname;
		void *rbody;
		
		printf("Sending reply\n");
		r.id = m->id;
		/* TODO handle the error */
		//r.error = ret;
		eshm_message_reply_name_get(m->type, &rname);
		printf("message encoded %d %d\n", rname, eshm_message_name_get(m->type));
		rbody = eshm_message_encode(rname, reply, &r.size);
		ecore_con_client_send(cdata->client, &r, sizeof(Eshm_Reply));
		ecore_con_client_send(cdata->client, rbody, r.size);
	}
	return 0;
}


int main(int argc, char **argv)
{
	ecore_init();
	ecore_con_init();
	
	_eshmd.buffer = NULL;
	_eshmd.srv = ecore_con_server_add(ECORE_CON_LOCAL_USER, ESHMD_NAME, ESHMD_PORT, NULL);
	if (!_eshmd.srv)
	{
		printf("error\n");
	}
	ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_ADD, _client_add, NULL);
	ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_DEL, _client_del, NULL);
	ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_DATA, _client_data, NULL);
	
	eshm_message_init();
	ecore_main_loop_begin();
	eshm_message_shutdown();
	ecore_con_shutdown();
	ecore_shutdown();
		
	return 0;
}

