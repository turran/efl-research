#ifndef ESVG_SVG_H_
#define ESVG_SVG_H_

typedef struct _ESVG_Svg ESVG_Svg;

struct _ESVG_Svg
{
	/* inheritance */
	ESVG_Container;
	ESVG_Rect *background;
};

#endif /*ESVG_SVG_H_*/
