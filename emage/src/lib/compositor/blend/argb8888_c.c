#ifdef BUILD_C
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/

static void
blend_sl_pixel(void *src, void *mask, DATA32 col, void *dst, int offset, int len)
{
	/* p_dp */
   DATA32 *e = d + l;
   while (d < e) {
	l = 256 - (*s >> 24);
	*d = *s++ + MUL_256(l, *d);
	d++;
     }

	/* pas_dp */
   DATA32 *e = d + l;
   while (d < e)
     {
	switch (*s & 0xff000000)
	  {
	    case 0:
	      break;
	    case 0xff000000:
		*d = *s;
	      break;
	    default :
		l = 256 - (*s >> 24);
		*d = *s + MUL_256(l, *d);
	      break;
	  }
	s++;  d++;
     }

}

static void
blend_sl_color(void *src, void *mask, DATA32 col, void *dst, int offset, int len)
{
	Emage_Data_ARGB8888 *t;
	DATA32 *e;
	
	
	e = d + l, a = 256 - (c >> 24);
	while (d < e)
	{
		*d = c + MUL_256(a, *d);
		d++;
	}
}

static void
blend_sl_pixel_color(void *src, void *mask, DATA32 col, void *dst, int offset, int len)
{
   /* p_c_dp */
   DATA32 *e = d + l;
   while (d < e) {
	DATA32 sc = MUL4_SYM(c, *s);
	l = 256 - (sc >> 24);
	*d = sc + MUL_256(l, *d);
	d++;
	s++;
     }
   /* pan_c_dp */
   DATA32 *e = d + l;
   l = 256 - (c >> 24);
   while (d < e) {
	*d = ((c & 0xff000000) + MUL3_SYM(c, *s)) + MUL_256(l, *d);
	d++;
	s++;
     }
   /* p_can_dp */
   DATA32 *e = d + l;
   while (d < e) {
	l = 256 - (*s >> 24);
	*d = ((*s & 0xff000000) + MUL3_SYM(c, *s)) + MUL_256(l, *d);
	d++;
	s++;
     }
   /* pan_can_dp */
   DATA32 *e = d + l;
   while (d < e) {
	*d++ = 0xff000000 + MUL3_SYM(c, *s);
	s++;
     }
   /* p_caa_dp */
   DATA32 *e = d + l;
   c = 1 + (c & 0xff);
  while (d < e) {
	DATA32 sc = MUL_256(c, *s);
	l = 256 - (sc >> 24);
	*d = sc + MUL_256(l, *d);
	d++;
	s++;
     }
   /* pan_caa_dp */
   DATA32 *e = d + l;
   c = 1 + (c & 0xff);
   while (d < e) {
	*d = INTERP_256(c, *s, *d);
	d++;
	s++;
     }


}

static void
blend_sl_mask_color(void *src, void *mask, DATA32 col, void *dst, int offset, int len)
{
   /* mas_c_dp */
   DATA32 *e = d + l;
   l = 256 - (c >> 24);
   while (d < e) {
	DATA32 a = *m;
	switch(a)
	  {
	    case 0:
		break;
	    case 255:
		*d = c + MUL_256(l, *d);
		break;
	    default:
	      {
		DATA32 mc = MUL_SYM(a, c);
		a = 256 - (mc >> 24);
		*d = mc + MUL_256(a, *d);
	      }
		break;
	  }
	m++;  d++;
     }
	/* mas_can_dp */
   DATA32 *e = d + l;
   while (d < e) {
	l = *m;
	switch(l)
	  {
	    case 0:
		break;
	    case 255:
		*d = c;
		break;
	    default:
		l++;
		*d = INTERP_256(l, c, *d);
		break;
	  }
	m++;  d++;
     }

}

static void
blend_sl_pixel_mask(void *src, void *mask, DATA32 col, void *dst, int offset, int len)
{
   /* p_mas_dp */
   while (d < e) {
	l = *m;
	switch(l)
	  {
	    case 0:
		break;
	    case 255:
		l = 256 - (*s >> 24);
		*d = *s + MUL_256(l, *d);
		break;
	    default:
		c = MUL_SYM(l, *s);
		l = 256 - (c >> 24);
		*d = c + MUL_256(l, *d);
		break;
	  }
	m++;  s++;  d++;
     }
	/* pas_mas_dp`*/
   DATA32 *e = d + l;
   while (d < e) {
	l = (*s >> 24);
	switch(*m & l)
	  {
	    case 0:
		break;
	    case 255:
		*d = *s;
		break;
	    default:
		c = MUL_SYM(l, *s);
		l = 256 - (c >> 24);
		*d = c + MUL_256(l, *d);
		break;
	  }
	m++;  s++;  d++;
     }
   /* pan_mas_dp */
   DATA32 *e = d + l;
   while (d < e) {
	l = *m;
	switch(l)
	  {
	    case 0:
		break;
	    case 255:
		*d = *s;
		break;
	    default:
		l++;
		*d = INTERP_256(l, *s, *d);
		break;
	  }
	m++;  s++;  d++;
     }

}

static void
blend_pt_pixel(void *src, void *mask, DATA32 col, void *dst, int offset)
{
	c = 256 - (s >> 24);
	*d = s + MUL_256(c, *d);
}

static void
blend_pt_color(void *src, void *mask, DATA32 col, void *dst, int offset)
{
	Emage_Data_ARGB8888 *t;
	
	s = 256 - (c >> 24);
	*d = c + MUL_256(s, *d);
}

static void
blend_pt_pixel_color(void *src, void *mask, DATA32 col, void *dst, int offset)
{
	s = MUL4_SYM(c, s);
	c = 256 - (s >> 24);
	*d = s + MUL_256(c, *d);

}

static void
blend_pt_mask_color(void *src, void *mask, DATA32 col, void *dst, int offset)
{
	/* mas_c_dp */
	s = MUL_SYM(m, c);
	m = 256 - (s >> 24);
	*d = s + MUL_256(m, *d);
	/* mas_can_dp */
	*d = INTERP_256(m + 1, c, *d);

}
static void
blend_pt_pixel_mask(void *src, void *mask, DATA32 col, void *dst, int offset)
{
	/* p_mas_dp */
	s = MUL_SYM(m, s);
	c = 256 - (s >> 24);
	*d = s + MUL_256(c, *d);
	/* pan_mas_dp */
	*d = INTERP_256(m + 1, s, *d);
}

#endif
