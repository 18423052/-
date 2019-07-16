#include "OLED.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"
/*************************************************************************/
/*��������: GPIOģ��SPI�˿ڳ�ʼ��                                         */
/*************************************************************************/
void OLED_SPI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE);//ʹ��PA�˿�ʱ��
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; //�˿�����
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//�������
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOA,&GPIO_InitStructure);//�����趨������ʼ��GPIOA
}
 
/*************************************************************************/
/*��������: ͨ��SPIO���ģ��SPIͨ��Э��,��ģ��(SSD1306)д��һ���ֽ�                                  */
/*��ڲ�����                                                                                                                          */ 
/*                      data��Ҫд�������/����                                                                       */             
/*                      cmd ������/�����־ 0,��ʾ����;1,��ʾ����;                   */
/*************************************************************************/
void SPI_WriteByte(unsigned char data,unsigned char cmd)
{
    unsigned char i=0;
    OLED_DC =cmd;
    OLED_CLK=0;
    for(i=0;i<8;i++)
    {
        OLED_CLK=0;
        if(data&0x80)OLED_MOSI=1; //�Ӹ�λ����λ
        else OLED_MOSI=0;
        OLED_CLK=1;
        data<<=1;
    }
    OLED_CLK=1;
    OLED_DC=1;
}
/*************************************************************************/
/*��������: д����                                                        */
/*************************************************************************/
void WriteCmd(unsigned char cmd)
{
    SPI_WriteByte(cmd,OLED_CMD);
}
/*************************************************************************/
/*��������: д����                                                        */
/*************************************************************************/
void WriteData(unsigned char data)
{
    SPI_WriteByte(data,OLED_DATA);
}
//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127    
//[1]0 1 2 3 ... 127    
//[2]0 1 2 3 ... 127    
//[3]0 1 2 3 ... 127    
//[4]0 1 2 3 ... 127    
//[5]0 1 2 3 ... 127    
//[6]0 1 2 3 ... 127    
//[7]0 1 2 3 ... 127           
u8 OLED_GRAM[128][8];   
 
/*************************************************************************/
/*��������: ����ʱ                                                        */
/*************************************************************************/
void OLED_DLY_ms(unsigned int ms)
{                         
  unsigned int a;
  while(ms)
  {
    a=1335;
    while(a--);
    ms--;
  }
}
/*************************************************************************/
/*��������: ��ʼ��OLEDģ��                                                */
/*************************************************************************/
void OLED_Init(void)
{
    OLED_SPI_Init();
    OLED_CLK = 1;
    OLED_RST = 0;
    OLED_DLY_ms(100);
    OLED_RST = 1;
 
    //���ϵ絽���濪ʼ��ʼ��Ҫ���㹻��ʱ�䣬���ȴ�RC��λ���
    WriteCmd(0xAE);         // Display Off (0x00)
    WriteCmd(0xD5);
    WriteCmd(0x80);         // Set Clock as 100 Frames/Sec
    WriteCmd(0xA8);
    WriteCmd(0x3F);         // 1/64 Duty (0x0F~0x3F)
    WriteCmd(0xD3);
    WriteCmd(0x00);         // Shift Mapping RAM Counter (0x00~0x3F)
    WriteCmd(0x40 | 0x00);  // Set Mapping RAM Display Start Line (0x00~0x3F)
    WriteCmd(0x8D);
    WriteCmd(0x10 | 0x04);  // Enable Embedded DC/DC Converter (0x00/0x04)
    WriteCmd(0x20);
    WriteCmd(0x02);         // Set Page Addressing Mode (0x00/0x01/0x02)
    WriteCmd(0xA0 | 0x01);  // Set SEG/Column Mapping    
    WriteCmd(0xC0);  // Set COM/x Scan Direction   
    WriteCmd(0xDA);
    WriteCmd(0x02 | 0x10);  // Set Sequential Configuration (0x00/0x10)
    WriteCmd(0x81);
    WriteCmd(0xCF);         // Set SEG Output Current
    WriteCmd(0xD9);
    WriteCmd(0xF1);         // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    WriteCmd(0xDB);
    WriteCmd(0x40);         // Set VCOM Deselect Level
    WriteCmd(0xA4 | 0x00);  // Disable Entire Display On (0x00/0x01)
    WriteCmd(0xA6 | 0x00);  // Disable Inverse Display On (0x00/0x01)
    WriteCmd(0xAE | 0x01);  // Display On (0x01)
 
    OLED_Clear();  //��ʼ����   
}
/*************************************************************************/
/*��������: ��OLED�������л���                                            */
/*************************************************************************/
void OLED_ON(void)
{
    WriteCmd(0X8D);  //���õ�ɱ�
    WriteCmd(0X14);  //������ɱ�
    WriteCmd(0XAF);  //OLED����
}
/*************************************************************************/
/*��������: ��OLED���� -- ����ģʽ��,OLED���Ĳ���10uA                      */
/*************************************************************************/
void OLED_OFF(void)
{
    WriteCmd(0X8D);  //���õ�ɱ�
    WriteCmd(0X10);  //�رյ�ɱ�
    WriteCmd(0XAE);  //OLED����
}
 
/*************************************************************************/
/*��������: �����Դ浽OLED                                                 */
/*************************************************************************/
void OLED_Refresh_Gram(void)
{
    u8 i,n;         
    for(i=0;i<8;i++)  
    {  
        WriteCmd(0xb0+i);   //����ҳ��ַ��0~7��
        WriteCmd(0x00);      //������ʾλ�á��е͵�ַ
        WriteCmd(0x10);      //������ʾλ�á��иߵ�ַ   
        for(n=0;n<128;n++)WriteData(OLED_GRAM[n][i]); 
    }   
}
/*************************************************************************/
/*��������: ����                                                          */
/*************************************************************************/
void OLED_Clear(void)  
{  
    u8 i,n;  
    for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
    OLED_Refresh_Gram();//������ʾ
}
/*************************************************************************/
/*��������: ����                                                          */
/*��ڲ�����                                                             */
/*                      x��������       0~127                               */
/*                      y��������               0~63                     */
/*                      dot:0,���;1,���                                                                                */             
/*************************************************************************/
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
    u8 pos,bx,temp=0;
    if(x>127||y>63)return;//������Χ��.
    pos=7-y/8;
    bx=y%8;
    temp=1<<(7-bx);
    if(t)OLED_GRAM[x][pos]|=temp;
    else OLED_GRAM[x][pos]&=~temp;     
}
/*************************************************************************/
/*��������: ���                                                          */
/*��ڲ�����                                                                                                                          */
/*                      x1,y1,x2,y2 �������ĶԽ�����                              */
/*                      ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63                                         */
/*                      dot:0,���;1,���                                                                                */             
/*************************************************************************/
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
    u8 x,y;  
    for(x=x1;x<=x2;x++)
    {
        for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
    }                                                       
    OLED_Refresh_Gram();//������ʾ
}
 
/*************************************************************************/
/*��������: ��ָ��λ����ʾһ���ַ�,���������ַ�                             */
/*��ڲ�����                                                                                                                          */
/*                      x:0~12                                                     */
/*                      y:0~63                                                                      */
/*                      mode:0,������ʾ;1,������ʾ                                                       */ 
/*            size:ѡ������ 16/12                                        */
/*************************************************************************/
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{                   
    u8 temp,t,t1;
    u8 y0=y;
    u8 csize=(size/8+((size%8)?1:0))*(size/2);      //�õ�����һ���ַ���Ӧ������ռ���ֽ���
    chr=chr-' ';//�õ�ƫ�ƺ��ֵ        
    for(t=0;t<csize;t++)
    {   
        if(size==12)temp=asc2_1206[chr][t];         //����1206����
        else if(size==16)temp=asc2_1608[chr][t];    //����1608����
        else if(size==24)temp=asc2_2412[chr][t];    //����2412����
        else return;                                //û�е��ֿ�
        for(t1=0;t1<8;t1++)
        {
            if(temp&0x80)OLED_DrawPoint(x,y,mode);
            else OLED_DrawPoint(x,y,!mode);
            temp<<=1;
            y++;
            if((y-y0)==size)
            {
                y=y0;
                x++;
                break;
            }
        }    
    }          
}
//m^n����
u32 mypow(u8 m,u8 n)
{
    u32 result=1;    
    while(n--)result*=m;    
    return result;
}                 
/*************************************************************************/
/*��������: ��ʾ2������                                                   */
/*��ڲ�����                                                             */
/*                      x,y :�������                                     */
/*                      len :���ֵ�λ��                                   */
/*                      size:�����С                                        */ 
/*            mode:ģʽ   0,���ģʽ;1,����ģʽ                             */
/*            num:��ֵ(0~4294967295)                                     */
/*************************************************************************/       
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{           
    u8 t,temp;
    u8 enshow=0;                           
    for(t=0;t<len;t++)
    {
        temp=(num/mypow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
                continue;
            }else enshow=1; 
 
        }
        OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
    }
} 
/*************************************************************************/
/*��������: ��ʾ�ַ���                                                                            */
/*��ڲ�����                                                                                                                          */
/*                      x,y:�������                                                */
/*                      size:�����С                                                          */
/*                      *p:�ַ�����ʼ��ַ                                                               */ 
/*************************************************************************/
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
{   
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size,1);    
        x+=size/2;
        p++;
    }  
}  

void OLED_ShowGBK(u8 x, u8 y,  u8 num, u8 size,u8 mode)
    {
        u8 temp,t,t1;
        u8 y0=y;
    	//u8 size = 16;
        u8 csize=(size/8 + ((size%8)?1:0)) * size;     //�õ�����һ���ַ���Ӧ������ռ���ֽ���
                  
        for(t=0;t<csize;t++)
        {  
    		  //  ��ֻ������16��12������   û��������������
            if(size==12)      temp = gbk_1212[num][t];    //����1212����
            else if(size==16) temp = gbk_1616[num][t];    //����1616����
    //      else if(size==24)temp=asc2_2412[chr][t];    //����2412����
            else return;                                //û�е��ֿ�
    		for(t1=0;t1<8;t1++)
            {
                if(temp&0x80)OLED_DrawPoint(x,y,mode);
                else OLED_DrawPoint(x,y,!mode);
                temp<<=1;
                y++;
                if((y-y0)==size)
                {
                    y=y0;
                    x++;
                    break;
                }
            }    
        } 	
    }
		
		void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)OLED_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else OLED_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 



/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
	unsigned int j=0;
	unsigned char x,y;

	if( y1 % 8 == 0 ) 
		y = y1/8;      
	else 
		y = y1/8+1;
	for(y=y0; y<y1; y++)
	{
		OLED_Set_Pos(x0,y);
		for(x=x0; x<x1; x++)
		{      
			SPI_WriteByte(BMP[j++],OLED_DATA);	    	
		}
	}
} 

void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	SPI_WriteByte(0xb0+y,OLED_CMD);
	SPI_WriteByte(((x&0xf0)>>4)|0x10,OLED_CMD);
	SPI_WriteByte((x&0x0f)|0x01,OLED_CMD); 
}   	




//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 	
//u8 OLED_GRAM[128][8];	
//ͼƬ��OLED���󣬵���⣬������PCtoLCD2002�����ģ�
//������õ�ȡģ��ʽͬOLEDʵ��ASCII��ȡģ��ʽһģһ��������128*64��С��һ�����ֵ���ռ��1KB��
/*
	* 1. 128*64����OLEDģ�鹦����ʾ������ֱ�������heltec.taobao.com���۲�Ʒ;
    * 2. �ֱ��ɴ�������еġ�ȡ�����������ó�;
    * 3. ȡ�ַ�ʽ -- ����������ʽ���������
*/
//��SSD1106д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
//void OLED_WR_Byte(u8 dat,u8 cmd)
//{	
//	u8 i;			  
//	if(cmd)
//	  OLED_DC_Set();
//	else 
//	  OLED_DC_Clr();		  
//	OLED_CS_Clr();
//	for(i=0;i<8;i++)
//	{			  
//		OLED_SCLK_Clr();
//		if(dat&0x80)
//		   OLED_SDIN_Set();
//		else 
//		   OLED_SDIN_Clr();
//		OLED_SCLK_Set();
//		dat<<=1;   
//	}				 		  
//	OLED_CS_Set();
//	OLED_DC_Set();   	  
//} 

//void OLED_Set_Pos(unsigned char x, unsigned char y) 
//{ 
//	OLED_WR_Byte(0xb0+y,OLED_CMD);
//	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
//	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
//}   	  
////����OLED��ʾ    
//void OLED_Display_On(void)
//{
//	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
//	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
//	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
//}
////�ر�OLED��ʾ     
//void OLED_Display_Off(void)
//{
//	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
//	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
//	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
//}		   			 
////��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
//void OLED_Clear(void)  
//{  
//	u8 i,n;		    
//	for(i=0;i<8;i++)  
//	{  
//		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
//		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
//		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
//		for(n=0;n<128;n++)
//			OLED_WR_Byte(0,OLED_DATA); 
//	} //������ʾ
//}

////��ָ��λ����ʾһ���ַ�,���������ַ�
////x:0~127
////y:0~63
////mode:0,������ʾ;1,������ʾ				 
////size:ѡ������ 16/12 
//void OLED_ShowChar(u8 x,u8 y,u8 chr)
//{      	
//	unsigned char c=0,i=0;	
//		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
//		if(x>Max_Column-1){x=0;y=y+2;}
//		if(SIZE ==16)
//			{
//			OLED_Set_Pos(x,y);	
//			for(i=0;i<8;i++)
//			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
//			OLED_Set_Pos(x,y+1);
//			for(i=0;i<8;i++)
//			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
//			}
//			else {	
//				OLED_Set_Pos(x,y+1);
//				for(i=0;i<6;i++)
//				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
//			}
//}
////m^n����
//u32 oled_pow(u8 m,u8 n)
//{
//	u32 result=1;	 
//	while(n--)result*=m;    
//	return result;
//}				  
////��ʾ2������
////x,y :�������	 
////len :���ֵ�λ��
////size:�����С
////mode:ģʽ	0,���ģʽ;1,����ģʽ
////num:��ֵ(0~4294967295);	 		  
//void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
//{         	
//	u8 t,temp;
//	u8 enshow=0;						   
//	for(t=0;t<len;t++)
//	{
//		temp=(num/oled_pow(10,len-t-1))%10;
//		if(enshow==0&&t<(len-1))
//		{
//			if(temp==0)
//			{
//				OLED_ShowChar(x+(size/2)*t,y,' ');
//				continue;
//			}else enshow=1; 
//		 	 
//		}
//	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0'); 
//	}
//} 
////��ʾһ���ַ��Ŵ�
//void OLED_ShowString(u8 x,u8 y,u8 *chr)
//{
//	unsigned char j=0;
//	while (chr[j]!='\0')
//	{		
//		OLED_ShowChar(x,y,chr[j]);
//		x+=8;
//		if(x>120){x=0;y+=2;}
//			j++;
//	}
//}
////��ʾ����
//void OLED_ShowCHinese(u8 x,u8 y,u8 no)
//{      			    
//	u8 t,adder=0;
//	OLED_Set_Pos(x,y);	
//    for(t=0;t<16;t++)
//	{
//		OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
//		adder+=1;
//	}	
//	OLED_Set_Pos(x,y+1);	
//    for(t=0;t<16;t++)
//	{	
//		OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
//		adder+=1;
//	}					
//}
///***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
//void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
//{ 	
//	unsigned int j=0;
//	unsigned char x,y;

//	if( y1 % 8 == 0 ) 
//		y = y1/8;      
//	else 
//		y = y1/8+1;
//	for(y=y0; y<y1; y++)
//	{
//		OLED_Set_Pos(x0,y);
//		for(x=x0; x<x1; x++)
//		{      
//			OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
//		}
//	}
//} 


////��ʼ��SSD1306					    
//void OLED_Init(void)
//{ 	
//	//IO�����ã��˴���Ҫ�޸�
//	RCC->APB2ENR|=1<<4;   	//ʹ��PORTCʱ��	   	 
//	RCC->APB2ENR|=1<<2;   	//ʹ��PORTAʱ��	   
//	GPIOA->CRL&=0XFF0FF0FF;
//	GPIOA->CRL|=0X00300300;
//	GPIOC->CRH&=0X000FFFFF;
//	GPIOC->CRH|=0x22200000;  //Ƶ������ܳ���50MHZ
//	PWR->CR|=1<<8;           //����д��RTC�ͺ󱸼Ĵ���
//	RCC->BDCR&=0XFFFFFFFE;   //�ⲿ32KHZ�����ر�
//	BKP->CR&=0XFFFFFFFE;     //������TAMPER������Ϊͨ��IO��ʹ��
//	PWR->CR&=0XFFFFFEFF;     //��ֹд��RTC�ͺ󱸼Ĵ���
//	//�޸Ľ�ֹ
//	
//	//ȫ������
//	OLED_CS=1;
//	OLED_RST=1;
//	OLED_DC=1;
//	OLED_SCLK=1;
//	OLED_SDIN=1;

//	OLED_RST_Set();
//	delay_ms(100);
//	OLED_RST_Clr();
//	delay_ms(100);
//	OLED_RST_Set(); 
//					  
//	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
//	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
//	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
//	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
//	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
//	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
//	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
//	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
//	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
//	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
//	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
//	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
//	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
//	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
//	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
//	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
//	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
//	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
//	OLED_WR_Byte(0x12,OLED_CMD);
//	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
//	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
//	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
//	OLED_WR_Byte(0x02,OLED_CMD);//
//	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
//	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
//	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
//	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
//	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
//	
//	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
//	OLED_Clear();
//	OLED_Set_Pos(0,0); 	
//	
//} 
//void HeadLine1(u8 y)		//��������ʾ
//{
//	OLED_ShowCHinese(10,y,0);	//˫��ѭ��ͣ��
//	OLED_ShowCHinese(28,y,1);	//
//	OLED_ShowCHinese(46,y,2);	//
//	OLED_ShowCHinese(64,y,3);	//
//	OLED_ShowCHinese(82,y,4);	//
//	OLED_ShowCHinese(100,y,5);	//
//} 
//void HeadLine2(u8 y)		//��������ʾ
//{
////	
////	OLED_ShowCHinese(54,y,10);	//ͣ
////	OLED_ShowCHinese(72,y,11);	//��
////	OLED_ShowCHinese(72,y,12);	//ϵ
////	OLED_ShowCHinese(54,y,13);	//ͳ
//}

//void Start(u8 y)	//��ʼ
//{
//	OLED_ShowCHinese(46,y,8);	//��
//	OLED_ShowCHinese(64,y,9);	//ʼ
//}
//void About(u8 y)	//����
//{
//	OLED_ShowCHinese(46,y,10);	
//	OLED_ShowCHinese(64,y,11);	
//}
//void Thank(u8 y)	//��л
//{
//	OLED_ShowCHinese(46,y,12);	
//	OLED_ShowCHinese(64,y,13);	
//}
////void FuncName4(u8 y)
////{
////	OLED_ShowCHinese(54,y,8);	//��
////	OLED_ShowCHinese(72,y,9);	//��
////}
////void FuncName5(u8 y)
////{
////	OLED_ShowCHinese(54,y,10);	//԰
////	OLED_ShowCHinese(72,y,11);	//��
////}
////void SettingName(u8 y)
////{
////	OLED_ShowCHinese(54,y,6);	//��
////	OLED_ShowCHinese(72,y,7);	//��
////}
//void HangClear(u8 y)	//�����y��
//{
//	OLED_ShowString(0,y,"                ");
//}
//void OLED_Menu(void)
//{
//	pFunc[0](0);
//	pFunc[1](3);
//	pFunc[2](6);
//}
//void Menu_Init(void)
//{
//	pFunc[0] = HeadLine1;	//����ָ��ָ������ܣ���ʾʱͨ����������ָ��ʵ�ֹ�����ʾ
//	pFunc[1] = Start;
//	pFunc[2] = About;
//	pFunc[3] = Thank;
//}
