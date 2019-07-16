#include "led.h"
#include "delay.h"
#include "sys.h"
#include "tim.h"
#include "OLED.h"
#include "adc.h"
#include "usart.h"

//ALIENTEK Mini STM32�����巶������8
//PWM���ʵ��   
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾

 int main(void)
 {	
 	u8 t;
	u8 len;	
	u16 times=0; 
 
	delay_init();	    	 //��ʱ������ʼ��	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
	LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ� 
	GPIO_WriteBit(GPIOA,GPIO_Pin_11,(BitAction)(1));
	while(1)
	{
		
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			printf("\r\n�����͵���ϢΪ:\r\n");
			for(t=0;t<len;t++)
			{
				USART1->DR=USART_RX_BUF[t];
				while((USART1->SR&0X40)==0);//�ȴ����ͽ���
			}
			printf("\r\n\r\n");//���뻻��
			USART_RX_STA=0;
		}else
		{
			times++;
			if(times%5000==0)
			{
				printf("test\n");
			}
			if(times%200==0)printf("����������,�Իس�������\r\n");  
			if(times%30==0)GPIO_WriteBit(GPIOC,GPIO_Pin_13,(BitAction)(1-GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_13))); //��˸LED,��ʾϵͳ��������.
			delay_ms(10);   
		}
	}	 
	
}






























// int main(void)
// {	   
//	delay_init();	    	 //��ʱ������ʼ��	  
//	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
//	TIM3_PWM_Init(899,0);//����Ƶ��PWMƵ��=72000/(899+1)=80Khz 
//   	while(1)
//	{			 
//		TIM_SetCompare2(TIM3,450);	   
//	} 
//}

