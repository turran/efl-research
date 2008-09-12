#include "Emage.h"
#include "emage_private.h"
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static int _init_count = 0;
Eina_List *_modules = NULL;
Eina_List *_providers = NULL;
typedef Eina_Bool (*Module_Init)(void);

int _fifo[2]; /* the communication between the main thread and the async ones */ 

Eina_Error EMAGE_ERROR_EXIST;
Eina_Error EMAGE_ERROR_LOADER;
Eina_Error EMAGE_ERROR_FORMAT;
Eina_Error EMAGE_ERROR_SIZE;

static pthread_t tid;

typedef struct _Emage_Job
{
	const char *file;
	Enesim_Surface **s;
	Emage_Load_Callback cb;
	void *data;
} Emage_Job;


int _module_cb(Eina_Module *m, void *data)
{
	Module_Init mi;
	
	/* load the module and call module_init */
	eina_module_load(m);
	mi = eina_module_symbol_get(m, "module_init");
	if (!mi)
		return 0;
	return mi();
}

void * _thread_load(void *data)
{
	Emage_Job *j = data;
	int ret;
	
	printf("thread created\n");
	ret = write(_fifo[1], &j, sizeof(j));
	printf("%d %d\n", ret, sizeof(j));
	return NULL;
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * @brief To be documented.
 *
 * @param
 * @return
 * 
 * FIXME
 */
EAPI int emage_init(void)
{
	if (!_init_count)
	{
		eina_init();
		/* the errors */
		EMAGE_ERROR_EXIST = eina_error_register("Files does not exist");
		EMAGE_ERROR_LOADER = eina_error_register("No loader for such file");
		EMAGE_ERROR_FORMAT = eina_error_register("Wrong surface format");
		EMAGE_ERROR_SIZE = eina_error_register("Size missmatch");
		/* the modules */
#if 0
		_modules = eina_module_list_get(PACKAGE_LIB_DIR"/emage/", 1, _module_cb, NULL);
#else
		png_provider_init();
#endif
		/* the fifo */
		pipe(_fifo);
		fcntl(_fifo[0], F_SETFL, O_NONBLOCK);
		/* TODO the pool of threads */
	}
	return ++_init_count;
}
/**
 * 
 */
EAPI void emage_shutdown(void)
{
	_init_count--;
	if (!_init_count)
	{
		eina_shutdown();
		/* unload every module */
#if 0
		eina_module_list_free(_modules);
#else
		png_provider_exit();
#endif
		/* shutdown every provider */
		/* the fifo */
		close(_fifo[0]);
		close(_fifo[1]);
	}
}
/**
 * 
 */
EAPI void emage_info_load(const char *file, Enesim_Surface **s)
{
	
}
/**
 * 
 */
EAPI Eina_Bool emage_load(const char *file, Enesim_Surface **s)
{
	Eina_List *tmp;
	
	if (!file)
		return EINA_FALSE;
	/* iterate over the list of providers and check for a compatible loader */
	for (tmp = _providers; tmp; tmp = eina_list_next(tmp))
	{
		Emage_Provider *p = eina_list_data(tmp);
		
		if (p->load(file, s) == EINA_TRUE)
			return EINA_TRUE;
	}
	eina_error_set(EMAGE_ERROR_LOADER);
	return EINA_FALSE;
}
/**
 * 
 */
EAPI void emage_load_async(const char *file, Enesim_Surface **s, 
		Emage_Load_Callback cb, void *data)
{
	Emage_Job *j;
	
	j = malloc(sizeof(Emage_Job));
	j->file = file;
	j->cb = cb;
	j->data = data;
	j->s = s;
	/* create a thread that loads the image on background and sends
	 * a command into the fifo fd */
	pthread_create(&tid, NULL, _thread_load, j);
}
/**
 * @brief Call every asynchronous callback set
 * 
 * 
 */
EAPI void emage_dispatch(void)
{
	fd_set readset;
	struct timeval t;
	Emage_Job *j;

	/* check if there's data to read */
	FD_ZERO(&readset);
	FD_SET(_fifo[0], &readset);
	t.tv_sec = 0;
	t.tv_usec = 0;
	printf("entering\n");
	if (select(_fifo[0] + 1, &readset, NULL, NULL, &t) <= 0)
	{
		printf("nothing to read\n");
		return;
	}
	printf("reading\n");
	/* read from the fifo fd and call the needed callbacks */
	while (read(_fifo[0], &j, sizeof(j)) > 0)
	{
		printf("job for file %s finished\n", j->file);
		j->cb(j->s ? *j->s : NULL, j->data, 0);
		free(j);
	}
	printf("exiting\n");
}
/**
 * 
 */
EAPI Enesim_Surface * emage_save(const char *file)
{
	
}
/**
 * 
 */
EAPI int emage_provider_register(Emage_Provider *p)
{
	printf("called\n");
	_providers = eina_list_append(_providers, p);
	return EINA_TRUE;
}
/**
 * 
 */
EAPI void emage_provider_unregister(Emage_Provider *p)
{
	/* remove from the list of providers */
	_providers = eina_list_remove(_providers, p);
}
/**
 * @brief Sets the size of the thread's pool
 * @param num The number of threads
 * 
 * Sets the maximum number of threads Emage will create to dispatch asynchronous
 * calls. 
 */
EAPI void emage_pool_size_set(int num)
{
	
}
/**
 * @brief Gets the size of the thread's pool
 * 
 * @return The number of threads
 * Returns the maximum number threads of number Emage will create the dispatch
 * asynchronous calls. 
 */
EAPI int emage_pool_size_get(void)
{
	
}
