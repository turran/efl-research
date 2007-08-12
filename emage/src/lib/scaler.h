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

#endif
