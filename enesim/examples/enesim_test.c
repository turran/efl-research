#include "enesim_test.h"

/* TODO change this to use SDL instead of dumping the file to png */
#define DSTDIR "/tmp"

SDL_Surface *screen;
/*============================================================================*
 *                                 Common                                     * 
 *============================================================================*/
struct Rasterizer_Cb_Data
{
	Enesim_Renderer *r;
	Enesim_Surface *s;
};

#define TIGER_WIDTH 400
#define TIGER_HEIGHT 412

static Enesim_Surface * tiger_draw(void)
{
	Enesim_Surface *s;
	
	s = surface_new(TIGER_WIDTH, TIGER_HEIGHT, ENESIM_SURFACE_ARGB8888);
	png_load(s, PACKAGE_DATA_DIR"/tiger.png");
	
	return s;
}

static void screen_clear(void)
{
	SDL_FillRect(screen, NULL, 0xffffffff);
}

/*============================================================================*
 * Test3:                                                                     *
 * Surface draw test                                                          *
 *============================================================================*/
#define MATRIX_XX 0
#define MATRIX_XY 1
#define MATRIX_XZ 2
#define MATRIX_YX 3
#define MATRIX_YY 4
#define MATRIX_YZ 5
#define MATRIX_ZX 6
#define MATRIX_ZY 7
#define MATRIX_ZZ 8
#define MATRIX_SIZE 9

#define PI 3.14159265

static void transformation_compose(float *st, float *dt)
{
	int i;
	float tmp[MATRIX_SIZE];
	
	tmp[MATRIX_XX] = (st[MATRIX_XX] * dt[MATRIX_XX]) + (st[MATRIX_XY] * dt[MATRIX_YX]) + (st[MATRIX_XZ] * dt[MATRIX_ZX]);
	tmp[MATRIX_XY] = (st[MATRIX_XX] * dt[MATRIX_XY]) + (st[MATRIX_XY] * dt[MATRIX_YY]) + (st[MATRIX_XZ] * dt[MATRIX_ZY]);
	tmp[MATRIX_XZ] = (st[MATRIX_XX] * dt[MATRIX_XZ]) + (st[MATRIX_XY] * dt[MATRIX_YZ]) + (st[MATRIX_XZ] * dt[MATRIX_ZZ]);
	
	tmp[MATRIX_YX] = (st[MATRIX_YX] * dt[MATRIX_XX]) + (st[MATRIX_YY] * dt[MATRIX_YX]) + (st[MATRIX_YZ] * dt[MATRIX_ZX]);
	tmp[MATRIX_YY] = (st[MATRIX_YX] * dt[MATRIX_XY]) + (st[MATRIX_YY] * dt[MATRIX_YY]) + (st[MATRIX_YZ] * dt[MATRIX_ZY]);
	tmp[MATRIX_YZ] = (st[MATRIX_YX] * dt[MATRIX_XZ]) + (st[MATRIX_YY] * dt[MATRIX_YZ]) + (st[MATRIX_YZ] * dt[MATRIX_ZZ]);

	tmp[MATRIX_ZX] = (st[MATRIX_ZX] * dt[MATRIX_XX]) + (st[MATRIX_ZY] * dt[MATRIX_YX]) + (st[MATRIX_ZZ] * dt[MATRIX_ZX]);
	tmp[MATRIX_ZY] = (st[MATRIX_ZX] * dt[MATRIX_XY]) + (st[MATRIX_ZY] * dt[MATRIX_YY]) + (st[MATRIX_ZZ] * dt[MATRIX_ZY]);
	tmp[MATRIX_ZZ] = (st[MATRIX_ZX] * dt[MATRIX_XZ]) + (st[MATRIX_ZY] * dt[MATRIX_YZ]) + (st[MATRIX_ZZ] * dt[MATRIX_ZZ]);

	for (i = 0; i < MATRIX_SIZE; i++)
		st[i] = tmp[i];
}

static void transformation_translate(float *t, float tx, float ty)
{
	t[MATRIX_XX] = 1;
	t[MATRIX_XY] = 0;
	t[MATRIX_XZ] = tx;
	t[MATRIX_YX] = 0;
	t[MATRIX_YY] = 1;
	t[MATRIX_YZ] = ty;
	t[MATRIX_ZX] = 0;
	t[MATRIX_ZY] = 0;
	t[MATRIX_ZZ] = 1;	
}
static void transformation_scale(float *t, float sx, float sy)
{
	t[MATRIX_XX] = sx;
	t[MATRIX_XY] = 0;
	t[MATRIX_XZ] = 0;
	t[MATRIX_YX] = 0;
	t[MATRIX_YY] = sy;
	t[MATRIX_YZ] = 0;
	t[MATRIX_ZX] = 0;
	t[MATRIX_ZY] = 0;
	t[MATRIX_ZZ] = 1;
}
static void transformation_rotate(float *t, float rad)
{
	float c = cos(rad);
	float s = sin(rad);
	
	t[MATRIX_XX] = c;
	t[MATRIX_XY] = -s;
	t[MATRIX_XZ] = 0;
	t[MATRIX_YX] = s;
	t[MATRIX_YY] = c;
	t[MATRIX_YZ] = 0;
	t[MATRIX_ZX] = 0;
	t[MATRIX_ZY] = 0;
	t[MATRIX_ZZ] = 1;	
}

static void transformation_identity(float *t)
{
	t[MATRIX_XX] = 1;
	t[MATRIX_XY] = 0;
	t[MATRIX_XZ] = 0;
	t[MATRIX_YX] = 0;
	t[MATRIX_YY] = 1;
	t[MATRIX_YZ] = 0;
	t[MATRIX_ZX] = 0;
	t[MATRIX_ZY] = 0;
	t[MATRIX_ZZ] = 1;
}
/*============================================================================*
 * Test4:                                                                     *
 * Renderers Demo                                                            *
 *============================================================================*/
static void test4(void)
{
	int i;
	Enesim_Surface *dst;
	Enesim_Scanline_Alias sl[100];
	Enesim_Renderer *renderer;
	
	dst = surface_new(SCREEN_WIDTH, SCREEN_HEIGHT, ENESIM_SURFACE_ARGB8888_PRE);
	/* fill color renderer */
	renderer = enesim_fill_color_new();
	enesim_fill_color_color_set(renderer, 0xffff0000);
	enesim_renderer_rop_set(renderer, ENESIM_FILL);
	for (i = 0; i < 100; i++)
	{
		sl[i].x = 200;
		sl[i].y = i + 50;
		sl[i].w = 200;
		enesim_renderer_draw(renderer, ENESIM_SCANLINE_ALIAS, &sl[i], dst);
	}
	enesim_fill_color_color_set(renderer, 0x5500ff00);
	enesim_renderer_rop_set(renderer, ENESIM_BLEND);
	for (i = 0; i < 100; i++)
	{
		sl[i].x = 100;
		sl[i].y = i + 100;
		sl[i].w = 200;
		enesim_renderer_draw(renderer, ENESIM_SCANLINE_ALIAS, &sl[i], dst);
	}
	surface_blt(dst, screen);
	surface_free(dst);
}
/*============================================================================*
 * Test3:                                                                     *
 *                                                                            *
 *============================================================================*/
static void test3(void)
{
	Enesim_Surface *dst, *src;
	Enesim_Transformation *tx;
	Enesim_Rectangle srect;
	Enesim_Rectangle drect;
	float m1[9], m2[9];

	src = tiger_draw();
	dst = surface_new(TIGER_WIDTH, TIGER_HEIGHT, ENESIM_SURFACE_ARGB8888_PRE);
	
	srect.x = 0;
	srect.y = 0;
	enesim_surface_size_get(src, &srect.w, &srect.h);
	
	drect.x = 0;
	drect.y = 0;
	drect.w = srect.w;
	drect.h = srect.h;
	
	/* identity matrix */
	/* affine matrix */
	/* projective matrix */
	transformation_scale(m1, 2.0, 2.0);
	transformation_translate(m2, -20, -26);
	transformation_compose(m1, m2);
	transformation_rotate(m2, PI/4);
	transformation_compose(m1, m2);
	/*
	transformation_translate(m2, 200, 206);
	transformation_compose(m1, m2);*/
	
	//enesim_surface_transformation_set(src, m1);
	//enesim_surface_draw(src, &srect, dst, &drect, 0, 0);
	//png_save(dst, DSTDIR"/enesim_test3_dst.png", 0);
	//enesim_transformation_apply(tx, src, &srect, dst, &drect);
	surface_blt(src, screen);
	surface_free(dst);
	surface_free(src);
}
/*============================================================================*
 * Test2:                                                                     *
 * Rasterizers comparative                                                    *
 *============================================================================*/
#define STAR_WIDTH 400
#define STAR_HEIGHT 400

static void test2(void)
{
}

/*============================================================================*
 * Test1:                                                                     *
 * Fixed point calculations                                                   *
 *============================================================================*/
static void test1(void)
{
	enesim_16p16_t fp, res;
	float r;

	/* sqrt(2) */
	fp = enesim_16p16_int_from(2);
	res = enesim_16p16_sqrt(fp);
	r = enesim_16p16_float_to(res);
	printf("sqrt(2) = %f\n", r);
}

static void help(void)
{
	
}

int main(int argc, char **argv)
{
	int i = 1;
	int end = 0;
	SDL_Event event;
		
	if (argc < 2) return -1;
	
	SDL_Init(SDL_INIT_VIDEO);
	if (!(screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE)))
	{
		fprintf(stderr, "%s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	screen_clear();
	SDL_UpdateRect(screen, 0, 0, 0, 0);
	while (!end)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
				/* iterate over argv */
				if (i >= argc)
				{
					end = 1;
					break;
				}
				screen_clear();
				if (!strcmp(argv[i], "test1"))
					test1();
				else if (!strcmp(argv[i], "test2"))
					test2();
				else if (!strcmp(argv[i], "test3"))
					test3();
				else if (!strcmp(argv[i], "test4"))
					test4();
				else
				{
					end = 1;
					break;
				}
				SDL_UpdateRect(screen, 0, 0, 0, 0);
				i++;
				break;
				
				case SDL_QUIT:
				end = 1;
				break;
			}
		}
	}
	return 0;
}
