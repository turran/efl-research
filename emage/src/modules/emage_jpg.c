#include <stdio.h>
#include <jpeglib.h>
#include <setjmp.h>

#include "Emage.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _JPEG_error_mgr *emptr;
struct _JPEG_error_mgr
{
   struct     jpeg_error_mgr pub;
   jmp_buf    setjmp_buffer;
};

static void _JPEGFatalErrorHandler(j_common_ptr cinfo);
static void _JPEGErrorHandler(j_common_ptr cinfo);
static void _JPEGErrorHandler2(j_common_ptr cinfo, int msg_level);

static void
_JPEGFatalErrorHandler(j_common_ptr cinfo)
{
   emptr errmgr;

   errmgr = (emptr) cinfo->err;
   /*   cinfo->err->output_message(cinfo);*/
   longjmp(errmgr->setjmp_buffer, 1);
   return;
}

static void
_JPEGErrorHandler(j_common_ptr cinfo)
{
   emptr errmgr;

   errmgr = (emptr) cinfo->err;
   /*   cinfo->err->output_message(cinfo);*/
   /*   longjmp(errmgr->setjmp_buffer, 1);*/
   return;
}

static void
_JPEGErrorHandler2(j_common_ptr cinfo, int msg_level)
{
   emptr errmgr;

   errmgr = (emptr) cinfo->err;
   /*   cinfo->err->output_message(cinfo);*/
   /*   longjmp(errmgr->setjmp_buffer, 1);*/
   return;
   msg_level = 0;
}

static int
evas_image_load_file_head_jpeg_internal(Image_Entry *ie, FILE *f)
{
   int w, h, scalew, scaleh;
   struct jpeg_decompress_struct cinfo;
   struct _JPEG_error_mgr jerr;

   if (!f) return 0;
   cinfo.err = jpeg_std_error(&(jerr.pub));
   jerr.pub.error_exit = _JPEGFatalErrorHandler;
   jerr.pub.emit_message = _JPEGErrorHandler2;
   jerr.pub.output_message = _JPEGErrorHandler;
   if (setjmp(jerr.setjmp_buffer))
     {
	jpeg_destroy_decompress(&cinfo);
	return 0;
     }
   jpeg_create_decompress(&cinfo);
   jpeg_stdio_src(&cinfo, f);
   jpeg_read_header(&cinfo, TRUE);
   cinfo.do_fancy_upsampling = FALSE;
   cinfo.do_block_smoothing = FALSE;
   cinfo.dct_method = JDCT_IFAST;
   cinfo.dither_mode = JDITHER_ORDERED;
   jpeg_start_decompress(&cinfo);

/* head decoding */
   w = cinfo.output_width;
   h = cinfo.output_height;
   if ((w < 1) || (h < 1) || (w > 8192) || (h > 8192))
     {  
        jpeg_destroy_decompress(&cinfo);
	return 0;
     }
   if (ie->load_opts.scale_down_by > 1)
     {
	w /= ie->load_opts.scale_down_by;
	h /= ie->load_opts.scale_down_by;
     }
   else if (ie->load_opts.dpi > 0.0)
     {
	w = (w * ie->load_opts.dpi) / 90.0;
	h = (h * ie->load_opts.dpi) / 90.0;
     }
   else if ((ie->load_opts.w > 0) &&
	    (ie->load_opts.h > 0))
     {
	int w2, h2;
	
	w2 = ie->load_opts.w;
	h2 = (ie->load_opts.w * h) / w;
	if (h2 > ie->load_opts.h)
	  {
	     h2 = ie->load_opts.h;
	     w2 = (ie->load_opts.h * w) / h;
	  }
	w = w2;
	h = h2;
     }
   if (w < 1) w = 1;
   if (h < 1) h = 1;
   
   if ((w != cinfo.output_width) || (h != cinfo.output_height))
     {
	scalew = cinfo.output_width / w;
	scaleh = cinfo.output_height / h;
	
	ie->scale = scalew;
	if (scaleh < scalew) ie->scale = scaleh;
	
	if      (ie->scale > 8) ie->scale = 8;
	else if (ie->scale < 1) ie->scale = 1;
	
	if      (ie->scale == 3) ie->scale = 2;
	else if (ie->scale == 5) ie->scale = 4;
	else if (ie->scale == 6) ie->scale = 4;
	else if (ie->scale == 7) ie->scale = 4;
     }

   if (ie->scale > 1)
     {
	jpeg_destroy_decompress(&cinfo);
   
	rewind(f);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, f);
	jpeg_read_header(&cinfo, TRUE);
	cinfo.do_fancy_upsampling = FALSE;
	cinfo.do_block_smoothing = FALSE;
	cinfo.scale_num = 1;
	cinfo.scale_denom = ie->scale;
	jpeg_calc_output_dimensions(&(cinfo));
	jpeg_start_decompress(&cinfo);
     }
   
   ie->w = cinfo.output_width;
   ie->h = cinfo.output_height;
/* end head decoding */

   jpeg_destroy_decompress(&cinfo);
   return 1;
}

static int
evas_image_load_file_data_jpeg_internal(Image_Entry *ie, FILE *f)
{
   int w, h;
   struct jpeg_decompress_struct cinfo;
   struct _JPEG_error_mgr jerr;
   DATA8 *ptr, *line[16], *data;
   DATA32 *ptr2;
   int x, y, l, i, scans, count, prevy;

   if (!f) return 0;
   cinfo.err = jpeg_std_error(&(jerr.pub));
   jerr.pub.error_exit = _JPEGFatalErrorHandler;
   jerr.pub.emit_message = _JPEGErrorHandler2;
   jerr.pub.output_message = _JPEGErrorHandler;
   if (setjmp(jerr.setjmp_buffer))
     {
	jpeg_destroy_decompress(&cinfo);
	return 0;
     }
   jpeg_create_decompress(&cinfo);
   jpeg_stdio_src(&cinfo, f);
   jpeg_read_header(&cinfo, TRUE);
   cinfo.do_fancy_upsampling = FALSE;
   cinfo.do_block_smoothing = FALSE;
   cinfo.dct_method = JDCT_IFAST;
   cinfo.dither_mode = JDITHER_ORDERED;

   if (ie->scale > 1)
     {
	cinfo.scale_num = 1;
	cinfo.scale_denom = ie->scale;
     }
   
/* head decoding */
   jpeg_calc_output_dimensions(&(cinfo));
   jpeg_start_decompress(&cinfo);
   
   w = cinfo.output_width;
   h = cinfo.output_height;
   
   if ((w != ie->w) || (h != ie->h))
     {
	jpeg_destroy_decompress(&cinfo);
	return 0;
     }
   
/* end head decoding */
/* data decoding */
   if (cinfo.rec_outbuf_height > 16)
     {
	jpeg_destroy_decompress(&cinfo);
	return 0;
     }
   data = alloca(w * 16 * 3);
   evas_cache_image_surface_alloc(ie, w, h);
   if (ie->flags.loaded)
     {
	jpeg_destroy_decompress(&cinfo);
	return 0;
     }
   ptr2 = evas_cache_image_pixels(ie);
   count = 0;
   prevy = 0;
   if (cinfo.output_components == 3)
     {
	for (i = 0; i < cinfo.rec_outbuf_height; i++)
	  line[i] = data + (i * w * 3);
	for (l = 0; l < h; l += cinfo.rec_outbuf_height)
	  {
	     jpeg_read_scanlines(&cinfo, line, cinfo.rec_outbuf_height);
	     scans = cinfo.rec_outbuf_height;
	     if ((h - l) < scans) scans = h - l;
	     ptr = data;
	     for (y = 0; y < scans; y++)
	       {
		  for (x = 0; x < w; x++)
		    {
		       *ptr2 =
			 (0xff000000) | ((ptr[0]) << 16) | ((ptr[1]) << 8) | (ptr[2]);
		       ptr += 3;
		       ptr2++;
		    }
	       }
	  }
     }
   else if (cinfo.output_components == 1)
     {
	for (i = 0; i < cinfo.rec_outbuf_height; i++)
	  line[i] = data + (i * w);
	for (l = 0; l < h; l += cinfo.rec_outbuf_height)
	  {
	     jpeg_read_scanlines(&cinfo, line, cinfo.rec_outbuf_height);
	     scans = cinfo.rec_outbuf_height;
	     if ((h - l) < scans) scans = h - l;
	     ptr = data;
	     for (y = 0; y < scans; y++)
	       {
		  for (x = 0; x < w; x++)
		    {
		       *ptr2 =
			 (0xff000000) | ((ptr[0]) << 16) | ((ptr[0]) << 8) | (ptr[0]);
		       ptr++;
		       ptr2++;
		    }
	       }
	  }
     }
/* end data decoding */
   jpeg_finish_decompress(&cinfo);
   jpeg_destroy_decompress(&cinfo);
   return 1;
}

#if 0 /* not used at the moment */
static int
evas_image_load_file_data_jpeg_alpha_internal(Image_Entry *ie, FILE *f)
{
   int w, h;
   struct jpeg_decompress_struct cinfo;
   struct _JPEG_error_mgr jerr;
   DATA8 *ptr, *line[16], *data;
   DATA32 *ptr2;
   int x, y, l, i, scans, count, prevy;

   if (!f) return 0;
   cinfo.err = jpeg_std_error(&(jerr.pub));
   jerr.pub.error_exit = _JPEGFatalErrorHandler;
   jerr.pub.emit_message = _JPEGErrorHandler2;
   jerr.pub.output_message = _JPEGErrorHandler;
   if (setjmp(jerr.setjmp_buffer))
     {
	jpeg_destroy_decompress(&cinfo);
	return 0;
     }
   jpeg_create_decompress(&cinfo);
   jpeg_stdio_src(&cinfo, f);
   jpeg_read_header(&cinfo, TRUE);
   cinfo.do_fancy_upsampling = FALSE;
   cinfo.do_block_smoothing = FALSE;
   jpeg_start_decompress(&cinfo);

/* head decoding */
   ie->w = w = cinfo.output_width;
   ie->h = h = cinfo.output_height;
/* end head decoding */
/* data decoding */
   if (cinfo.rec_outbuf_height > 16)
     {
	jpeg_destroy_decompress(&cinfo);
	return 0;
     }
   data = alloca(w * 16 * 3);
   if (!ie->flags.loaded)
     {
	jpeg_destroy_decompress(&cinfo);
	return 0;
     }
   ptr2 = evas_cache_image_pixels(ie);
   count = 0;
   prevy = 0;
   if (cinfo.output_components == 3)
     {
	for (i = 0; i < cinfo.rec_outbuf_height; i++)
	  line[i] = data + (i * w * 3);
	for (l = 0; l < h; l += cinfo.rec_outbuf_height)
	  {
	     jpeg_read_scanlines(&cinfo, line, cinfo.rec_outbuf_height);
	     scans = cinfo.rec_outbuf_height;
	     if ((h - l) < scans) scans = h - l;
	     ptr = data;
	     for (y = 0; y < scans; y++)
	       {
		  for (x = 0; x < w; x++)
		    {
		       *ptr2 =
			 ((*ptr2) & 0x00ffffff) |
			 (((ptr[0] + ptr[1] + ptr[2]) / 3) << 24);
		       ptr += 3;
		       ptr2++;
		    }
	       }
	  }
     }
   else if (cinfo.output_components == 1)
     {
	for (i = 0; i < cinfo.rec_outbuf_height; i++)
	  line[i] = data + (i * w);
	for (l = 0; l < h; l += cinfo.rec_outbuf_height)
	  {
	     jpeg_read_scanlines(&cinfo, line, cinfo.rec_outbuf_height);
	     scans = cinfo.rec_outbuf_height;
	     if ((h - l) < scans) scans = h - l;
	     ptr = data;
	     for (y = 0; y < scans; y++)
	       {
		  for (x = 0; x < w; x++)
		    {
		       *ptr2 =
			 ((*ptr2) & 0x00ffffff) |
			 ((ptr[0]) << 24);
		       ptr++;
		       ptr2++;
		    }
	       }
	  }
     }
/* end data decoding */
   jpeg_finish_decompress(&cinfo);
   jpeg_destroy_decompress(&cinfo);
   return 1;
}
#endif
/*============================================================================*
 *                          Emage Provider API                                *
 *============================================================================*/
Eina_Bool _jpg_loadable(const char *file)
{

}

Eina_Bool _jpg_info_load(const char *file, int *w, int *h, Enesim_Surface_Format *sfmt)
{
	int val;
	FILE *f;

	if ((!file))
		return 0;
	f = fopen(file, "rb");
	if (!f)
		return 0;
	val = evas_image_load_file_head_jpeg_internal(ie, f);
	fclose(f);
	return val;
	key = 0;
}

Eina_Bool _jpg_load(const char *file, Enesim_Surface *s)
{
	int val;
	FILE *f;

	if ((!file))
		return 0;
	f = fopen(file, "rb");
	if (!f)
		return 0;
	val = evas_image_load_file_data_jpeg_internal(ie, f);
	fclose(f);
	return val;
	key = 0;
}

static Emage_Provider _provider = {
	.name = "jpg",
	.type = EMAGE_PROVIDER_SW,
	.load = _jpg_load,
	.info_get = _jpg_info_load,
	.loadable = _jpg_loadable,
};
/*============================================================================*
 *                             Module API                                     *
 *============================================================================*/
Eina_Bool jpg_provider_init(void)
{
	return emage_provider_register(&_provider);
}

void jpg_provider_shutdown(void)
{
	emage_provider_unregister(&_provider);
}

EINA_MODULE_INIT(jpg_provider_init);
EINA_MODULE_SHUTDOWN(jpg_provider_shutdown);
