#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h>

#include "Emage.h"

void png_save(Emage_Surface *s, char *file, int compress);
double time_get(void);
void time_display(struct timeval ts, struct timeval te);
