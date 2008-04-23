#ifndef EVAS_PRIVATE_H_
#define EVAS_PRIVATE_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Ekeko.h"
#include "Eina.h"
#include "Enesim.h"

#ifdef DEBUG
#define MAGIC_EVAS          0x70777770
#define MAGIC_OBJ           0x71777770
#define MAGIC_CHECK()
#else
#define MAGIC_CHECK()
#endif

struct _Evas
{
	Ekeko_Canvas *canvas;
#ifdef DEBUG
	int magic;
#endif
	unsigned int w;
	unsigned int h;
	unsigned char changed : 1;
};

struct _Evas_Object
{
	Eina_Inlist list;
#ifdef DEBUG
	int magic;
#endif
};

typedef struct _Evas_Engine_Func Evas_Engine_Func;

struct _Evas_Engine
{
	Evas_Engine_Func *func;
};

/* TODO define engine backend */
struct _Evas_Engine_Func
{
	/* init engine */
	/* shutdown engine */
	/* flush rectangles */
	int (*flush)(void *data, Enesim_Rectangle *r);
	/* get native surface */
};

#endif /*EVAS_PRIVATE_H_*/
