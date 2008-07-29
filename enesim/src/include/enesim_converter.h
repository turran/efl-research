#ifndef ENESIM_CONVERTER_H_
#define ENESIM_CONVERTER_H_

/**
 * @defgroup Enesim_Surface_Group Surface
 * @{
 */
typedef void (*Enesim_Converter_Fnc)(Enesim_Surface *s, Enesim_Surface *d);

EAPI Enesim_Converter_Fnc enesim_converter_fnc_get(Enesim_Surface_Format stype,
		Enesim_Surface_Format dtype);

/** @} */
#endif /*ENESIM_CONVERTER_H_*/
