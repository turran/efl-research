#ifndef ENGINE_H_
#define ENGINE_H_

typedef struct _ESVG_Engine_Func
{
	/* context functions */
	void * (*context_new)(void *engine_data);
	void (*context_free)(void *engine_data, void *context);
	/* attributes */
	/* rect */
	void (*rect_draw)(void *engine_data, void *context, ESVG_Rect *r, Eina_Rectangle *rclip);
} ESVG_Engine_Func;

void * esvg_engine_context_new(void);
void esvg_engine_context_free(void *context);

extern ESVG_Engine_Func esvg_engine_cairo;

#endif /*ENGINE_H_*/
