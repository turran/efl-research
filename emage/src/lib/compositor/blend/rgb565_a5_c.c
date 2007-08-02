#ifdef BUILD_C

SL_FUNC(blend, rgb565_a5, c, pixel)
{

}

#else

#define rgb565_a5_c_sl_pixel 		dummy_sl
#define rgb565_a5_c_sl_color 		dummy_sl
#define rgb565_a5_c_sl_pixel_color 	dummy_sl
#define rgb565_a5_c_sl_mask_color 	dummy_sl
#define rgb565_a5_c_sl_pixel_mask 	dummy_sl
#define rgb565_a5_c_pt_color 		dummy_pt
#define rgb565_a5_c_pt_pixel_color 	dummy_pt
#define rgb565_a5_c_pt_mask_color 	dummy_pt
#define rgb565_a5_c_pt_pixel_mask 	dummy_pt

#endif
