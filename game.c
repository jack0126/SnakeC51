							   
#include <intrins.h>
#include <stdlib.h>
#include <string.h>  
#include "game.h"	  
#include "lcd9648_screen.h"

#define GAME_STATE_INIT			0x80
#define GAME_STATE_PLAYING		0x40
#define GAME_STATE_OVER			0x20

typedef struct
{
	int8 x;
	int8 y;	
} Game_Node;
 				
typedef enum
{
	Left, Up, Right, Down
} Game_Direction;

typedef struct
{
	Game_Node xdata *snake;
	uint8 snakeSize;
	Game_Node food;
	Game_Direction direction;
	Game_Direction nextDirection; 
	int8 state;
} Game_Context;
				   
static void 		Game_Init				(void);
static void 		Game_GenerateFood		(void);
static Game_Node*	Game_FindNodeFromSnake  (int8 x, int8 y);
static void 		Game_DrawNode			(Game_Node *node, int8 bLight);
static void 		Game_DrawSnake			(void);

#define _ANY_HEAP_SIZE	448	

static uint8 xdata _any_heap[_ANY_HEAP_SIZE];
 			 
static Game_Context context;

static void Game_Init()
{	
	LCD9648_Clear();
	Game_DrawBackground();
	memset(_any_heap, 0, sizeof(_any_heap));

	context.snake = _any_heap;
	context.snake[0].x = 1;						 
	context.snake[0].y = 5;
	context.snake[1].x = 2;
	context.snake[1].y = 5;
	context.snakeSize = 2;
	Game_DrawSnake();

	Game_GenerateFood();
	Game_DrawNode(&context.food, 1);

	context.direction = Right;
	context.nextDirection = Right;
	context.state = GAME_STATE_INIT;
}

static void Game_GenerateFood()
{
	while(1)
	{
		context.food.x = (rand() % 22) + 1;
		context.food.y = (rand() % 10) + 1;
		if (Game_FindNodeFromSnake(context.food.x, context.food.y) == NULL)
		{
			break;
		}
	}
}

static Game_Node *Game_FindNodeFromSnake(int8 x, int8 y)
{
	uint8 pos = 0;
	while (pos < context.snakeSize)
	{
		if (context.snake[pos].x == x && context.snake[pos].y == y)
		{
			return &context.snake[pos];
		}
		pos++;
	}
	return NULL;
}

static void Game_DrawSnake()
{
	uint8 pos = 0;
	while (pos < context.snakeSize)
	{
		Game_DrawNode(&context.snake[pos], 1);
		pos++;
	}
}

static void Game_DrawNode(Game_Node *node, int8 bLight)
{
	int8 x, y, sx, sy;

	for (sx = 0; sx < 4; sx++)
	{
		x = (node->x << 2) + sx;
		for (sy = 0; sy < 4; sy++)
		{
			y = (node->y << 2) + sy;
			if (bLight) 
			{
				LCD9648_SetPixel(x, y, sx == 0 || sx == 3 || sy == 0 || sy == 3);
			}
			else
			{
				LCD9648_SetPixel(x, y, 0);
			}
		}
	}
}
		
void Game_DrawBackground()
{
	int8 x, y;
	for (x = 0; x < LCD9648_SCREEN_WIDTH; x++)
	{
		for (y = 0; y < LCD9648_SCREEN_HEIGHT; y++)
		{
			if (x < 4 || x > 91 || y < 4 || y > 43)
			{
				LCD9648_SetPixel(x, y, 1);
			}
		}
	}	
}

void Game_Start()
{	
	if (context.state != GAME_STATE_PLAYING) 
	{
		Game_Init();
		context.state = GAME_STATE_PLAYING;
	}
}

void Game_Over()
{	
	LCD9648_DrawFill(0x18);
	context.state = GAME_STATE_OVER;
}

void Game_OnLeft()
{
	if (context.direction != Right) context.nextDirection = Left;
}

void Game_OnUp()
{
	if (context.direction != Down) context.nextDirection = Up;
}

void Game_OnRight()
{
	if (context.direction != Left) context.nextDirection = Right;
}

void Game_OnDown()
{
	if (context.direction != Up) context.nextDirection = Down;
}

void Game_OnceWalk()
{	
	int8 nx, ny;
	uint8 pos, firstPos;

	if (context.state != GAME_STATE_PLAYING)
	{
		return;
	}
	
	//计算蛇头的下一个座标
	context.direction = context.nextDirection; 
	firstPos = context.snakeSize - 1;
	nx = context.snake[firstPos].x;
	ny = context.snake[firstPos].y;
	switch (context.direction)
	{
		case Left:  nx--; break;
		case Up:    ny--; break;
		case Right: nx++; break;
		case Down:  ny++; break;
	}
	
	if (nx == context.food.x && ny == context.food.y)
	{
		//遇到食物 
		memcpy(&context.snake[context.snakeSize], &context.food, sizeof(Game_Node));
		context.snakeSize++;

		Game_GenerateFood();
		Game_DrawNode(&context.food, 1);
	}	
	else if (nx < 1 || nx > 22 || ny < 1 || ny > 10 ||	//撞墙了
			 Game_FindNodeFromSnake(nx, ny) != NULL)	//吃到自己 
	{
		Game_Over();
	}
	else 
	{	
		//往前爬一步
		Game_DrawNode(context.snake, 0);//擦除蛇尾

		pos = 1;
		while(pos < context.snakeSize)
		{
			memcpy(&context.snake[pos - 1], &context.snake[pos], sizeof(Game_Node));
			pos++;
		}
		
		context.snake[firstPos].x = nx;
		context.snake[firstPos].y = ny;
		Game_DrawNode(&context.snake[firstPos], 1);//画蛇头
	}	 
}
							  