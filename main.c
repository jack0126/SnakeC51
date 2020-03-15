
//#include <reg52.h>
//#include <stdlib.h>
//#include <absacc.h>   
#include "keyboard.h"
#include "lcd9648_screen.h"
//#include "lcd9648_graphics.h"
#include "game.h"
	
void main()
{									   
	uint16 key;
	static int16 timer;
 	
	LCD9648_Init();
	Game_DrawBackground();	  

	while(1)
	{		
		key = Keyboard_ReadKey();
		switch (Keyboard_GetKeyCode(key))
		{
			case KEY_VK_K1: if (Keyboard_GetKeyState(key)) Game_Start(); break;
			case KEY_VK_K2: break;
			case KEY_VK_K3: Game_OnUp(); break;
			case KEY_VK_K4: break;
			case KEY_VK_K5: if (Keyboard_GetKeyState(key)) Game_Over(); break;
			case KEY_VK_K6: Game_OnLeft(); break;
			case KEY_VK_K7: Game_OnDown(); break;
			case KEY_VK_K8: Game_OnRight(); break;
		}

		if (0 == (timer & 0x0FFF))//Ë¢ÐÂÊ±¼ä 
		{
			Game_OnceWalk();
			LCD9648_Flush();
			timer = 0;
		}
		timer++;
	}		
}
		
									  