#include "stm32f10x.h"
#include "delay.h"



uint16_t arr = 899;  
uint16_t psc = 0; //����Ƶ,PWM Ƶ��=72000/900=80Khz
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
	u8 dir;
	u16 led_PB5_pwmval;
	 
  GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	 
	/*led�� IO�˿�����*/	 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	    //ʹ��PC13,--D2	->	PC13	->LED��
	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;			    //LED
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);			     //��ʼ��GPIO
  GPIO_SetBits(GPIOC,GPIO_Pin_13);					// �����
	 
	/*pwm�� ����-��ʱ��ʹ��-AFIO*/	 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//�����˿ڸ���
	 GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);//��ʱ��3����remap-pwm��ch2��PB5
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //������ʱ��3����pwm����Ƚ�ͨ��2--�˿ڸ��õ�PB5
	 
	 
	 	/*pwm�µ�PB5--IO�˿�����*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	    //pwm���--�˿�PB5--ʱ��ʹ��
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;			    //pwm����Ƚ�ͨ��2--�˿ڸ��õ�PB5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	 //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);			     //��ʼ��GPIO
  GPIO_SetBits(GPIOB,GPIO_Pin_5);					// PB5�����
	 
	 
	 	/*��ʱ��3��arr��pscģʽ����*/
		

		TIM_TimeBaseInitStruct.TIM_Period = arr;//�Զ���װ��ֵ
		TIM_TimeBaseInitStruct.TIM_Prescaler = psc; ////����Ԥ��Ƶֵ
		TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
		TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
		
		TIM_TimeBaseInit( TIM3,  &TIM_TimeBaseInitStruct);
		
		
		/*��ʱ��3��pwmģʽ����*/

     
		TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;//ѡ�� PWM ģʽ 2
		TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable ;  //�Ƚ����ʹ�� 
		TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;    //������Ը�
		TIM_OCInitStruct.TIM_Pulse = 0;    //�Ƚ�ֵ
		TIM_OC2Init( TIM3,  &TIM_OCInitStruct);
		
		
		/*ʹ�� TIM3�����ñȽ�ֵarr*/
		TIM_OC2PreloadConfig( TIM3,  TIM_OCPreload_Enable);//ʹ��Ԥװ�ؼĴ���
	  TIM_ARRPreloadConfig( TIM3,  ENABLE); //�������ⲻ��//ʹ��arr�Զ���װ�أ�arpe=1,�л�����
		TIM_Cmd( TIM3,  ENABLE);//ʹ�� TIM3
		
	  delay_init();//tick ��ʱ����ʱinit
		//TIM_SetCompare2(TIM3,0);//�Ƚ�ֵ
  while(1)
	{
	    
	//	GPIO_ResetBits(GPIOC,GPIO_Pin_13);  
	 delay_ms(10);
		
			
		if(dir) 
		{
			led_PB5_pwmval++;
		}else led_PB5_pwmval--;
			
		if(led_PB5_pwmval>200)
		{
			dir=0;
		}	else if(led_PB5_pwmval==0) dir=1;
		
		
		//GPIO_SetBits(GPIOC,GPIO_Pin_13);
		TIM_SetCompare2(TIM3,led_PB5_pwmval);//�Ƚ�ֵ
		//delay_ms(10);
	}
	
 }
