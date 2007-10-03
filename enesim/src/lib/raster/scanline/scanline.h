#ifndef _SCANLINE_H
#define _SCANLINE_H

/**
 * @file
 * @brief Scanline
 * @defgroup Scanline_Internal_Group Scanline
 * @ingroup Internal_Group
 * @{
 */

typedef void *(*Enesim_Scanline_Create)(void);
typedef void (*Enesim_Scanline_Add)(void *d, int x0, int x1, int y, int coverage);
typedef void (*Enesim_Scanline_Free)(void *d);

typedef struct _Enesim_Scanline_Func
{
	Enesim_Scanline_Add 	add;
	Enesim_Scanline_Free 	free;
} Enesim_Scanline_Func;

struct _Enesim_Scanline
{
	int 			anti_alias;
	Enesim_Scanline_Func 	*funcs;
	void 			*data;
};

/** @} */

#endif
