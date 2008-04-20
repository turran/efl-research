#ifndef ENESIM_GENERATOR_H_
#define ENESIM_GENERATOR_H_

#include <stdio.h>


#define MAX_PLANES 4

typedef enum
{
	ROP_BLEND,
	ROP_FILL,
	ROPS
} Rop;

typedef enum
{
	PIXEL_OPAQUE,
	PIXEL_TRANSPARENT,
	PIXELS
} Pixel_Type;

typedef enum
{
	TYPE_UINT8,
	TYPE_UINT16,
	TYPE_UINT32,
	TYPES
} Type;

typedef enum
{
	COLOR_ALPHA,
	COLOR_RED,
	COLOR_GREEN,
	COLOR_BLUE,
	COLORS
} Color_Name;

typedef struct _Color
{
	Color_Name name;
	unsigned int offset;
	unsigned int length;
	unsigned int type;
} Color;

typedef struct _Plane
{
	Color colors[COLORS];
	unsigned int length;
	unsigned int num_colors;
	unsigned int type;
} Plane;

typedef struct _Format
{
	Plane planes[MAX_PLANES];
	unsigned int num_planes;
	const char *name;
	unsigned char premul;
} Format;

extern const char *type_names[TYPES];
extern const char *color_names[COLORS];
extern const int argb_offsets[COLORS];
extern const char *rop_names[ROPS];
extern const char *pixel_types[PIXELS];
extern Format *formats[];
extern FILE *fout;

void rop_functions(Format *sf, Format *df);
void core_functions(Format *f);
void drawer_functions(Format *f);
void data_parameters(Format *f);

#endif /*ENESIM_GENERATOR_H_*/
