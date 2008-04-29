#ifndef ESVG_CANVAS_H_
#define ESVG_CANVAS_H_

typedef struct _ESVG ESVG;

typedef enum _ESVG_Engine_Type
{
	ESVG_ENGINE_CAIRO,
	ESVG_ENGINE_ENESIM,
	ESVG_ENGINES,
} ESVG_Engine_Type;

EAPI ESVG * esvg_new(int w, int h, ESVG_Engine_Type type, void *engine_data);
EAPI void esvg_free(ESVG *e);

#endif /*ESVG_CANVAS_H_*/
