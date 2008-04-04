#ifndef EVAS_PRIVATE_H_
#define EVAS_PRIVATE_H_

#include "Ekeko.h"
#include "Eina.h"
#include "Enesim.h"

#ifdef DEBUG
#define MAGIC_EVAS          0x70777770
#define MAGIC_OBJ           0x71777770
#endif

struct _Evas
{
	Ekeko_Canvas *canvas;
#ifdef DEBUG
	int magic;
#endif
};

struct _Evas_Object
{
	Eina_Inlist list;
#ifdef DEBUG
	int magic;
#endif
};

#endif /*EVAS_PRIVATE_H_*/
