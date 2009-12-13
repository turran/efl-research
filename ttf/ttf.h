#ifndef _TTF_H
#define _TTF_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <string.h>

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
#define longdate time_t

#define TTFTAG(a, b, c, d) \
		((((uint8)a) << 24) | \
		(((uint8)b) << 16) | \
		(((uint8)c) << 8)  | \
		((uint8)d))

#define TTFTAG_CMAP TTFTAG('c', 'm', 'a', 'p')
#define TTFTAG_GLYF TTFTAG('g', 'l', 'y', 'f')

typedef enum ttf_platform_id
{
	TTF_PLATFORM_UNICODE, /* unicode version */
	TTF_PLATFORM_MACINTOSH, /* script manager code */
	TTF_PLATFORM_RESERVED,
	TTF_PLATFORM_MICROSOFT, /* microsoft encoding */
	TTF_PLATFORM_IDS,
} ttf_platform_id;

typedef struct ttf_directory
{
	uint32 tag;
	uint32 chksum;
	uint32 offset;
	uint32 length;
} ttf_directory;

typedef struct _Font
{
	FILE *f;
	/* TODO we need the desired encoding */
	ttf_directory cmap;
	ttf_directory glyf;
} Font;

static inline uint32_t get_32(Font *f)
{
	uint32_t i;

	fread(&i, sizeof(uint32_t), 1, f->f);
	return ntohl(i);
}

static inline uint16_t get_16(Font *f)
{
	uint16_t sh;

	fread(&sh, sizeof(uint16_t), 1, f->f);
	return ntohs(sh);
}

static inline uint8_t get_8(Font *f)
{
	uint8_t ch;

	fread(&ch, sizeof(uint8_t), 1, f->f);
	return ch;
}

/* API */
Font * ttf_fopen(const char *file);
int ttf_font_glyph_index_get(Font *f, char ch);

#endif
