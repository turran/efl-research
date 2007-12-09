#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>

#include <Ecore.h>
#include <Ecore_Data.h>
#include <Ecore_File.h>
#include <E_DBus.h>

#include <Emm.h>
/* TODO
 * - use the limits for shm (SHMMIN, etc. get them from proc/sys/kernel)
 * - replace list iterators with new one (s = ecore_list_next());
 * - for this hanshake between the reading/writing processes there might
 *   be some kind DoS attack :)
 * - signals for reading / writing
 * - create a log or verbose output
 */

struct manager
{
	Ecore_List 		*segments;
	E_DBus_Connection 	*conn;
	E_DBus_Object 		*o;
};

struct segment
{
	struct 	manager *m;
	
	Emm 		*manager;

	E_DBus_Object 	*o;
	Ecore_List 	*elements;
	Ecore_List	*paths;
	/* well known dbus object name */
	char 		*name;
	/* size in bytes */
	int 		size;
	int 		shmid;
	int 		ref;
	void 		*data;
};


struct element
{
	struct 	segment *segment;
	
	E_DBus_Object 	*o;
	/* well known dbus object name */
	char 		*name;
	void 		**data;
	/* size in bytes */
	int 		size;
	/* offset from segment data start */
	int 		offset;

	/* if someone is writing */
	int 		writing; 	
	/* flag that disables more readers in case someone requests a write */
	int 		to_write;	
	/* if someone is reading */
	int 		reading; 	
	
	/* 
	 * the connection that created the object
	 * in case of writeable 
	 */
	char 		*creator;

	/* for a file we need the path to reuse this element */
	/* FIXME, what about other element types ? */
	char 		*path;
};

enum
{
	ELEMENT_MODE_READABLE,
	ELEMENT_MODE_WRITEABLE
};

static struct element * element_new(struct segment *, const char *, int, int);
static struct segment * segment_new(struct manager *, const char *, int);

/* shmElement interface
 *
 *
 */

/* When a process requests a write, only the connection that request the element
 * is able to do that, so other connections calling this method will fail.
 * We have to check that all readers finished reading. If any process is
 * reading, append the write.
 */
DBusMessage *
shmelement_element_write_start(E_DBus_Object *obj, DBusMessage *msg)
{
	DBusMessage *r;
	
	struct element *e;
	
	e = e_dbus_object_data_get(obj);
	r = dbus_message_new_method_return(msg);
	if (e->writing)
		goto reply;

	/* check if the sending process is the same as the creator */
	if (!(e->creator && !strcmp(e->creator, dbus_message_get_sender(msg))))
	{
		goto reply;	
	}
	/* someone is reading, so queue the writing */
	if (e->reading)
	{
		e->to_write = 1;
		goto reply;
	}
	/* nobody reading, correct sender */
	e->writing = 1;
reply:
	return r;
}

DBusMessage *
shmelement_element_write_end(E_DBus_Object *obj, DBusMessage *msg)
{
	DBusMessage *r;
	
	struct element *e;
	
	e = e_dbus_object_data_get(obj);
	r = dbus_message_new_method_return(msg);
	return r;
}

/* if the write flag is enabled, dont allow more clients to read
 *
 */

DBusMessage *
shmelement_element_read_start(E_DBus_Object *obj, DBusMessage *msg)
{
	DBusMessage *r;
	
	struct element *e;
	
	e = e_dbus_object_data_get(obj);
	r = dbus_message_new_method_return(msg);
	if (e->writing || e->to_write)
	{
	
	}
	else
	{
		e->reading++;
	}
	return r;
}

DBusMessage *
shmelement_element_read_end(E_DBus_Object *obj, DBusMessage *msg)
{
	DBusMessage *r;
	
	struct element *e;
	
	e = e_dbus_object_data_get(obj);
	r = dbus_message_new_method_return(msg);
	if (e->writing)
	{

	}
	else
	{
		e->reading--;
	}
	return r;
}

/* shmSegment interface
 *
 *
 */
DBusMessage *
shmsegment_path_append(E_DBus_Object *obj, DBusMessage *msg)
{
	DBusError e;
	DBusMessage *r;
	struct segment *s;
	char *path;
	char *p;

	s = e_dbus_object_data_get(obj);
	r = dbus_message_new_method_return(msg);
	/* parse the parameters */
	memset(&e, 0, sizeof(DBusError));
	dbus_message_get_args(msg, &e, DBUS_TYPE_STRING,
		&path, DBUS_TYPE_INVALID);
	/* check if the path is already appended */
	ecore_list_first_goto(s->paths);
	p = ecore_list_current(s->paths);
	while (p)
	{
		if (!strcmp(p, path))
			break;
		ecore_list_next(s->paths);
		p = ecore_list_current(s->paths);
	}
	/* append it at the end */
	if (!p)
	{
		ecore_list_first_goto(s->paths);
		ecore_list_append(s->paths, strdup(path)); 
	}
	return r;
}

/*
 * When a client process creates a new element, that process
 * *must* write the data to it, using the functions to 
 * tell the daemon that the data is being written or that
 * the write process is finished. Then the daemon should send
 * the appropiate signals to other process. So all the communication
 * goes through the daemon, we should find a way to make reader process
 * access the data *directly* from the writer process (peer to peer?).
 *
 * TODO
 * merge this function and shmsegment_element_load_from_file
 * to reuse some code.
 */

DBusMessage *
shmsegment_element_new(E_DBus_Object *obj, DBusMessage *msg)
{
	DBusError err;
	DBusMessage *r;
	struct segment *s;
	struct element *e;
	
	int size;
	char *name;

	int offset;
	
	s = e_dbus_object_data_get(obj);
	r = dbus_message_new_method_return(msg);
	/* parse the parameters */
	memset(&err, 0, sizeof(DBusError));
	dbus_message_get_args(msg, &err, DBUS_TYPE_INT32,
		&size, DBUS_TYPE_INVALID);
	dbus_message_get_args(msg, &err, DBUS_TYPE_STRING,
		&name, DBUS_TYPE_INVALID);
	printf("requesting an element with name %s of size %d\n", name, size);
	
	e = element_new(s, name, size, ELEMENT_MODE_READABLE);
	e->creator = dbus_message_get_sender(msg);
	/* error allocating */
	if (!e)
	{
		offset = -1;
		goto reply;
	}
reply:
	return r;
}

/* check if the file is already cached */
static inline struct element * _file_cached(struct segment *s, char *path)
{
	struct element *e;

	printf("%p\n", s);
	ecore_list_first_goto(s->elements);
	while ((e = ecore_list_next(s->elements)))
	{
		if (!strcmp(e->path, path))
			break;
	}
	if (e)
		printf("file already cached!!!!\n");
	else
		printf("not cached!\n");
	return e;
}

/* get the real path of a file */
static inline char * _file_path_get(struct segment *s, char *path)
{
	char *p;

	/* get the real path from a file */
	/* absolute path */
	if (*path == '/')
	{
		if (!ecore_file_exists(path))
			goto error;
		else
			p = strdup(path);
	}
	/* relative path */
	else
	{
		char real_path[PATH_MAX];
		
		ecore_list_first_goto(s->paths);
		while ((p = ecore_list_next(s->paths)))
		{
			snprintf(real_path, PATH_MAX, "%s/%s", p, path);
			if (ecore_file_exists(p))
				break;
		}
		if (!p)
		{
			goto error;
		}
		p = strdup(real_path);
	}
	printf("file found at %s\n", p);
	return p;
error:
	printf("file doesnt exist\n");
	return NULL;
}

/* Loads a file from the file system and stores it on the shm segment
 * Returns the offset where the data is and its size, if error
 * it will return -1 as the offset and the size
 * When an element is written by the daemon, the client process
 * should wait for a signal to start reading the element and *must not*
 * write to it.
 */
DBusMessage *
shmsegment_element_new_from_file(E_DBus_Object *obj, DBusMessage *msg)
{
	struct segment *s;
	struct element *e;
	
	DBusError err;
	DBusMessage *r;
	
	char *path;
	
	s = e_dbus_object_data_get(obj);
	r = dbus_message_new_method_return(msg);
	/* parse the parameters */
	memset(&err, 0, sizeof(DBusError));
	dbus_message_get_args(msg, &err, DBUS_TYPE_STRING,
		&path, DBUS_TYPE_INVALID);
	/* get the real path */
	path = _file_path_get(s, path);
	/* FIXME what happens if the file isnt found ? */
	if (!path)
		return NULL;
	/* check if the file is already cached */
	e = _file_cached(s, path);
	if (!e)
	{
		FILE *f;
		struct stat st;
		char name[PATH_MAX];

		/* create a new element ? */
		f = fopen(path, "r");
		stat(path, &st);
		
		snprintf(name, PATH_MAX, "%s/%d", s->name, s->elements->nodes);
		e = element_new(s, name, st.st_size, ELEMENT_MODE_READABLE);
		e->path = path;
		/* file ok, write the data into a new segment element */
		fread(*e->data, st.st_size, 1, f);
	}
	/* get the offset from the segment start */
	dbus_message_append_args(r, 
		DBUS_TYPE_OBJECT_PATH, &e->name,
		DBUS_TYPE_INT32, &e->offset,
		DBUS_TYPE_INT32, &e->size,
		DBUS_TYPE_INVALID);
	return r;
}

/* For every element on the list of elements from
 * a segment return its object's name and the size
 */
DBusMessage *
shmsegment_element_list(E_DBus_Object *obj, DBusMessage *msg)
{
	return NULL;
}

/* shmManager interface
 * 
 *
 */

/* For every shm segment on the list of shares
 * return its object's name and the size
 */
DBusMessage *
shmmanager_segment_list(E_DBus_Object *obj, DBusMessage *msg)
{
	DBusMessage *r;
	DBusMessageIter it, it_array, it_struct;
	struct manager *m;
	struct segment *s;
	
	m = e_dbus_object_data_get(obj);
	r = dbus_message_new_method_return(msg);
	
	dbus_message_iter_init_append(r, &it);
	dbus_message_iter_open_container(&it, DBUS_TYPE_ARRAY, 
		DBUS_STRUCT_BEGIN_CHAR_AS_STRING
		DBUS_TYPE_OBJECT_PATH_AS_STRING
		DBUS_TYPE_INT32_AS_STRING
		DBUS_TYPE_INT32_AS_STRING
		DBUS_STRUCT_END_CHAR_AS_STRING,
		&it_array);

	ecore_list_first_goto(m->segments);
	s = ecore_list_current(m->segments);
	while (s)
	{
		ecore_list_next(m->segments);
		dbus_message_iter_open_container(&it_array, DBUS_TYPE_STRUCT,
			DBUS_TYPE_OBJECT_PATH_AS_STRING
			DBUS_TYPE_INT32_AS_STRING, 
			&it_struct);
		dbus_message_iter_append_basic(&it_struct, DBUS_TYPE_OBJECT_PATH, &(s->name));
		dbus_message_iter_append_basic(&it_struct, DBUS_TYPE_INT32, &s->size);
		dbus_message_iter_append_basic(&it_struct, DBUS_TYPE_INT32, &s->shmid);
		
		dbus_message_iter_close_container(&it_array, &it_struct);
		s = ecore_list_current(m->segments);
	}
	dbus_message_iter_close_container(&it, &it_array);
	return r;
}

/* If a client wants to create a segment this function
 * will create the new shm segment in case it doesnt exist
 * or request it if it is already created
 */
DBusMessage *
shmmanager_segment_new(E_DBus_Object *obj, DBusMessage *msg)
{
	struct segment *s;
	struct manager *m;
	
	DBusError e;
	DBusMessage *r;
	
	int size;
	char *name;
	
	m = e_dbus_object_data_get(obj);
	/* parse the parameters */
	memset(&e, 0, sizeof(DBusError));
	dbus_message_get_args(msg, &e,
		DBUS_TYPE_OBJECT_PATH, &name,
		DBUS_TYPE_INT32, &size, 
		DBUS_TYPE_INVALID);
	/* TODO check the size of the segment */
	r = dbus_message_new_method_return(msg);

	/* check if the segment was already created */
	ecore_list_first_goto(m->segments);
	s = ecore_list_current(m->segments);
	while (s)
	{
		if (!strcmp(s->name, name))
			goto reply;
		ecore_list_next(m->segments);
		s = ecore_list_current(m->segments);
	}
	/* create the new segment */
	s = segment_new(m, name, size);
reply:
	/* for both cases (new / requested) increase the reference count */
	s->ref++;
	dbus_message_append_args(r, DBUS_TYPE_INT32, &(s->size),
		DBUS_TYPE_INVALID);
	dbus_message_append_args(r, DBUS_TYPE_INT32, &(s->shmid),
		DBUS_TYPE_INVALID);
	return r;
}


struct manager *
eshmd_init(void)
{
	E_DBus_Interface *i;
	struct manager *m;

	m = calloc(1, sizeof(struct manager));
	if (!m) goto ecalloc;

	m->conn = e_dbus_bus_get(DBUS_BUS_SESSION);
	if (!m->conn) goto ebusget;

	m->segments = ecore_list_new();
	if (!m->segments) goto elistnew;

	e_dbus_request_name(m->conn, "org.e.shm", 0, NULL, NULL);
	m->o = e_dbus_object_add(m->conn, "/org/e/shm/Manager", m);
	/* shmManager */
	i = e_dbus_interface_new("org.e.shm.manager");
	e_dbus_interface_method_add(i, "ListSegments", NULL,
		DBUS_TYPE_ARRAY_AS_STRING 
		DBUS_STRUCT_BEGIN_CHAR_AS_STRING
		DBUS_TYPE_OBJECT_PATH_AS_STRING /* name */
		DBUS_TYPE_INT32_AS_STRING 	/* size */
		DBUS_TYPE_INT32_AS_STRING 	/* shmid */
		DBUS_STRUCT_END_CHAR_AS_STRING,
		shmmanager_segment_list);
	e_dbus_interface_method_add(i, "NewSegment",
		DBUS_TYPE_OBJECT_PATH_AS_STRING /* name */
		DBUS_TYPE_INT32_AS_STRING, 	/* size */
		DBUS_TYPE_INT32_AS_STRING	/* real size */
		DBUS_TYPE_INT32_AS_STRING, 	/* shmid */
		shmmanager_segment_new);
	e_dbus_object_interface_attach(m->o, i);
	return m;

elistnew:
	//e_dbus_connection_unref(m->conn);
ebusget:
	free(m);
ecalloc:
	return NULL;
}

/* element */
static struct element * element_new(struct segment *s, const char *n, int size, int mode)
{
	E_DBus_Interface *i;
	struct element *e;

	e = calloc(1, sizeof(struct element));
	if (!e) return NULL;

	e->o = e_dbus_object_add(s->m->conn, n, e);
	e->name = strdup(n);
	e->data = emm_malloc(s->manager, size);
	/* error allocating */
	if (!e->data || !*e->data)
	{
		free(e);
		return NULL;
	}
	/* FIX THIS! */
	e->size = size;
	e->offset = (int)*e->data - (int)s->data;
	e->segment = s;
	ecore_list_append(s->elements, e);
	
	i = e_dbus_interface_new("org.e.ipc.readable");
	//e_dbus_interface_method_add(i, "Attach", NULL, NULL, shmelement_element_request);
	//e_dbus_interface_method_add(i, "Detach", NULL, NULL, shmelement_element_request);
	e_dbus_object_interface_attach(e->o, i);

	if (mode == ELEMENT_MODE_WRITEABLE)
	{
		i = e_dbus_interface_new("org.e.shm.writeable");
		e_dbus_interface_method_add(i, "StartWrite", NULL, NULL, shmelement_element_write_start);
		e_dbus_interface_method_add(i, "EndWrite", NULL, NULL, shmelement_element_write_end);
		e_dbus_object_interface_attach(e->o, i);
	}
	
	return e;
}

static inline key_t _key_new(void)
{
	char path[PATH_MAX];
	int ret;
	char *tmp;

	/* get the tmp dir */
	if (!(tmp = getenv("TMPDIR")))
	{
		tmp = strdup("/tmp");
	}
	snprintf(path, PATH_MAX, "%s/eshmdXXXXXX", tmp);
	ret = mkstemp(path);
	printf("ret = %d %s\n", ret, path);
	return ftok(path, 'e');
}

/* segment */
static struct segment * segment_new(struct manager *m, const char *n, int size)
{
	E_DBus_Interface *i;
	struct segment *s;

	key_t key;
	struct shmid_ds ds;

	printf("NEW SEGMENT OF NAME %s and SIZE %d\n", n, size);
	s = calloc(1, sizeof(struct segment));
	if (!s) return NULL;

	s->size = size;
	s->name = strdup(n);
	s->m = m;
	ecore_list_append(m->segments, s);
	/* create the new shmSegment object */
	s->o = e_dbus_object_add(m->conn, s->name, s);
	i = e_dbus_interface_new("org.e.shm.segment");
#if 0
	e_dbus_interface_method_add(i, "ListElements",
		NULL,
		DBUS_TYPE_OBJECT_PATH_AS_STRING /* element object */
		DBUS_TYPE_INT32_AS_STRING,
		shmsegment_element_new);
#endif
	e_dbus_interface_method_add(i, "NewElement",
		DBUS_TYPE_INT32_AS_STRING, 	/* size */
		DBUS_TYPE_OBJECT_PATH_AS_STRING /* element object */
		DBUS_TYPE_INT32_AS_STRING,
		shmsegment_element_new);
	e_dbus_interface_method_add(i, "NewElementFromFile", 
		DBUS_TYPE_STRING_AS_STRING, 	/* file path */
		DBUS_TYPE_OBJECT_PATH_AS_STRING /* element object */
		DBUS_TYPE_INT32_AS_STRING	/* offset */
		DBUS_TYPE_INT32_AS_STRING,	/* size */
		shmsegment_element_new_from_file);
	e_dbus_object_interface_attach(s->o, i);
	/* create a new segment with the size passed as
	 * an argument of the message. Returns a new 
	 * message with the shmid as first argument
	 */
	key = _key_new(); 
	s->shmid = shmget(key, size, IPC_CREAT | 0);
	if (s->shmid < 0)
	{
		printf("error getting the segment of name %s [%s] with key %d\n", s->name, strerror(errno), key);
	}
	/* set the permissions */
	shmctl(s->shmid, IPC_STAT, &ds);
	ds.shm_perm.mode = 0666;
	shmctl(s->shmid, IPC_SET, &ds);
	
	s->data = shmat(s->shmid, NULL, 0);

	if (s->data == (char *)-1)
	{
		printf("error attaching the segment for id %d [%s]\n", s->shmid, strerror(errno));
	}
	s->manager = emm_new(s->data, "firstfit", s->size);
	s->elements = ecore_list_new();

	return s;
}



void
eshmd_shutdown(struct manager *m)
{
	struct segment *s;

	if (!m) return;

	/* delete each segment */
	if (!m->segments) goto free;
	ecore_list_first_goto(m->segments);
	s = ecore_list_current(m->segments);
	while (s)
	{
		struct element *e;
		char *path;

		/* remove segment elements */
		if (!s->elements) goto next;
		ecore_list_first_goto(s->elements);
		e = ecore_list_current(s->elements);
		while (e)
		{
			ecore_list_remove(s->elements);
			if (e->path)
				free(e->path);
			free(e);
			e = ecore_list_current(s->elements);
		}
		/* remove segment paths */
		if (!s->paths) goto next;
		ecore_list_first_goto(s->paths);
		path = ecore_list_current(s->paths);
		while (path)
		{
			ecore_list_remove(s->paths);
			free(path);
			path = ecore_list_current(s->paths);
		}
next:
		/* delete the segment */
		if (!s->ref)
		{
			shmdt(s->data);
			shmctl(s->shmid, IPC_RMID, NULL); 
		}
		else
		{
			struct shmid_ds buf;

			printf("ouch! can't remove the shm segment %d\n", s->shmid);
			printf("some process missed the release function?\n");
			shmdt(s->data);
			shmctl(s->shmid, IPC_STAT, &buf);
			printf("releasing it (%d)\n", (int)buf.shm_nattch);
			shmctl(s->shmid, IPC_RMID, NULL); 
		}
		ecore_list_remove(m->segments);
		emm_delete(s->manager);
		free(s->name);
		free(s);
		s = ecore_list_current(m->segments);
	}
free:
	;
	//e_dbus_connection_unref(m->conn);
}

#define DAEMONIZE 0

int main(int argc, char **argv)
{
	struct manager *m;
#if DAEMONIZE
	int null_fd;
	int pid;
#endif
	/* TODO parse command line */
	
	/* daemonize */
#if DAEMONIZE
	if (chdir("/") < 0)
	{
		fprintf(stderr, "Could not chdir to / (%s)\n", strerror(errno));
		exit(1);
	}
	pid = fork();
	switch (pid)
	{
		/* error */
		case -1:
		fprintf(stderr, "Could not fork (%s)\n", strerror(errno));
		break;
		
		/* child */
		case 0:
		null_fd = open("/dev/null", O_RDWR);
		if (null_fd >= 0)
		{
			dup2(null_fd, 0);
			dup2(null_fd, 1);
			dup2(null_fd, 2);
			close(null_fd);
		}
		umask(022);
		break;

		/* parent */
		default:
		exit(0);
		break;
	}
#endif
	ecore_init();
	ecore_file_init();
	e_dbus_init();

	if (!(m = eshmd_init())) goto error;

	ecore_main_loop_begin();
	eshmd_shutdown(m);
error:
	e_dbus_shutdown();
	ecore_file_shutdown();
	ecore_shutdown();

	return 0;
}
