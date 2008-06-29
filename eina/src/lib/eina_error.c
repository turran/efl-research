#include "Eina.h"
#include "eina_private.h"

static int _curr = 1;

/**
 * Register a new error type
 * @param str The description of the error
 * @return The unique number identifier for this error
 */
EAPI int eina_error_register(const char *str)
{
	/* TODO
	 * + store the string error
	 * + register the eina's basic errors
	 */
	return ++_curr;
}
/**
 * Given an error number return the description of it
 * @param error The error number
 * @return The description of the error
 */
EAPI const char * eina_error_str_get(int error)
{
	
}
