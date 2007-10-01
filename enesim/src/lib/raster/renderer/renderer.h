#ifndef _RENDERER_H
#define _RENDERER_H

typedef struct _Enesim_Renderer_Func
{

} Enesim_Renderer_Func;

struct _Enesim_Renderer
{
	void 				*data;
	Enesim_Renderer_Func 		*funcs;
};

#endif
