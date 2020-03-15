#include <REGX52.H>
#include "keyboard.h"

#define in 		(P1)	//独立键盘输入口
#define in_ex 	(P0)	//矩阵键盘输入口

int16 Keyboard_ReadKey()
{	
	static uint16 cacheKey = 0, k;
	uint8 press;

	press = in;
	if (press != 0xFF)
	{
		if (cacheKey == 0) 
		{
			cacheKey = 0x00FF & press;
		}
		return cacheKey;
	}
	
	if (cacheKey != 0)
	{
		k = cacheKey;
		cacheKey = 0;
		return (0x0100 | k);
	}

	return 0;
}

int16 Keyboard_ReadKeyEx()
{	
	static uint16 cacheKey = 0, k;
	uint8 h, l;

	in_ex = 0xF0;
	h = in_ex;
	if (h != 0xF0)
	{
		in_ex = 0x0F;
		l = in_ex;
		if(l != 0x0F)
		{	
			if (cacheKey == 0) 
			{
				cacheKey = 0x00FF & (h | l);
			}
			return cacheKey;
		}
	}
	
	if (cacheKey != 0)
	{
		k = cacheKey;
		cacheKey = 0;
		return (0x0100 | k);
	}

	return 0;
} 
