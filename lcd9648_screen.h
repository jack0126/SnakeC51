	
  
#ifndef __LCD9648_SCREEN_H__
#define __LCD9648_SCREEN_H__

#include "base_type.h"
#include "lcd9648_graphics.h"

#define LCD9648_SCREEN_WIDTH	96
#define LCD9648_SCREEN_HEIGHT	48
#define LCD9648_AREA_COUNT		6	

void LCD9648_Init			(void);
void LCD9648_SetPixel		(int8 x, int8 y, int8 bLight);
int8 LCD9648_GetPixel		(int8 x, int8 y);
void LCD9648_Clear			(void);	
void LCD9648_DrawFill		(uint8 val);	 
void LCD9648_FillRectangle	(int8 x, int8 y, int8 w, int8 h, int8 bLight);
void LCD9648_Flush			(void);

#endif