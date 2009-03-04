#include "Emage.h"
#include "emage_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static int _init_count = 0;
static Eina_Array *_modules = NULL;
static Eina_List *_providers = NULL;
static int _fifo[2]; /* the communication between the main thread and the async ones */

Eina_Error EMAGE_ERROR_EXIST;
Eina_Error EMAGE_ERROR_PROVIDER;
Eina_Error EMAGE_ERROR_FORMAT;
Eina_Error EMAGE_ERROR_SIZE;

static pthread_t tid;

typedef struct _Emage_Job
{
	const char *file;
	Enesim_Surface **s;
	Emage_Load_Callback cb;
	void *data;
	Eina_Error err;
} Emage_Job;

void _provider_info_load(Emage_Provider *p, const char *file, int *w, int *h, Enesim_Surface_Format *sfmt)
{
	int pw, ph;
	Enesim_Surface_Format pfmt;

	/* get the info from the image */
	p->info_get(file, &pw, &ph, &pfmt);
	if (w) *w = pw;
	if (h) *h = ph;
	if (sfmt) *sfmt = pfmt;
}

void _provider_data_load(Emage_Provider *p, const char *file, Enesim_Surface **s)
{
	Enesim_Surface *ls;
	Enesim_Surface *stmp;
	int w, h;
	Enesim_Surface_Format sfmt;

	_provider_info_load(p, file, &w, &h, &sfmt);
	if (*s)
	{
		int sw, sh;
		Enesim_Surface_Format ssfmt;

		ssfmt = enesim_surface_format_get(*s);
		enesim_surface_size_get(*s, &sw, &sh);
		/* create a temporary surface */
		if ((ssfmt != sfmt) || (sw != w) || (sh != h))
		{
			ls = enesim_surface_new(sfmt, w, h);
		}
		else
			ls = *s;
	}
	/* create a temporary surface */
	else
	{
		ls = enesim_surface_new(sfmt, w, h);
		*s = ls;
	}
	/* load the file */
	if (p->load(file, ls) == EINA_FALSE)
		return;
	/* convert if needed */
	if (ls != *s)
	{
		enesim_surface_convert(ls, *s, NULL);
		enesim_surface_delete(ls);
	}
}

Emage_Provider * _provider_get(const char *file)
{
	Eina_List *tmp;
	Emage_Provider *p;
	struct stat stmp;

	if ((!file) || (stat(file, &stmp) < 0))
		return NULL;
	/* iterate over the list of providers and check for a compatible loader */
	for (tmp = _providers; tmp; tmp = eina_list_next(tmp))
	{
		p = eina_list_data_get(tmp);
		/* TODO priority loaders */
		/* check if the provider can load the image */
		if (!p->loadable)
			continue;
		if (p->loadable(file) == EINA_TRUE)
			return p;
	}
	return NULL;
}

void _thread_finish(Emage_Job *j)
{
	int ret;
	ret = write(_fifo[1], &j, sizeof(j));
}

void * _thread_load(void *data)
{
	Emage_Provider *prov;
	Emage_Job *j = data;

	prov = _provider_get(j->file);
	if (!prov)
	{
		j->err = EMAGE_ERROR_PROVIDER;
		_thread_finish(j);
		return NULL;
	}
	_provider_data_load(prov, j->file, j->s);
	_thread_finish(j);
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
		EMAGE_ERROR_EXIST = eina_error_msg_register("Files does not exist");
		EMAGE_ERROR_PROVIDER = eina_error_msg_register("No provider for such file");
		EMAGE_ERROR_FORMAT = eina_error_msg_register("Wrong surface format");
		EMAGE_ERROR_SIZE = eina_error_msg_register("Size mismatch");
		/* the modules */
#if 1
		_modules = eina_module_list_get(_modules, PACKAGE_LIB_DIR"/emage/", 1, NULL, NULL);
		eina_module_list_load(_modules);
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
#if 1
		eina_module_list_delete(_modules);
#else
		png_provider_exit();
#endif
		/* shutdown every provider */
		/* TODO what if we shutdown while some thread is active? */
		/* the fifo */
		close(_fifo[0]);
		close(_fifo[1]);
	}
}
/**
 *
 */
EAPI Eina_Bool emage_info_load(const char *file, int *w, int *h, Enesim_Surface_Format *sfmt)
{
	Emage_Provider *prov;

	prov = _provider_get(file);
	if (!prov)
	{
		eina_error_set(EMAGE_ERROR_PROVIDER);
		return EINA_FALSE;
	}
	_provider_info_load(prov, file, w, h, sfmt);
	return EINA_TRUE;
}
/**
 *
 */
EAPI Eina_Bool emage_load(const char *file, Enesim_Surface **s, const char *options)
{
	Emage_Provider *prov;

	prov = _provider_get(file);
	if (!prov)
	{
		eina_error_set(EMAGE_ERROR_PROVIDER);
		return EINA_FALSE;
	}
	_provider_data_load(prov, file, s);
	return EINA_TRUE;
}
/**
 *
 */
EAPI void emage_load_async(const char *file, Enesim_Surface **s,
		Emage_Load_Callback cb, void *data, const char *options)
{
	Emage_Job *j;

	j = malloc(sizeof(Emage_Job));
	j->file = file;
	j->cb = cb;
	j->data = data;
	j->s = s;
	j->err = 0;
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

	if (select(_fifo[0] + 1, &readset, NULL, NULL, &t) <= 0)
		return;
	/* read from the fifo fd and call the needed callbacks */
	while (read(_fifo[0], &j, sizeof(j)) > 0)
	{
		j->cb(j->s ? *j->s : NULL, j->data, j->err);
		free(j);
	}
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
EAPI Eina_Bool emage_provider_register(Emage_Provider *p)
{
	if (!p)
		return EINA_FALSE;
	/* check for mandatory functions */
	if (!p->loadable)
	{
		EINA_ERROR_PERR("Provider %s doesn't provide the loadable() function\n", p->name);
		goto err;
	}
	if (!p->info_get)
	{
		EINA_ERROR_PERR("Provider %s doesn't provide the info_get() function\n", p->name);
		goto err;
	}
	if (!p->load)
	{
		EINA_ERROR_PERR("Provider %s doesn't provide the load() function\n", p->name);
		goto err;
	}
	_providers = eina_list_append(_providers, p);
	return EINA_TRUE;
err:
	return EINA_FALSE;
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
