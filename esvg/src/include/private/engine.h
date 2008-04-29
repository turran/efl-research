#ifndef ENGINE_H_
#define ENGINE_H_

typedef struct _ESVG_Engine
{
	/* context functions */
	void * (*context_new)(void *engine_data);
	void (*context_free)(void *engine_data, void *context);
	/* attributes */
	/* rect */
//	void (*rect_render)(ESVG_Rect *r);
} ESVG_Engine;

void * esvg_engine_context_new(ESVG *svg);
void esvg_engine_context_free(ESVG *svg, void *context);

extern ESVG_Engine esvg_engine_cairo;

#endif /*ENGINE_H_*/
