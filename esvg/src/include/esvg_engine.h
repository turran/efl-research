#ifndef ESVG_ENGINE_H_
#define ESVG_ENGINE_H_

/* TODO add a struct to describe the engine_data
 * cairo for example should receive data based on the
 * backend cairo supports (x11 = dpy, etc, etc)
 * for enesim (sw only) we need the flush function: when the
 * backbuffer is drawn we need to flip it into the frontbuffer
 * + TODO how to handle relative coordinates? add callbacks when an object
 * is resized and moved and keep track of the union of every bounding box
 */ 

typedef enum _ESVG_Engine_Type
{
	ESVG_ENGINE_UNKNOWN,
	ESVG_ENGINE_CAIRO,
	ESVG_ENGINE_ENESIM,
	ESVG_ENGINES,
} ESVG_Engine_Type;

EAPI Eina_Bool esvg_engine_output_size_set(unsigned int width, unsigned int height);
EAPI Eina_Bool esvg_engine_set(ESVG_Engine_Type type, void *engine_data);

#endif /*ESVG_ENGINE_H_*/
