#include "stm32f10x.h"


/************************************************
 ALIENTEK ս��STM32F103������ʵ��0
 ����ģ��
 ע�⣬�����ֲ��е��½������½�ʹ�õ�main�ļ� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/


 void Delay(u32 count)
 {
   u32 i=0;
   for(;i<count;i++);
 }
 int main(void)
 {	
  GPIO_InitTypeDef  GPIO_InitStructure;
	 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	    //ʹ��PC13,--D2	->	PC13	->LED��
	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;			    //LED
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);			     //��ʼ��GPIO
  GPIO_SetBits(GPIOC,GPIO_Pin_13);					// �����
	 

	 
	 
  while(1)
	{
	    
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);  
		Delay(3000000);
		
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		Delay(3000000);
	}
	
 }
