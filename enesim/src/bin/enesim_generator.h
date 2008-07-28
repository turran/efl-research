#ifndef ENESIM_GENERATOR_H_
#define ENESIM_GENERATOR_H_

#include "common.h"

extern const char *type_names[TYPES];
extern const char *color_names[COLORS];
extern const int argb_offsets[COLORS];
extern const char *rop_names[ROPS];
extern const char *pixel_types[PIXELS];

extern FILE *fout;

Format * format_from_format(Enesim_Surface_Format fmt);
void rop_functions(Format *sf, Format *df);
void core_functions(Format *f);
void drawer_functions(Format *f);
void data_parameters(Format *f);

#endif /*ENESIM_GENERATOR_H_*/
