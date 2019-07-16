#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"
#define OLED_CMD 0   //��������
#define OLED_DATA 1 //��������

#define OLED_CLK    PAout(5)    // CLKʱ��    d0
#define OLED_MOSI   PAout(7)    // MOSI     d1
#define OLED_RST    PAout(6)    // RET��λ    ret
#define OLED_DC     PAout(4)    // ����|����    dc  ��0��������1�������ݣ�

void OLED_SPI_Init(void); //����MCU��SPI
void SPI_WriteByte(uint8_t addr,uint8_t data); //��Ĵ�����ַдһ��byte������
void WriteCmd(unsigned char cmd); //д����
void WriteDat(unsigned char data); //д����

void OLED_Init(void);//��ʼ��OLED
void OLED_ON(void);//����OLED
void OLED_OFF(void);//OLED����
void OLED_Refresh_Gram(void);//�����Դ浽OLED
void OLED_Clear(void);//����
void OLED_DrawPoint(u8 x,u8 y,u8 t);//����
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);//���
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);//��ʾ�ַ�
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);//��ʾ2������
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size);//��ʾ�ַ���
void OLED_ShowGBK(u8 x, u8 y,  u8 num, u8 size,u8 mode);//��ʾ����
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);
void OLED_Set_Pos(unsigned char x, unsigned char y);
#endif



