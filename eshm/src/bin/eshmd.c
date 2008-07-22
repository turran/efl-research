#include "Eshm.h"
#include "eshm_private.h"

/* TODO check if this structure can be shared with the one at the library */
typedef struct _Eshmd_Segment
{
	int shmid;
	int ref;
	Eina_Bool locked; /* TODO differentiate between a read lock and a write lock */
	Ecore_Con_Client *owner;
} Eshmd_Segment;

typedef struct _Eshmd_Client
{
	Eina_List *segments;
} Eshmd_Client;

typedef struct _Eshmd
{
	Eina_Hash *hash;
	Eina_List *clients;
	
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

/* Protocol message handlers */
static Eshm_Error msg_segment_new(Ecore_Con_Client *c, Eshm_Message_Segment_New *sn, void **reply)
{
	Eshmd_Segment *s;
	Eshm_Reply_Segment_New *rsn;
	struct shmid_ds ds;
	key_t key;
	
	printf("requesting segment new of name %s\n", sn->id);
	
	/* check if the segment already exists on the hash of segments */
	s = eina_hash_find(_eshmd.hash, sn->id);
	if (s)
	{
		printf("Segment found\n");
		return ESHM_ERR_EXIST;
	}
	/* create a new segment */
	*reply = calloc(1, sizeof(Eshm_Reply_Segment_New));
	rsn = *reply;
	
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
		
	s = calloc(1, sizeof(Eshmd_Segment));
	s->shmid = rsn->shmid;
	s->ref++;
	_eshmd.hash = eina_hash_add(_eshmd.hash, sn->id, s);
	
	
	printf("New segment created with id %d\n", rsn->shmid);
	
	return ESHM_ERR_NONE;
}

static Eshm_Error msg_segment_get(Ecore_Con_Client *c, Eshm_Message_Segment_Get *sn, void **reply)
{
	Eshmd_Segment *s;
	Eshm_Reply_Segment_Get *rsn;
	
	printf("requesting segment new of name %s\n", sn->id);
	
	/* check if the segment already exists on the hash of segments */
	s = eina_hash_find(_eshmd.hash, sn->id);
	if (!s)
	{
		printf("Segment not found\n");
		return ESHM_ERR_NEXIST;
	}
	s->ref++;
	/* get a new segment */
	*reply = calloc(1, sizeof(Eshm_Reply_Segment_New));
	rsn = *reply;
	rsn->shmid = s->shmid;

	return ESHM_ERR_NONE;
}

static int msg_segment_delete(Ecore_Con_Client *c, Eshm_Message_Segment_Delete *m, void **reply)
{
	/* decrement the segment reference count */
	/* delete the segment */
	return ESHM_ERR_NONE;
}

static int msg_segment_lock(Ecore_Con_Client *c, Eshm_Message_Segment_Lock *m, void **reply)
{
	Eshmd_Segment *s;
	
	/* lock the segment */
	printf("locking the segment\n");
	s = eina_hash_find(_eshmd.hash, m->id);
	if (!s)
		return ESHM_ERR_NEXIST;
	else if (m->write && c != s->owner)
		return ESHM_ERR_ACCESS;
	/* if the request was for a write and the client is not the owner of
	 * the segment, return an error */
	return ESHM_ERR_NONE;
}

static int msg_segment_unlock(Ecore_Con_Client *c, Eshm_Message_Segment_Unlock *m, void **reply)
{
	/* unlock the segment */
	printf("unlocking the segment\n");
	return ESHM_ERR_NONE;
}

int _client_add(void *data, int type, void *event)
{
	Ecore_Con_Event_Client_Add *e = event;
	Eshmd_Client *c;
	
	printf("client add\n");
	c = calloc(1, sizeof(Eshmd_Client));
	ecore_con_client_data_set(e->client, c);
}

int _client_del(void *data, int type, void *event)
{
	Ecore_Con_Event_Client_Add *e = event;
	Eshmd_Client *c;
	
	printf("client del\n");
	c = ecore_con_client_data_get(e->client);
	/* TODO unref all the segments */
}

int _client_data(void *data, int type, void *event)
{
	Ecore_Con_Event_Client_Data *cdata = event;
	Eshm_Message *m;
	void *body;
	void *reply = NULL;
	unsigned int m_length;
	Eshm_Error err;

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
	/* TODO replace this with an array of function pointers */
	switch (m->type)
	{
		case ESHM_MSG_TYPE_SEGMENT_NEW:
			err = msg_segment_new(cdata->client, body, &reply);
			break;
		case ESHM_MSG_TYPE_SEGMENT_LOCK:
			err = msg_segment_lock(cdata->client, body, &reply);
			break;
		case ESHM_MSG_TYPE_SEGMENT_UNLOCK:
			err = msg_segment_unlock(cdata->client, body, &reply);
			break;
		case ESHM_MSG_TYPE_SEGMENT_GET:
			err = msg_segment_get(cdata->client, body, &reply);
			break;
		case ESHM_MSG_TYPE_SEGMENT_DELETE:
			break;
	}
shift:
	if (eshm_message_reply_has(m->type) == EINA_TRUE)
	{
		Eshm_Reply r;
		Eshm_Message_Name rname;
		void *rbody;
		
		printf("Sending reply\n");
		r.id = m->id;
		r.error = err;
		eshm_message_reply_name_get(m->type, &rname);
		printf("message encoded %d %d\n", rname, eshm_message_name_get(m->type));
		if (reply)
			rbody = eshm_message_encode(rname, reply, &r.size);	
		else
			r.size = 0;
		ecore_con_client_send(cdata->client, &r, sizeof(Eshm_Reply));
		if (r.size)
			ecore_con_client_send(cdata->client, rbody, r.size);
	}
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
	else
	{
		free(_eshmd.buffer);
		_eshmd.buffer = NULL;
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

