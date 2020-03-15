
  
#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "base_type.h"

#define KEY_VK_K1 		0xFE
#define KEY_VK_K2 		0xFD
#define KEY_VK_K3 		0xFB
#define KEY_VK_K4 		0xF7
#define KEY_VK_K5 		0xEF
#define KEY_VK_K6 		0xDF
#define KEY_VK_K7 		0xBF
#define KEY_VK_K8 		0x7F

#define KEY_VK_S1 		0xEE
#define KEY_VK_S2 		0xDE
#define KEY_VK_S3 		0xBE
#define KEY_VK_S4 		0x7E
#define KEY_VK_S5 		0xED
#define KEY_VK_S6 		0xDD
#define KEY_VK_S7 		0xBD
#define KEY_VK_S8 		0x7D
#define KEY_VK_S9 		0xEB
#define KEY_VK_S10 		0xDB
#define KEY_VK_S11 		0xBB
#define KEY_VK_S12 		0x7B
#define KEY_VK_S13 		0xE7
#define KEY_VK_S14 		0xD7
#define KEY_VK_S15 		0xB7
#define KEY_VK_S16 		0x77

#define Keyboard_KEY_STATE_PRESSED		0x0000
#define Keyboard_KEY_STATE_RELEASED		0x0100
#define Keyboard_GetKeyState(k)	   	   (0xFF00 & k)
#define Keyboard_GetKeyCode(k)		   (0x00FF & k)

int16 Keyboard_ReadKey		(void);	//读取K系按键
int16 Keyboard_ReadKeyEx	(void);	//读取S系按键

#endif