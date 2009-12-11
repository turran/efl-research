#include "ttf.h"

typedef struct ttf_offset
{
	uint32 scaler_type;
	uint16 num;
	uint16 search;
	uint16 entry;
	uint16 range;
} ttf_offset;

static void ttf_offset_dump(ttf_offset *off)
{
	printf("* ttf offset\n");
	printf("scaler %08x\n", off->scaler_type);
	printf("num %d\n", off->num);
	printf("search %d\n", off->search);
	printf("entry %d\n", off->entry);
	printf("range %d\n", off->range);
}

static void ttf_offset_read(Font *f, ttf_offset *off)
{
	off->scaler_type = get_32(f);
	off->num = get_16(f);
	off->search = get_16(f);
	off->entry = get_16(f);
	off->range = get_16(f);
	ttf_offset_dump(off);
}

static void ttf_directory_dump(ttf_directory *dir)
{
	printf("* ttf directory\n");
	printf("tag %c%c%c%c\n", ((char *)&dir->tag)[3],
			((char *)&dir->tag)[2],
			((char *)&dir->tag)[1],
			((char *)&dir->tag)[0]);
	printf("checksum %d\n", dir->chksum);
	printf("offset %d\n", dir->offset);
	printf("length %d\n", dir->length);
}

static void ttf_directory_read(Font *f, ttf_directory *dir)
{
	dir->tag = get_32(f);
	dir->chksum = get_32(f);
	dir->offset = get_32(f);
	dir->length = get_32(f);
	ttf_directory_dump(dir);
}

Font * ttf_fopen(const char *file)
{
	Font *f;
	ttf_offset off;
	int i;

	f = malloc(sizeof(Font));
	f->f = fopen(file, "r");
	ttf_offset_read(f, &off);
	for (i = 0; i < off.num; i++)
	{
		ttf_directory dir;

		ttf_directory_read(f, &dir);
		if (dir.tag == TTFTAG_CMAP)
			f->cmap = dir;
		if (dir.tag == TTFTAG_GLYF)
			f->glyf = dir;
	}

	return f;
}
