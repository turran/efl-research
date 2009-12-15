#include "ttf.h"

typedef enum ttf_platform_id
{
	TTF_PLATFORM_UNICODE, /* unicode version */
	TTF_PLATFORM_MACINTOSH, /* script manager code */
	TTF_PLATFORM_RESERVED,
	TTF_PLATFORM_MICROSOFT, /* microsoft encoding */
	TTF_PLATFORM_IDS,
} ttf_platform_id;

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
	ttf_directory_dump(dir);
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
/* 
UInt16  	format  	Format number is set to 4  	 
UInt16 	length 	Length of subtable in bytes 	 
UInt16 	language 	Language code for this encoding subtable, or zero if language-independent 	 
UInt16 	segCountX2 	2 * segCount 	 
UInt16 	searchRange 	2 * (2**FLOOR(log2(segCount))) 	 
UInt16 	entrySelector 	log2(searchRange/2) 	 
UInt16 	rangeShift 	(2 * segCount) - searchRange 	 
UInt16 	endCode[segCount] 	Ending character code for each segment, last = 0xFFFF. 	
UInt16 	reservedPad 	This value should be zero 	
UInt16 	startCode[segCount] 	Starting character code for each segment 	
UInt16 	idDelta[segCount] 	Delta for all character codes in segment 	 
UInt16 	idRangeOffset[segCount] 	Offset in bytes to glyph indexArray, or 0 	 
UInt16 	glyphIndexArray[variable] 	Glyph index array
*/
static int ttf_cmap_glyph_get_4(Font *f, char ch)
{
	/* startcode <= ch <= endcode */
}

static void ttf_head_setup(Font *f)
{
	char *ptr = f->bytes + f->head.offset;

	printf("%08x\n", get_32(ptr + 12));
	f->longoff = get_16(ptr + 48);
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

int ttf_glyph_get(Font *f, char ch)
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
