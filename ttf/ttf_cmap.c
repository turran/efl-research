#include "ttf.h"

typedef enum ttf_cmap_format_id
{
	TTF_FORMAT_2  = 2,
	TTF_FORMAT_4  = 4,
	TTF_FORMAT_8  = 8,
	TTF_FORMAT_10 = 10,
	TTF_FOTMAT_12 = 12,
} ttf_cmap_format_id;

typedef struct ttf_cmap
{
	uint16 version;
	uint16 numsub;
} ttf_cmap;

typedef struct ttf_cmap_sub
{
	uint16 platformid;
	uint16 platformspecid;
	uint32 offset;
} ttf_cmap_sub;

typedef struct ttf_cmap_format
{
	uint16 format;
	uint16 length;
	uint16 language;
	union {
		struct {

		} format2;

		struct {

		} format4;
		struct {
		} format6;
		struct {

		} format8;
		struct {

		} format10;
		struct {
		} format12;
	} specific;
} ttf_cmap_format;


static void ttf_cmap_dump(ttf_cmap *cmap)
{
	printf("* ttf cmap\n");
	printf("version %d\n", cmap->version);
	printf("num %d\n", cmap->numsub);
}

static void ttf_cmap_sub_dump(ttf_cmap_sub *sub)
{
	printf("* ttf cmap sub\n");
	printf("platform id %d\n", sub->platformid);
	printf("platform spec id %d\n", sub->platformspecid);
	printf("offset %d\n", sub->offset);
}

static void ttf_cmap_sub_read(Font *f, ttf_cmap_sub *sub)
{
	sub->platformid = get_16(f);
	sub->platformspecid = get_16(f);
	sub->offset = get_32(f);
}

static void ttf_cmap_read(Font *f, ttf_cmap *cmap)
{
	cmap->version = get_16(f);
	cmap->numsub = get_16(f);
}

static void ttf_cmap_format_read(Font *f, ttf_cmap_format *fmt)
{
	fmt->format = get_16(f);
	fmt->length = get_16(f);
	fmt->language = get_16(f);
}

static void ttf_cmap_format_index_get(Font *f, ttf_cmap_format *fmt, char ch)
{

}

void ttf_cmap_parse(Font *f)
{

}

int ttf_glyph_index_get(Font *f, uint32_t ch)
{
	ttf_cmap cmap;
	fpos_t curr, table;
	int i;

	/* get the format and return the real glyph index */
	fgetpos(f->f, &curr);
	fseek(f->f, f->cmap.offset, SEEK_SET);
	fgetpos(f->f, &table);
	ttf_cmap_read(f, &cmap);
	ttf_cmap_dump(&cmap);
	for (i = 0; i < cmap.numsub; i++)
	{
		fpos_t prev;

		ttf_cmap_sub sub;
		ttf_cmap_format fmt;

		ttf_cmap_sub_read(f, &sub);
		ttf_cmap_sub_dump(&sub);

		fgetpos(f->f, &prev);
		fsetpos(f->f, &table);
		fseek(f->f, sub.offset, SEEK_CUR);
		ttf_cmap_format_read(f, &fmt);
		fsetpos(f->f, &prev);
	}
	fsetpos(f->f, &curr);
}

