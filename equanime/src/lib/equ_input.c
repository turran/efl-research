#include "Equanime.h"
#include "equ_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI const char * equ_input_name_get(Equ_Input *i)
{
	return i->name;
}

EAPI Equ_Mode * equ_input_mode_get(Equ_Input *i)
{
	return i->mode;
}

EAPI Eina_Bool equ_input_mode_set(Equ_Input *i, Equ_Mode *m)
{

}
