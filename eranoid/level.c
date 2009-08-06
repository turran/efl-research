#include <stdio.h>
#include "eranoid.h"

static Ekeko_Type *level_type_get(void);
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(l) ((Level_Private *)((Level *)(l))->prv)

struct _Level_Private
{
	Block *blocks[MAXROWS][MAXCOLS];
	Eina_List *balls;
	Eina_List *bars;
	int mx;
	int my;
	int numblocks;
};

static void bar_collision(Bar *b, Level *l, Ball *ball, BallEvent *be)
{
	int x, y, w, h;

	bar_geometry_get(b, &x, &y, &w, &h);
	if (be->y + be->w != y)
		return;
	//printf("Bar at %d %d %d %d, Ball at %d %d\n", x, y, w, h, be->x, be->y);
	//printf("%d %d %d %d\n", be->x, x - be->w + 1, be->x + be->w - 1, x + w);
	if ((be->x >= x - be->w + 1) && (be->x - 1 <= x + w))
		b->base.hit((Obstacle *)b, ball, EINA_TRUE);
}

static void block_collision(Level *l, Ball *b, BallEvent *be)
{
	Level_Private *prv = PRIVATE(l);
	Block *tb = NULL;
	Block *lr = NULL;
	Block *corner = NULL;
	int row, column;
	int cornerrow, cornercol;
	int dirlr, dirtb;

	cornerrow = row = be->y / BLOCKH;
	cornercol = column = be->x / BLOCKW;
	/* check direction */
	ball_direction_get(b, &dirtb, &dirlr);

	if ((dirlr > 0) && (column + 1 < MAXCOLS))
	{
		cornercol = column + 1;
		lr = prv->blocks[row][cornercol];
	}
	else if ((dirlr < 0) && (column -1 >= 0))
	{
		cornercol = column - 1;
		lr = prv->blocks[row][cornercol];
	}
	if ((dirtb > 0) && (row + 1 < MAXROWS))
	{
		cornerrow = row + 1;
		tb = prv->blocks[cornerrow][column];
	}
 	else if ((dirtb < 0) && (row - 1 >= 0))
	{
		cornerrow = row - 1;
		tb = prv->blocks[cornerrow][column];
	}
	corner = prv->blocks[cornerrow][cornercol];
	//printf("TB %d %d - LR %d %d - CORN %d %d\n", cornerrow, column, row, cornercol, cornerrow, cornercol);
	//printf("Ball at %d %d (%dx%d)\n", be->x, be->y, be->w, be->h);
	if (tb)
	{
		if ((be->y == (cornerrow + 1) * BLOCKH) || (be->y + be->h == cornerrow * BLOCKH))
			if (tb->base.hit)
			{
				tb->base.hit((Obstacle *)tb, b, EINA_TRUE);
			}

	}
	if (lr)
	{
		if ((be->x == (cornercol + 1) * BLOCKW) || (be->x + be->w == cornercol * BLOCKW))
			if (lr->base.hit)
			{
				lr->base.hit((Obstacle *)lr, b, EINA_FALSE);
			}
	}
	if (corner)
	{
		if (((be->y == (cornerrow + 1) * BLOCKH) || (be->y + be->h == cornerrow * BLOCKH)) && ((be->x == (cornercol + 1) * BLOCKW) || (be->x + be->w == cornercol * BLOCKW)))
			if (corner->base.hit)
			{
				printf("CORNER HITTTTED\n");
				corner->base.hit((Obstacle *)corner, b, EINA_FALSE);
				corner->base.hit((Obstacle *)corner, b, EINA_TRUE);
			}
	}
	/*
	 * tr
	 * [   ][ 1 ][ 2 ]
	 * [   ]  *  [ 3 ]
	 * [   ][   ][   ]
	 */

#if 0
	printf("Y = trying to find a block at %d %d!\n", row, column);
	if (be->y == row * BLOCKH)
	{
		printf("CASE 1\n");
		block = prv->blocks[row - 1][column];
	}
	if ((row + 1 < MAXROWS) && ((be->y + be->w) == (row + 1) * BLOCKH))
	{
		printf("CASE 2\n");
		block = prv->blocks[row + 1][column];
	}
	if (block && block->base.hit)
		block->base.hit((Obstacle *)block, b, EINA_TRUE);

	if (be->x == (column - 1) * BLOCKW)
	{
		printf("CASE 3\n");
		block = prv->blocks[row][column - 1];
	}
	if ((be->x + be->w) == (column + 1) * BLOCKW)
	{
		printf("CASE 4\n");
		block = prv->blocks[row][column + 1];
	}
	if (block && block->base.hit)
		block->base.hit((Obstacle *)block, b, EINA_FALSE);
#endif
}

/* get the current row and col, remove it from the array */
static void block_delete(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	Level *l = (Level *)data;
	Level_Private *prv = PRIVATE(l);
	Block *b = o;
	int row, col;

	block_position_get(b, &row, &col);
	printf("BLOCK DELETED at %d %d!!!!!\n", row, col);
	prv->blocks[row][col] = NULL;
}


static void ball_change(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	Ball *b = (Ball *)o;
	BallEvent *be = (BallEvent *)e;
	Level *l = data;
	Level_Private *prv = PRIVATE(l);

	/* inside playground */
	if ((be->x > 0 && be->x + be->w < prv->mx) && (be->y > 0 && be->y + be->h < prv->my))
	{
		/* bar area */
		if (be->y >= BLOCKH * MAXROWS)
		{
			Eina_List *tmp;
			Bar *bar;

			EINA_LIST_FOREACH(prv->bars, tmp, bar)
			{
				bar_collision(bar, l, b, be);
			}
		}
		/* blocks */
		else
			block_collision(l, b, be);
	}
	/* borders */
	else
	{
		if (be->y <= 0)
		{
			ball_bounce_y(b);
		}
		else if (be->y + be->h >= prv->my)
		{
			ball_bounce_y(b);
		}
		if (be->x <= 0)
		{
			ball_bounce_x(b);
		}
		else if (be->x + be->w >= prv->mx)
		{
			ball_bounce_x(b);
		}
	}
}

static void _ctor(void *instance)
{
	Level *l;
	Level_Private *prv;

	l = (Level *)instance;
	l->prv = prv = ekeko_type_instance_private_get(level_type_get(), instance);
	prv->mx = BLOCKW * MAXCOLS;
	prv->my = BLOCKH * TOTROWS;
}

static void _dtor(void *polygon)
{

}

static Ekeko_Type *level_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(ERANOID_LEVEL, sizeof(Level),
				sizeof(Level_Private), ekeko_object_type_get(),
				_ctor, _dtor, NULL);
	}

	return type;
}

static int level_ticker(void *data)
{
	Level *l = data;
	Level_Private *prv = PRIVATE(l);
	Eina_List *tmp;
	Ball *b;

	EINA_LIST_FOREACH(prv->balls, tmp, b)
	{
		ball_update(b);
	}
	return 1;
}

static Level * level_new(Eon_Canvas *c)
{
	Level *l;
	Level_Private *prv;
	Ball *ball;
	Bar *bar;

	l = ekeko_type_instance_new(level_type_get());

	ball = ball_new(c);
	bar = bar_new(c);

	prv = PRIVATE(l);
	prv->balls = eina_list_append(prv->balls, ball);
	prv->bars = eina_list_append(prv->bars, bar);

	/* add the callbacks to the ball, whenever it moves */
	//ekeko_event_listener_add((Ekeko_Object *)ball, BALL_EVENT_X, ball_x, EINA_FALSE, l);
	ekeko_event_listener_add((Ekeko_Object *)ball, BALL_EVENT, ball_change, EINA_FALSE, l);
	ecore_timer_add(1.0f/50.0f, level_ticker, l);
	//ecore_timer_add(1, level_ticker, l);

	return l;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void level_load(const char *file, Level *l)
{
	/* load the file and load the level which is EEEEN0EEN1EN1N2 ...
	 * E = Empty
	 * N = Normal block (0, 1, 2)
	 */

}

void level_generate(Level *l)
{
	int i, j;
	long int r;

	for (i = 0; i < MAXROWS; i++)
		for(j = 0; j < MAXCOLS; j++)
		{

		}
}

void level_start(Level *l)
{

}

void level_block_add(Level *l, Block *b, int row, int col)
{
	Level_Private *prv = PRIVATE(l);

	if (row > MAXROWS || col > MAXCOLS)
		return;

	prv->blocks[row][col] = b;
	prv->numblocks++;
	// add a callback whenever the object is being destroyed */
	ekeko_event_listener_add((Ekeko_Object *)b, EKEKO_EVENT_OBJECT_DELETE, block_delete, EINA_FALSE, l);
}

Level * simplelevel(Eon_Canvas *c)
{
	Level *l;
	Level_Private *prv;
	Block *b;

	l = level_new(c);
	prv = PRIVATE(l);
#if 1

	b = (Block *)normalblock_new(c, 10, 5);
	level_block_add(l, b, 10, 5);

	b = (Block *)normalblock_new(c, 10, 10);
	level_block_add(l, b, 10, 10);

	b = (Block *)normalblock_new(c, 3, 7);
	level_block_add(l, b, 3, 7);
#endif

	return l;
}
