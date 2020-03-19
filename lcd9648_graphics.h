
  
#ifndef __LCD9648_GRAPHICS_H__
#define __LCD9648_GRAPHICS_H__

#include "base_type.h"

//void LCD9648_DrawString(int8 x, int8 y, char *str);
void LCD9648_PrintMemory(int8 x, int8 y, uint8 *mem, int8 len);

#endif