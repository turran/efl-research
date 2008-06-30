#ifndef EINA_ERROR_H_
#define EINA_ERROR_H_

typedef enum _Eina_Error_Level
{
	EINA_ERROR_LEVEL_ERR,
	EINA_ERROR_LEVEL_WARN,
	EINA_ERROR_LEVEL_INFO,
	EINA_ERROR_LEVEL_DBG,
	EINA_ERROR_LEVELS
} Eina_Error_Level;

EAPI int eina_error_init(void);
EAPI int eina_error_shutdown(void);
EAPI int eina_error_register(const char *msg);
EAPI const char * eina_error_msg_get(int error);
EAPI void eina_error_perr(const char *fmt, ...);
EAPI void eina_error_pwarn(const char *fmt, ...);
EAPI void eina_error_pdebug(const char *fmt, ...);
EAPI void eina_error_pinfo(const char *fmt, ...);
EAPI void eina_error_print(Eina_Error_Level level, const char *fmt, ...);
EAPI void eina_error_log_level_set(Eina_Error_Level level);

#endif /*EINA_ERROR_H_*/
