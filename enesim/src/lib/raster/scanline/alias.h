#ifndef _ALIAS_H
#define _ALIAS_H

typedef struct _Scanline_Alias_Sl
{
	int y;
	int x;
	int w;
} Scanline_Alias_Sl;

typedef struct _Scanline_Alias
{
	Edata_Array 		*a;
	Scanline_Alias_Sl	*sls;
	int 			num_sls;
} Scanline_Alias;

#endif
