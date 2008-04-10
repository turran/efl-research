#include <stdio.h>

#define MAX_PLANES 4

typedef enum
{
	TYPE_UINT8,
	TYPE_UINT16,
	TYPE_UINT32,
	TYPES
} Type;

typedef enum
{
	COLOR_RED,
	COLOR_GREEN,
	COLOR_BLUE,
	COLOR_ALPHA,
	COLORS
} Color_Name;

typedef struct _Color
{
	Color_Name name;
	unsigned int offset;
	unsigned int length;
} Color;

typedef struct _Plane
{
	Color colors[COLORS];
	unsigned int length;
} Plane;

typedef struct _Format
{
	Plane planes[MAX_PLANES];
	const char *name;
} Format;

/*============================================================================*
 *                                  argb8888                                  * 
 *============================================================================*/
Format argb8888 = {
	.name = "argb888",
	.planes[0] = {
		.colors[COLOR_BLUE] = {
			.offset = 0,
			.length = 8,
		},
		.colors[COLOR_GREEN] = {
			.offset = 8,
			.length = 8,
		},
		.colors[COLOR_RED] = {
			.offset = 16,
			.length = 8,
		},
		.colors[COLOR_ALPHA] = {
			.offset = 24,
			.length = 8,
		},
	},
};

int main(void)
{
	/* TODO for every format generate the code */
	/* TODO what about indentation? before writing the file try to call
	 * indent and write the final file like that?
	 */
	return 0;
}
