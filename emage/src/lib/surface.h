#ifndef _SURFACE_H
#define _SURFACE_H

typedef enum _Emage_Surface_Flag
{
	SURFACE_FLAG_HAS_ALPHA 		= (1 << 0),
	SURFACE_FLAG_HAS_SPARSE_ALPHA 	= (1 << 1),
	SURFACE_FLAGS 			= (1 << 2)
} Emage_Surface_Flag;

typedef struct _Emage_Data_ARGB8888
{
	DATA32 *data;
} Emage_Data_ARGB8888;

typedef struct _Emage_Data_RGB565_A5
{
	DATA16 	*data;
	DATA8 	*alpha;
} Emage_Data_RGB565_A5;

/* FIXME check the above flags!! */
struct _Emage_Surface
{
	int                	w, h;
	union {
		Emage_Data_ARGB8888 	argb8888;
		Emage_Data_RGB565_A5 	rgb565_a5;
	} data;
	Emage_Data_Format	format;
	Emage_Surface_Flag 	flags;
};


#define RGBA_SURFACE_HAS_ALPHA 1
#define RGBA_SURFACE_ALPHA_SPARSE 2

#endif
