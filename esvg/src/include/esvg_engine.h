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

#define ESVG_ENGINE_CLASS       (esvg_engine_class_get())
#define ESVG_ENGINE(obj)       (EOBJ_OBJECT_CAST((obj), ESVG_ENGINE_CLASS, ESVG_Engine))
#define ESVG_IS_ENGINE(obj)    (EOBJ_OBJECT_CHECK_CLASS((obj), ESVG_ENGINE_CLASS))

typedef enum _ESVG_Engine_Type
{
	ESVG_ENGINE_UNKNOWN,
	ESVG_ENGINE_CAIRO,
	ESVG_ENGINE_ENESIM,
	ESVG_ENGINES,
} ESVG_Engine_Type;

typedef struct _ESVG_Engine_Func ESVG_Engine_Func;

struct _ESVG_Engine
{
	Eobj_Object object;
	ESVG_Rectangle geometry;
	void *data;
	ESVG_Engine_Type type;
	ESVG_Engine_Func *func;
};

EAPI ESVG_Engine * esvg_engine_new(void);

#endif /*ESVG_ENGINE_H_*/
