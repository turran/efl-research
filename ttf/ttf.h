#ifndef _TTF_H
#define _TTF_H

#include "Eina.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

/* TTF reader
 * required tables:
 * cmap character to glyph mapping
 * glyf glyph data
 * head font header
 * hhea horizontal header
 * hmtx horizontal metrics
 * loca index to location
 * maxp maximum profile
 * name naming
 * post PostScript
 */

#define uint8 uint8_t
#define int8 int8_t
#define uint16 uint16_t
#define int16 int16_t
#define uint32 uint32_t
#define int32 int32_t
#define fracc int16_t
#define fixed int32_t
#define fword int16_t
#define ufword uint16_t
#define f2p14 int16_t
#define longdate time_t /* 64 bits */

#define TTFTAG(a, b, c, d) \
		((((uint8)a) << 24) | \
		(((uint8)b) << 16) | \
		(((uint8)c) << 8)  | \
		((uint8)d))

#define TTFTAG_CMAP TTFTAG('c', 'm', 'a', 'p')
#define TTFTAG_GLYF TTFTAG('g', 'l', 'y', 'f')
#define TTFTAG_HEAD TTFTAG('h', 'e', 'a', 'd')
#define TTFTAG_LOCA TTFTAG('l', 'o', 'c', 'a')

typedef enum ttf_op
{
	TTF_OP_MOVE_TO,
	TTF_OP_LINE_TO,
	TTF_OP_QUADRATIC_TO,
	TTF_OP_CUBIC_TO,
} ttf_op;

typedef struct ttf_point
{
	short int x[4];
	short int y[4];
	ttf_op op;
} ttf_point;

typedef struct ttf_directory
{
	uint32 tag;
	uint32 chksum;
	uint32 offset;
	uint32 length;
} ttf_directory;

static inline uint32_t get_32(char *bytes)
{
	uint32_t i;

	i = *((uint32_t *)bytes);
	return ntohl(i);
}

static inline uint16_t get_16(char *bytes)
{
	uint16_t sh;

	sh = *((uint16_t *)bytes);
	return ntohs(sh);
}

static inline uint8_t get_8(char *bytes)
{
	uint8_t ch;

	ch = *((uint8_t *)bytes);
	return ch;
}

/* API */
typedef struct _Font Font;

typedef struct _Glyph
{
	int numcontours;
	int numcoords;
} Glyph;

typedef void (*glyph_point_cb)(Glyph *g, ttf_point *p, void *data);

Font * ttf_fopen(const char *file);
int ttf_glyph_index_get(Font *f, int ch);
void ttf_glyph_info_get(Font *f, int index, Glyph *g, glyph_point_cb cb, void *data);

#endif
