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
			printf("BAR AREA\n");
			return;
		}
		/* blocks */
		else
		{
			Block *block = NULL;
			int row, column;

			row = be->y / BLOCKH;
			column = be->x / BLOCKW;
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
		}
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

	l = ekeko_type_instance_new(level_type_get());

	ball = ball_new(c);
	prv = PRIVATE(l);
	prv->balls = eina_list_append(prv->balls, ball);
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
}

Level * simplelevel(Eon_Canvas *c)
{
	Level *l;
	Level_Private *prv;
	Block *b;

	l = level_new(c);
	prv = PRIVATE(l);

	b = (Block *)normalblock_new(c, 10, 5);
	level_block_add(l, b, 10, 5);

	b = (Block *)normalblock_new(c, 10, 10);
	level_block_add(l, b, 10, 10);

	return l;
}
