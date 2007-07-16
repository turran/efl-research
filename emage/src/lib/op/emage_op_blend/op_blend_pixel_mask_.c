
/* blend pixel x mask --> dst */

#ifdef BUILD_C
static void
_op_blend_p_mas_dp(DATA32 *s, DATA8 *m, DATA32 c, DATA32 *d, int l) {
   DATA32 *e = d + l;
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
}

static void
_op_blend_pas_mas_dp(DATA32 *s, DATA8 *m, DATA32 c, DATA32 *d, int l) {
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
}

static void
_op_blend_pan_mas_dp(DATA32 *s, DATA8 *m, DATA32 c, DATA32 *d, int l) {
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


#define _op_blend_p_mas_dpan _op_blend_p_mas_dp
#define _op_blend_pas_mas_dpan _op_blend_pas_mas_dp
#define _op_blend_pan_mas_dpan _op_blend_pan_mas_dp

static void
init_blend_pixel_mask_span_funcs_c(void)
{
   op_blend_span_funcs[SP][SM_AS][SC_N][DP][CPU_C] = _op_blend_p_mas_dp;
   op_blend_span_funcs[SP_AS][SM_AS][SC_N][DP][CPU_C] = _op_blend_pas_mas_dp;
   op_blend_span_funcs[SP_AN][SM_AS][SC_N][DP][CPU_C] = _op_blend_pan_mas_dp;

   op_blend_span_funcs[SP][SM_AS][SC_N][DP_AN][CPU_C] = _op_blend_p_mas_dpan;
   op_blend_span_funcs[SP_AS][SM_AS][SC_N][DP_AN][CPU_C] = _op_blend_pas_mas_dpan;
   op_blend_span_funcs[SP_AN][SM_AS][SC_N][DP_AN][CPU_C] = _op_blend_pan_mas_dpan;
}
#endif

#ifdef BUILD_C
static void
_op_blend_pt_p_mas_dp(DATA32 s, DATA8 m, DATA32 c, DATA32 *d) {
	s = MUL_SYM(m, s);
	c = 256 - (s >> 24);
	*d = s + MUL_256(c, *d);
}

static void
_op_blend_pt_pan_mas_dp(DATA32 s, DATA8 m, DATA32 c, DATA32 *d) {
	*d = INTERP_256(m + 1, s, *d);
}

#define _op_blend_pt_pas_mas_dp _op_blend_pt_p_mas_dp

#define _op_blend_pt_p_mas_dpan _op_blend_pt_p_mas_dp
#define _op_blend_pt_pas_mas_dpan _op_blend_pt_pas_mas_dp
#define _op_blend_pt_pan_mas_dpan _op_blend_pt_pan_mas_dp

static void
init_blend_pixel_mask_pt_funcs_c(void)
{
   op_blend_pt_funcs[SP][SM_AS][SC_N][DP][CPU_C] = _op_blend_pt_p_mas_dp;
   op_blend_pt_funcs[SP_AS][SM_AS][SC_N][DP][CPU_C] = _op_blend_pt_pas_mas_dp;
   op_blend_pt_funcs[SP_AN][SM_AS][SC_N][DP][CPU_C] = _op_blend_pt_pan_mas_dp;

   op_blend_pt_funcs[SP][SM_AS][SC_N][DP_AN][CPU_C] = _op_blend_pt_p_mas_dpan;
   op_blend_pt_funcs[SP_AS][SM_AS][SC_N][DP_AN][CPU_C] = _op_blend_pt_pas_mas_dpan;
   op_blend_pt_funcs[SP_AN][SM_AS][SC_N][DP_AN][CPU_C] = _op_blend_pt_pan_mas_dpan;
}
#endif

/*-----*/

/* blend_rel pixel x mask -> dst */

#ifdef BUILD_C
static void
_op_blend_rel_p_mas_dp(DATA32 *s, DATA8 *m, DATA32 c, DATA32 *d, int l) {
   DATA32 *e = d + l;
   while (d < e) {
	c = MUL_SYM(*m, *s);
	l = 256 - (c >> 24);
	*d = MUL_SYM(*d >> 24, c) + MUL_256(l, *d);
	d++;
	m++;
      	s++;
     }
}

#define _op_blend_rel_pas_mas_dp _op_blend_rel_p_mas_dp
#define _op_blend_rel_pan_mas_dp _op_blend_rel_p_mas_dp

#define _op_blend_rel_p_mas_dpan _op_blend_p_mas_dpan
#define _op_blend_rel_pas_mas_dpan _op_blend_pas_mas_dpan
#define _op_blend_rel_pan_mas_dpan _op_blend_pan_mas_dpan

static void
init_blend_rel_pixel_mask_span_funcs_c(void)
{
   op_blend_rel_span_funcs[SP][SM_AS][SC_N][DP][CPU_C] = _op_blend_rel_p_mas_dp;
   op_blend_rel_span_funcs[SP_AS][SM_AS][SC_N][DP][CPU_C] = _op_blend_rel_pas_mas_dp;
   op_blend_rel_span_funcs[SP_AN][SM_AS][SC_N][DP][CPU_C] = _op_blend_rel_pan_mas_dp;

   op_blend_rel_span_funcs[SP][SM_AS][SC_N][DP_AN][CPU_C] = _op_blend_rel_p_mas_dpan;
   op_blend_rel_span_funcs[SP_AS][SM_AS][SC_N][DP_AN][CPU_C] = _op_blend_rel_pas_mas_dpan;
   op_blend_rel_span_funcs[SP_AN][SM_AS][SC_N][DP_AN][CPU_C] = _op_blend_rel_pan_mas_dpan;
}
#endif

#ifdef BUILD_C
static void
_op_blend_rel_pt_p_mas_dp(DATA32 s, DATA8 m, DATA32 c, DATA32 *d) {
 	s = MUL_SYM(m, s);
	c = 256 - (s >> 24);
	*d = MUL_SYM(*d >> 24, s) + MUL_256(c, *d);
}

#define _op_blend_rel_pt_pas_mas_dp _op_blend_rel_pt_p_mas_dp
#define _op_blend_rel_pt_pan_mas_dp _op_blend_rel_pt_p_mas_dp

#define _op_blend_rel_pt_p_mas_dpan _op_blend_pt_p_mas_dpan
#define _op_blend_rel_pt_pas_mas_dpan _op_blend_pt_pas_mas_dpan
#define _op_blend_rel_pt_pan_mas_dpan _op_blend_pt_pan_mas_dpan

static void
init_blend_rel_pixel_mask_pt_funcs_c(void)
{
   op_blend_rel_pt_funcs[SP][SM_AS][SC_N][DP][CPU_C] = _op_blend_rel_pt_p_mas_dp;
   op_blend_rel_pt_funcs[SP_AS][SM_AS][SC_N][DP][CPU_C] = _op_blend_rel_pt_pas_mas_dp;
   op_blend_rel_pt_funcs[SP_AN][SM_AS][SC_N][DP][CPU_C] = _op_blend_rel_pt_pan_mas_dp;

   op_blend_rel_pt_funcs[SP][SM_AS][SC_N][DP_AN][CPU_C] = _op_blend_rel_pt_p_mas_dpan;
   op_blend_rel_pt_funcs[SP_AS][SM_AS][SC_N][DP_AN][CPU_C] = _op_blend_rel_pt_pas_mas_dpan;
   op_blend_rel_pt_funcs[SP_AN][SM_AS][SC_N][DP_AN][CPU_C] = _op_blend_rel_pt_pan_mas_dpan;
}
#endif
