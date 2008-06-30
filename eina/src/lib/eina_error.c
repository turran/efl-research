#include "Eina.h"
#include "eina_private.h"
/* TODO
 * + printing errors to stdout or stderr can be implemented
 * using a queue, usful for multiple threads printing
 * + add a wapper for assert?
 * + add common error numbers, messages
 */
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static int _curr = 1;
static int _init_count = 0;
static Eina_List *_error_list;

#define RED     "\033[31;1m"
#define GREEN   "\033[32;1m"
#define YELLOW  "\033[32;1m"
#define WHITE   "\033[37;1m"
#define NOTHING "\033[0m"


#ifdef DEBUG
static _log_level = EINA_ERROR_LEVEL_DBG;
#else
static _log_level = EINA_ERROR_LEVEL_ERR;
#endif

static char *_colors[EINA_ERROR_LEVELS] = {
	[EINA_ERROR_LEVEL_ERR] = RED,
	[EINA_ERROR_LEVEL_WARN] = YELLOW,
	[EINA_ERROR_LEVEL_INFO] = NOTHING,
	[EINA_ERROR_LEVEL_DBG] = GREEN,
};

static void _error_print(Eina_Error_Level level, const char *fmt, va_list args)
{
	if (level <= _log_level)
	{
		printf("%s", _colors[level]);
		vprintf(fmt, args);
		printf("%s", _colors[EINA_ERROR_LEVEL_INFO]);
	}
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
void eina_error_magic_check(unsigned int magic, unsigned int cmp)
{
	assert(magic == cmp);
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * 
 */
EAPI int eina_error_init(void)
{
	if (!_init_count) 
	{
		/* TODO register the eina's basic errors */
		/* TODO load the environment variable for getting the log level */
		/* getenv(EINA_LOG_LEVEL)
		 */
	}
	/* get all the modules */
	return ++_init_count;
}

/**
 * 
 */
EAPI int eina_error_shutdown(void)
{
	if (!_init_count)
			return _init_count;
	_init_count--;
	if (!_init_count)
	{
		/* remove the error strings */
		while (_error_list)
		{
			free(eina_list_data(_error_list));
			_error_list = eina_list_free(_error_list);
		}
	}
	return _init_count;
}

/**
 * Register a new error type
 * @param str The description of the error
 * @return The unique number identifier for this error
 */
EAPI int eina_error_register(const char *msg)
{
	char *str;
	
	str = strdup(msg);
	_error_list = eina_list_append(_error_list, str);
	
	return ++_curr;
}
/**
 * Given an error number return the description of it
 * @param error The error number
 * @return The description of the error
 */
EAPI const char * eina_error_msg_get(int error)
{
	return eina_list_nth(_error_list, error);
}
/**
 * 
 */
EAPI void eina_error_perr(const char *fmt, ...)
{
	va_list args;
		
	va_start(args, fmt);
	_error_print(EINA_ERROR_LEVEL_ERR, fmt, args);
	va_end(args);	
}
/**
 * 
 */
EAPI void eina_error_pwarn(const char *fmt, ...)
{
	va_list args;
		
	va_start(args, fmt);
	_error_print(EINA_ERROR_LEVEL_WARN, fmt, args);
	va_end(args);	
}
/**
 * 
 */
EAPI void eina_error_pdebug(const char *fmt, ...)
{
	va_list args;
		
	va_start(args, fmt);
	_error_print(EINA_ERROR_LEVEL_DBG, fmt, args);
	va_end(args);	
}
/**
 * 
 */
EAPI void eina_error_pinfo(const char *fmt, ...)
{
	va_list args;
		
	va_start(args, fmt);
	_error_print(EINA_ERROR_LEVEL_INFO, fmt, args);
	va_end(args);	
}
/**
 * 
 */
EAPI void eina_error_print(Eina_Error_Level level, const char *fmt, ...)
{
	va_list args;
	
	va_start(args, fmt);
	_error_print(level, fmt, args);
	va_end(args);
}
/**
 * 
 */
EAPI void eina_error_log_level_set(Eina_Error_Level level)
{
	_log_level = level;
}
