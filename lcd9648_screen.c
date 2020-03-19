
#include <REGX51.H>
#include <intrins.h>	
#include <string.h>
#include "lcd9648_screen.h"

sbit LCD9648_CS0 = P0^0;		//Ƭѡ
sbit LCD9648_RST = P0^1;		//��λ
sbit LCD9648_RS  = P2^7; 		//��������ѡ���
sbit LCD9648_SCL = P2^6;		//SPIʱ�Ӷ�
sbit LCD9648_SDA = P2^5;		//SPI���ݶ�

static uint8 xdata mScreenMapBuf[LCD9648_AREA_COUNT][LCD9648_SCREEN_WIDTH];//��Ļ�ڴ�ӳ��

static const uint8 BitPos[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
/*
void Delay10ms(uint16 c)   //��� 0us
{
    uint8 a,b;
    for(;c>0;c--)
		for(b=38;b>0;b--)
			for(a=130;a>0;a--);
}
*/
static void LCD9648_SendDataSPI(uint8 dat)
{  
	uint8 i;
   
	for(i = 0; i < 8; i++)			
	{  
		if(dat & 0x80)
		{
			LCD9648_SDA = 1;
		}
        else 
		{
			LCD9648_SDA = 0;
		}
      	dat <<= 1;

	  	LCD9648_SCL = 0;
		LCD9648_SCL = 1;			
	}
}
  
static void LCD9648_WriteComm(uint8 i)
{
    LCD9648_CS0 = 0;
	LCD9648_RS  = 0;

	LCD9648_SendDataSPI(i);

	LCD9648_CS0 = 1;
}

static void LCD9648_WriteData(uint8 i)
{
    LCD9648_CS0 = 0;
	LCD9648_RS  = 1;

	LCD9648_SendDataSPI(i);

	LCD9648_CS0 = 1;
}

void LCD9648_Init()
{
	uint16 i, j;
	LCD9648_RST=1;  
	for (i = 0; i < 1000; i++);
	
	LCD9648_RST=0;
	for (i = 0; i < 1000; i++);

	LCD9648_RST=1;
	for (i = 0; i < 1000; i++);

	LCD9648_WriteComm(0xe2);		  	//�����λ
	LCD9648_WriteComm(0xc8);		  	//0xA0�η���ѡ����������0xA1Ϊ������
	LCD9648_WriteComm(0xa0);		  	//0xC8��ͨ����ѡ��ѡ����0xC0Ϊ��������
	LCD9648_WriteComm(0x2f);
	LCD9648_WriteComm(0x26);
	LCD9648_WriteComm(0x81);		  	//������Աȶ�
	LCD9648_WriteComm(0x10);		  	//0x10�����иߵ�ַ 
	LCD9648_WriteComm(0xaf);		  	//������ʾ
	//LCD9648_WriteComm(0xa4);
	
	for(i = 0; i < 9; i++)				//һ����ҳ			   
	{
		LCD9648_WriteComm(0x40);  		//set scroll line 		���ù���һ��
		LCD9648_WriteComm(0xb0 | i);	//set page address		����ҳ��ַ
		LCD9648_WriteComm(0x10);  		//column  msb		    �����λ
		LCD9648_WriteComm(0x00);  		//column  lsb		    �����λ
		
		for(j = 0; j < LCD9648_SCREEN_WIDTH; j++)
		{
			LCD9648_WriteData(0);	
		}
	}

	LCD9648_Clear();
}

void LCD9648_SetPixel(int8 x, int8 y, int8 bLight)
{	
	if (bLight)							 			
	{									 			
		mScreenMapBuf[y >> 3][x] |= BitPos[y & 7];	
	}									 			
	else								 			
	{									 			
		mScreenMapBuf[y >> 3][x] &= ~BitPos[y & 7];	
	}
}

int8 LCD9648_GetPixel(int8 x, int8 y)
{
	return (mScreenMapBuf[y >> 3][x] & BitPos[y & 7]);	
}

void LCD9648_Clear()
{
	LCD9648_DrawFill(0);	
}

void LCD9648_DrawFill(uint8 val)
{
	memset(mScreenMapBuf, val, sizeof(mScreenMapBuf));		
}

void LCD9648_FillRectangle(int8 x, int8 y, int8 w, int8 h, int8 bLight)
{
	int8 i, j, tx, ty;
	for (i = 0; i < w; i++)
	{
		tx = x + i;
		for (j = 0; j < h; j++)
		{
			ty = y + j;
			LCD9648_SetPixel(tx, ty, bLight);	
		}
	}	
}

void LCD9648_Flush()
{
	int8 x, a;
	for(a = 0; a < LCD9648_AREA_COUNT; a++)			   
	{
		LCD9648_WriteComm(0x40);  			//set scroll line 		���ù���һ��
		LCD9648_WriteComm(0xb0 | a);		//set page address		����ҳ��ַ
		LCD9648_WriteComm(0x10);  			//column  msb		    �����λ
		LCD9648_WriteComm(0x00);  			//column  lsb		    �����λ
		
		for(x = 0; x < LCD9648_SCREEN_WIDTH; x++)
		{
			LCD9648_WriteData(mScreenMapBuf[a][x]);	
		}
	}		
}
