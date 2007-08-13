#ifndef _SCALER_H
#define _SCALER_H

/** 
 * Scaler
 */
void emage_scaler_init(void);
void emage_scaler_sampled_init(void);
void emage_scaler_smooth_init(void);


void dummy_scale(Emage_Surface *src, Emage_Surface *dst, Emage_Rectangle srect, Emage_Rectangle drect, Emage_Draw_Context *dc);

typedef void (*Emage_Scaler_Func)(Emage_Surface *src, Emage_Surface *dst, Emage_Rectangle srect, Emage_Rectangle drect, Emage_Draw_Context *dc);

typedef struct _Emage_Scaler
{
	Emage_Scaler_Func func;
} Emage_Scaler;

extern Emage_Scaler Emage_Scalers[EMAGE_SCALER_TYPES];

/* source rect coordinate: src
 * source rect length: srl
 * dest rect coordinate: drc
 * dest rect length: drl
 * dest clip coordinate: dcc
 * dest clip length: dcl
 * source surface coordinate: sc
 * source surface length: sl
 * dest surface coordinate: dc
 * dest surface length: dl
 */
#define SANITISE(src, srl, drc, drl, dcc, dcl, sc, sl, dc, dl) 	\
if (src < 0) 							\
{ 								\
	drc -= (src * drl) / srl; 				\
	drl += (src * drl) / srl; 				\
	srl += src; 						\
	src = 0; 						\
} 								\
if (src >= sl) return; 						\
if ((src + srl) > sl) 						\
{ 								\
	drl = (drl * (sl - src)) /  (srl); 			\
	srl = sl - src; 					\
} 								\
if (drl <= 0) return; 						\
if (srl <= 0) return; 						\
if (dcc < 0) 							\
{ 								\
	dcl += dcc; 						\
	dcc = 0; 						\
} 								\
if (dcl <= 0) return; 						\
if (dcc >= dl) return; 						\
if (dcc < drc) 							\
{ 								\
	dcl += (dcc - drc); 					\
	dcc = drc; 						\
} 								\
if ((dcc + dcl) > dl) 						\
{ 								\
	dcl = dl - dcc; 					\
} 								\
if (dcl <= 0) return;

#endif
