#ifndef _ERANOID_H
#define _ERANOID_H

#include "Eon.h"

#define MAXROWS 20
#define MAXCOLS 13
#define TOTROWS 26
#define BLOCKW 20
#define BLOCKH 10

#define ERANOID_OBSTACLE "Eranoid_Obstacle"
#define ERANOID_LEVEL "Eranoid_Level"
#define ERANOID_BALL "Eranoid_Ball"
#define ERANOID_BAR "Eranoid_Bar"
#define ERANOID_BLOCK "Eranoid_Block"
#define ERANOID_NORMALBLOCK "Eranoid_NormalBlock"
#define ERANOID_DURABLEBLOCK "Eranoid_DurableBlock"

typedef struct _Obstacle Obstacle;
typedef struct _Obstacle_Private Obstacle_Private;

typedef struct _Level Level;
typedef struct _Level_Private Level_Private;

typedef struct _Block Block;
typedef struct _Block_Private Block_Private;
typedef struct _NormalBlock NormalBlock;
typedef struct _NormalBlock_Private NormalBlock_Private;
typedef struct _DurableBlock DurableBlock;
typedef struct _DurableBlock_Private DurableBlock_Private;

typedef struct _Ball Ball;
typedef struct _Ball_Private Ball_Private;

typedef struct _Bar Bar;
typedef struct _Bar_Private Bar_Private;


#define BALL_EVENT "BallEvent"

struct _Ball
{

	Ekeko_Object base;
	Ball_Private *prv;
};

typedef struct _BallEvent
{
	Ekeko_Event ev;
	int x;
	int y;
	int w;
	int h;
} BallEvent;

#define BALL_GOES_RIGHT(d)  (d & BALL_RIGHT)
#define BALL_GOES_LEFT(d)   (~(d & BALL_RIGHT))
#define BALL_GOES_TOP(d)    (d & BALL_TOP)
#define BALL_GOES_BOTTOM(d) (~(d & BALL_TOP))

typedef enum _BallDirection
{
	BALL_TOP = 0x01,
	BALL_RIGHT = 0x10,
} BallDirection;

Ball * ball_new(Eon_Canvas *c);
void ball_update(Ball *b);
void ball_bounce_x(Ball *b);
void ball_bounce_y(Ball *b);
void ball_vel_set(Ball *b, float vel);
void ball_pos_set(Ball *b, int x, int y);


Ekeko_Type *obstacle_type_get(void);
struct _Obstacle
{
	Ekeko_Object base;
	Obstacle_Private *prv;
	void (*hit)(Obstacle *b, Ball *ball, Eina_Bool top);
};

struct _Bar
{
	Obstacle base;
	Bar_Private *prv;
};

Bar * bar_new(Eon_Canvas *c);


struct _Level
{
	Ekeko_Object base;
	Level_Private *prv;
};

static Level * level_new(Eon_Canvas *c);
void level_start(Level *l);
Level * simplelevel(Eon_Canvas *c);

Ekeko_Type *block_type_get(void);
struct _Block
{
	Obstacle base;
	Block_Private *prv;
};

struct _NormalBlock
{
	Block base;
	NormalBlock_Private *prv;
};

struct _DurableBlock
{
	Block base;
	DurableBlock_Private *prv;
};

#endif
