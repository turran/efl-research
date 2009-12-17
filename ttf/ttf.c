#include "ttf.h"

typedef enum ttf_platform_id
{
	TTF_PLATFORM_UNICODE, /* unicode version */
	TTF_PLATFORM_MACINTOSH, /* script manager code */
	TTF_PLATFORM_RESERVED,
	TTF_PLATFORM_MICROSOFT, /* microsoft encoding */
	TTF_PLATFORM_IDS,
} ttf_platform_id;

typedef enum ttf_outline_flags
{
	TTF_OUTLINE_ON_CURVE = (1 << 0),
	TTF_OUTLINE_X_SHORT  = (1 << 1),
	TTF_OUTLINE_Y_SHORT  = (1 << 2),
	TTF_OUTLINE_REPEAT   = (1 << 3),
	TTF_OUTLINE_X_SAME   = (1 << 4),
	TTF_OUTLINE_Y_SAME   = (1 << 5),
} ttf_outline_flags;

typedef enum ttf_format_id
{
	TTF_FORMAT_2  = 2,
	TTF_FORMAT_4  = 4,
	TTF_FORMAT_8  = 8,
	TTF_FORMAT_10 = 10,
	TTF_FOTMAT_12 = 12,
} ttf_format_id;

typedef enum ttf_ms_id
{
	TTF_MS_SYMBOL,
	TTF_MS_UNICODE,
	TTF_MS_SHIFTJIS,
	TTF_MS_BIG5,
	TTF_MS_PRC,
	TTF_MS_WANSUNG,
	TTF_MS_JOHAB,
} ttf_ms_id;

typedef enum ttf_unicode_id
{
	TTF_UNICODE_DEFAULT,
	TTF_UNICODE_1_1,
	TTF_UNICODE_DEPRECATED,
	TTF_UNICODE_2_0,
} ttf_unicode_id;

struct _Font
{
	int fd;
	char *bytes;
	size_t size;

	Eina_Bool longoff;

	struct {
		ttf_format_id fmt;
		ssize_t off;
		ttf_directory dir;
	} cmap;
	ttf_directory head;
	ttf_directory glyf;
	ttf_directory loca;
};

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

static void ttf_directory_read(char *ptr, ttf_directory *dir)
{
	dir->tag = get_32(ptr);
	ptr += 4;
	dir->chksum = get_32(ptr);
	ptr += 4;
	dir->offset = get_32(ptr);
	ptr += 4;
	dir->length = get_32(ptr);
}

static Eina_Bool ttf_magic_check(char *ptr)
{
	uint32 st;

	st = get_32(ptr);
	if (st != 0x00010000)
		return EINA_FALSE;
	return EINA_TRUE;
}

static void ttf_tables_setup(Font *f)
{
	char *ptr = f->bytes;
	uint16 num;
	int i;

	/* get the number of tables and store the needed ones */
	ptr += 4;
	num = get_16(ptr);
	ptr += 8;
	for (i = 0; i < num; i++)
	{
		ttf_directory dir;

		ttf_directory_read(ptr, &dir);
		if (dir.tag == TTFTAG_HEAD)
			f->head = dir;
		if (dir.tag == TTFTAG_GLYF)
			f->glyf = dir;
		if (dir.tag == TTFTAG_CMAP)
			f->cmap.dir = dir;
		if (dir.tag == TTFTAG_GLYF)
			f->glyf = dir;
		if (dir.tag == TTFTAG_LOCA)
			f->loca = dir;
		ptr += 16;
	}
}

static int ttf_cmap_rate(uint16 pid, uint16 specpid)
{
	switch (pid)
	{
		case TTF_PLATFORM_UNICODE:
		switch (specpid)
		{
			case TTF_UNICODE_2_0:
			return 10;
			break;

			default:
			break;
		}
		break;

		case TTF_PLATFORM_MICROSOFT:
		switch (specpid)
		{
			case TTF_MS_UNICODE:
			return 10;
			break;

			default:
			break;
		}
		break;

		default:
		return 0;
		break;
	}
	return 0;
}

static int ttf_cmap_glyph_get_4(Font *f, short int ch)
{
	int nsegs;
	char *ptr = f->bytes + f->cmap.dir.offset + f->cmap.off;
	int segcountx2;
	int i;
	uint16 *start, *end, *range, *delta;

	ptr += 6;
	segcountx2 = get_16(ptr);
	nsegs = segcountx2 >> 1;

	ptr += 8;
	end = (uint16 *)ptr;
	start = (uint16 *)(ptr + segcountx2 + 2);
	delta = (uint16 *)(ptr + (segcountx2 * 2) + 2);
	range = (uint16 *)(ptr + (segcountx2 * 3) + 2);

	for (i = 0; i < nsegs; i++)
	{
		unsigned short int sv, ev, rv, dv;

		sv = get_16(start + i);
		if (sv == 0xffffff)
			return 0;

		ev = get_16(end + i);

		/* startcode <= ch <= endcode */
		if (ch > ev || ch < sv)
			continue;

		rv = get_16(range + i);
		dv = get_16(delta + i);
		/* if range offset is not 0, the mapping relies on indexarray */
		if (rv != 0)
		{
			unsigned int idx;

			idx = (rv / 2) + (ch - sv);
			printf("rv = %d\n", rv);
			return get_16(range + i + idx);
		}
		else
		{
			return ((ch + dv) % 65536);
		}
	}
	return 0;
}

static void ttf_head_setup(Font *f)
{
	char *ptr = f->bytes + f->head.offset;

	f->longoff = get_16(ptr + 50);
	printf("Long offsets = %d\n", f->longoff);
}

static void ttf_cmap_setup(Font *f)
{
	char *base = f->bytes + f->cmap.dir.offset;
	char *ptr = base;
	uint16 version, numsub;
	int i;
	int rate = 0;

	/* get the cmap data */
	ptr += 2;
	numsub = get_16(ptr);
	ptr += 2;

	for (i = 0; i < numsub; i++)
	{
		uint16 pid;
		uint16 specpid;
		uint32 off;
		int crate;

		pid = get_16(ptr);
		ptr += 2;
		specpid = get_16(ptr);
		ptr += 2;
		off = get_32(ptr);
		ptr += 4;

		/* choose the best cmap table */
		crate = ttf_cmap_rate(pid, specpid);
		if (rate < crate)
		{
			f->cmap.off = off;
			f->cmap.fmt = get_16(base + off);
			rate = crate;
		}
		printf("pid = %d, specid %d, off %d\n", pid, specpid, off);
	}
	printf("The cmap selected is %d @ %d (%d)\n", f->cmap.fmt, f->cmap.off, rate);
}

Font * ttf_fopen(const char *file)
{
	Font *f;
	int fd;
	struct stat st;
	void *ptr;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return NULL;

	if (fstat(fd, &st) < 0)
		return NULL;

	ptr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (ptr == MAP_FAILED)
		return NULL;

	/* check that the file is actually a ttf file */
	if (!ttf_magic_check(ptr))
	{
		printf("File is not a TTF File\n");
		return NULL;
	}

	f = malloc(sizeof(Font));
	f->fd = fd;
	f->bytes = ptr;
	f->size = st.st_size;
	ttf_tables_setup(f);
	/* get the head info */
	ttf_head_setup(f);
	/* get the better cmap */
	ttf_cmap_setup(f);

	return f;
}

void ttf_close(Font *f)
{
	munmap(f->bytes, f->size);
	if (f->fd)
		close(f->fd);
	free(f);
}

int ttf_glyph_index_get(Font *f, int ch)
{
	int idx = 0;

	/* given the ch and the font format retrieve the glyph index */
	printf("glyph get %c %d\n", ch, f->cmap.fmt);
	switch (f->cmap.fmt)
	{
		case TTF_FORMAT_2:
		case TTF_FORMAT_4:
		idx = ttf_cmap_glyph_get_4(f, ch);
		break;

		case TTF_FORMAT_8:
		case TTF_FORMAT_10:
		case TTF_FOTMAT_12:
		break;
	}
	return idx;
}

static void ttf_glyph_offsets_get(char *flags, int num, int *flength, int *xlength)
{
	int i;
	int len = 0;
	int repeat = 0;
	char *fcurr = flags;

	for (i = 0; i < num; i++)
	{
		unsigned char f = get_8(flags);
		int inc = 2;

		if (f & TTF_OUTLINE_X_SHORT)
			inc = 1;
		len += inc;

		if (f & TTF_OUTLINE_REPEAT)
		{
			int repeat;

			flags++;
			repeat = get_8(flags);
			len += inc * repeat;
			i += repeat;
		}
		flags++;
	}
	/* TODO check that i is exactly num here, as the repeat might make it overflow */
	*flength = flags - fcurr;
	*xlength = len;
}

void ttf_glyph_simple_process(Font *f, char *ptr, Glyph *g)
{

}


void ttf_glyph_info_get(Font *f, int idx, Glyph *g, glyph_point_cb cb, void *data)
{
	char *ptr;
	uint32 off;
	int ncontours;
	uint16 *contours;

	/* get the glyph offset based on index */
	ptr = f->bytes + f->loca.offset;
	if (f->longoff)
		off = get_32(ptr + (idx * 4));
	else
		off = get_16(ptr + (idx * 2)) << 1;
	/* check that offset is inside the file */
	if (f->glyf.offset + off >= f->size)
		return;

	/* get the glyph from the glyf table */
	ptr = f->bytes + f->glyf.offset + off;
	printf("xyMin %hd %hd - xyMax %hd %hd\n", get_16(ptr + 2), get_16(ptr + 4), get_16(ptr + 6), get_16(ptr + 8));
	ncontours = get_16(ptr);
	ptr += 10;
	contours = ptr;
	if (ncontours > 0)
	{
		int i;
		char *flags;
		char *instructions;
		char *xptr, *yptr;
		size_t ilength;
		int j = 0;
		int vlp;
		int xlength, flength;

		instructions = ptr + (2 * ncontours);
		ilength = get_16(instructions);
		instructions += 2;

		vlp = get_16(contours + ncontours - 1);
		if (!cb)
			return;
		/* TODO set the glyph information */
		/* set the coord pointers */
		flags = instructions + ilength;
		ttf_glyph_offsets_get(flags, vlp, &flength, &xlength);
		xptr = flags + flength;
		yptr = xptr + xlength;
		/* send the coord information */
		for (i = 0; i < ncontours; i++)
		{
			Eina_Bool first = EINA_TRUE;
			int lp;
			unsigned char f;
			int frepeat = 0;
			short int x = 0;
			short int y = 0;
			ttf_point point;

			/* new contour */
			lp = get_16(contours + i);
			for (; j < lp; j++)
			{

				if (frepeat)
				{
					frepeat--;
					goto parse_flags;
				}
				f = get_8(flags);
				flags++;

				if (f & TTF_OUTLINE_REPEAT)
				{

					frepeat = get_8(flags);
				}
parse_flags:
				/* x */
				if (f & TTF_OUTLINE_X_SHORT)
				{
					x = get_8(xptr);
					xptr++;
					if (!(f & TTF_OUTLINE_X_SAME))
						x = -x;
				}
				else
				{
					if (!(f & TTF_OUTLINE_X_SAME))
					{
						short int delta;

						delta = get_16(xptr);
						x += delta;
						xptr += 2;
					}
				}
				/* y */
				if (f & TTF_OUTLINE_Y_SHORT)
				{
					y = get_8(yptr);
					yptr++;
					if (!(f & TTF_OUTLINE_Y_SAME))
						y = -y;
				}
				else
				{
					if (!(f & TTF_OUTLINE_Y_SAME))
					{
						short int delta;

						delta = get_16(yptr);
						y += delta;
						yptr += 2;
					}
				}
				if (first)
				{
					/* TODO check first point and no ON_CURVE */
					first = EINA_FALSE;
					point.op = -1;
				}
				point.op++;
				point.x[point.op] = x;
				point.y[point.op] = y;
				/* send coordinates */
				if (f & TTF_OUTLINE_ON_CURVE)
				{
					cb(g, &point, data);
					point.op = 0;
				}
			}
		}
	}
	else
	{

	}
}
