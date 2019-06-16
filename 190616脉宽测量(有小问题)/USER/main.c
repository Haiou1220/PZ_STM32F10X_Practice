#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"	 


//uint16_t arr = 899;  
//uint16_t psc = 0; //����Ƶ,PWM Ƶ��=72000/900=80Khz
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
 u8  TIM2CH1_CAPTURE_STA;		//���벶��״̬		    				
 u16	TIM2CH1_CAPTURE_VAL;	//���벶��ֵ	

void led_VariPWM_init(uint16_t arr,uint16_t psc);//PB5	->LED��
void chang_pwm(void); //�ı�Ƚ�ֵ���ı�pwm�Ŀ��
void  Cap_init(uint16_t arr ,uint16_t psc);   //tim2���벶��init

 
 int main(void)
 {	

	 	u32 temp=0; 


//		/*led�� IO�˿�����*/	 
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	    //ʹ��PC13,--D2	->	PC13	->LED��

//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;			    //LED
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //�������
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
//		GPIO_Init(GPIOC, &GPIO_InitStructure);			     //��ʼ��GPIO
//		GPIO_SetBits(GPIOC,GPIO_Pin_13);					// �����
	 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 

		
		
		
	 led_VariPWM_init( 719, 100);//PB5	->LED��////����Ƶ,PWM Ƶ��=72000/720=100Khz->1kHZ
	  delay_init();//tick ��ʱ����ʱinit
		 uart_init(9600);//����init
		TIM_SetCompare2(TIM3,0);//�Ƚ�ֵ //80k����
	
  Cap_init(0XFFFF,72-1);  ////��1Mhz��Ƶ�ʼ��� ???-1us
  while(1)
	{
	    



	

		delay_ms(10);
		TIM_SetCompare2(TIM3,TIM_GetCapture2(TIM3)+1);

		if(TIM_GetCapture2(TIM3)==300)TIM_SetCompare2(TIM3,0);	
		 		 
 		if(TIM2CH1_CAPTURE_STA&0X80)//�ɹ�������һ��������
		{
			temp=TIM2CH1_CAPTURE_STA&0X3F;
			temp*=65536;//���ʱ���ܺ�
			temp+=TIM2CH1_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
			printf("HIGH:%d us\r\n",temp);//��ӡ�ܵĸߵ�ƽʱ��
			TIM2CH1_CAPTURE_STA=0;//������һ�β���
		
		}
		
		
		
	}
	
 }
 
 
 
 
 void led_VariPWM_init(uint16_t arr,uint16_t psc)//PB5	->LED��
 {
 
	   GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	 
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
		TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ
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
 
 
 
 
 }
 
 void chang_pwm(void)
{		
//	static u8 dir;
//	static u16 led_PB5_pwmval;
//	
//	if(dir) 
//	{
//		led_PB5_pwmval++;
//	}else led_PB5_pwmval--;
//		
//	if(led_PB5_pwmval>200)
//	{
//		dir=0;
//	}	else if(led_PB5_pwmval==0) dir=1;
//	
//	
//	TIM_SetCompare2(TIM3,led_PB5_pwmval);//�Ƚ�ֵ
		

				{//��->����ѭ��
					TIM_SetCompare2(TIM3,TIM_GetCapture2(TIM3)+1);
			if(TIM_GetCapture2(TIM3)==300)
				TIM_SetCompare2(TIM3,0);
				}	
}

void  Cap_init(uint16_t arr,uint16_t psc)  //tim2���벶��init
{
	
	
//	 u8  TIM2CH1_CAPTURE_STA;		//���벶��״̬		    				
// u16	TIM2CH1_CAPTURE_VAL;	//���벶��ֵ	
//	
	
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
		TIM_ICInitTypeDef TIM_ICInitStruct;
		GPIO_InitTypeDef GPIO_InitStruct;
		NVIC_InitTypeDef NVIC_InitStruc;
	
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//������ʱ��2ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//����Tim2_ch1����������PA15ʱ��
	
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD; // �ߵ�ƽ��Ч
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15; //im2_ch1����������PA15ʱ��
		GPIO_Init( GPIOA, &GPIO_InitStruct);
		GPIO_ResetBits(GPIOA,GPIO_Pin_15);//Ĭ��״̬����
	
		TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ��
		TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInitStruct.TIM_Period =arr;
		TIM_TimeBaseInitStruct.TIM_Prescaler =psc;
		TIM_TimeBaseInit( TIM2, &TIM_TimeBaseInitStruct);//��ʱ��2init
	
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;//tim2_ch1-->pa15
	TIM_ICInitStruct.TIM_ICFilter = 0x0;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;//������
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;//����Ƶ
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit( TIM2, & TIM_ICInitStruct);//���벶��init
	
	//�����ж����ȼ�����
	 NVIC_InitStruc.NVIC_IRQChannel = TIM2_IRQn;
	 NVIC_InitStruc.NVIC_IRQChannelCmd =  ENABLE;
	 NVIC_InitStruc.NVIC_IRQChannelPreemptionPriority = 2;
	 NVIC_InitStruc.NVIC_IRQChannelSubPriority = 0;//��ռ���ȼ� 2 ��,//�����ȼ� 0 ��
	 NVIC_Init(& NVIC_InitStruc);

   TIM_ITConfig( TIM2,  TIM_IT_Update|TIM_IT_CC1, ENABLE);//ʹ�� �����ж��� �����ж�
	
	TIM_Cmd(TIM2,ENABLE ); //ʹ�ܶ�ʱ��
}


void TIM2_IRQHandler(void)
{
//	extern u16 TIM2CH1_CAPTURE_TIME ;
//	extern u16	TIM2CH1_CAPTURE_VAL ;	//���벶��ֵ
//	extern u8 Flag_TIM2_CAPTURE_OK ; //�ɹ������־λ
//	
//	struct bs{		//��̬����λ��
//				unsigned char capture_ok:1;
//				unsigned char rising_flagbit:1;
//        unsigned char falling_flagbit: 1;
//        unsigned char UpdateOver_time: 5;
//    } static tim2IRQ_sta = { 0, 0, 0};
//		


//	if((tim2IRQ_sta.capture_ok)!= SET)//��һ���жϣ�δ�ɹ���������	
//	{
//				// �и����ж�
//				if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
//				{
//						if(tim2IRQ_sta.rising_flagbit==SET)//�Ѿ����񵽸ߵ�ƽ��
//						{
//										if(tim2IRQ_sta.UpdateOver_time>=0X1F)//�ߵ�ƽ̫����
//										{ 
//											tim2IRQ_sta.capture_ok = SET;//��ǳɹ�������һ��
//											Flag_TIM2_CAPTURE_OK  = SET;
//														{
//														tim2IRQ_sta.capture_ok = 0;
//														tim2IRQ_sta.falling_flagbit = 0;
//														tim2IRQ_sta.rising_flagbit = 0;
//														
//														}
//											TIM2CH1_CAPTURE_VAL = 0XFFFF;
//										}else tim2IRQ_sta.UpdateOver_time++;
//						}
//				}

//			
//				// �в����ж� 
//			if (TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET) 
//			{
//											
//						if(tim2IRQ_sta.rising_flagbit == SET)		//���ڲ���һ���½��� ��֮ǰ�����������أ�		
//						{	  			
//							tim2IRQ_sta.capture_ok = SET ;		//��ǳɹ�����һ�θߵ�ƽ����
//							Flag_TIM2_CAPTURE_OK  = SET;
//								{
//								tim2IRQ_sta.capture_ok = 0;
//								tim2IRQ_sta.falling_flagbit = 0;
//								tim2IRQ_sta.rising_flagbit = 0;
//								
//								}
//							TIM2CH1_CAPTURE_VAL=TIM_GetCapture1(TIM2); //��ü�����ֵ
//							TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
//						}
//						else //��һ�β���������						
//						{
//							
//							//���tim2IRQ_sta��TIM5CH1_CAPTURE_VAL�����rising_flagbit 
//							tim2IRQ_sta.capture_ok = 0;
//							tim2IRQ_sta.falling_flagbit = 0;
//							tim2IRQ_sta.UpdateOver_time = 0;
//							TIM2CH1_CAPTURE_VAL=0;
//							
//							TIM_SetCounter(TIM2,0);//��ռ�����
//							tim2IRQ_sta.rising_flagbit = 1; //��ǲ�����������
//							TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
//						}		    		
//						
//			}
	

if((TIM2CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{	  
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		{	    
			if(TIM2CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM2CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM2CH1_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM2CH1_CAPTURE_VAL=0XFFFF;
				}else TIM2CH1_CAPTURE_STA++;
			}	 
		}
		
		
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)//����1���������¼�
		{	
			if(TIM2CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM2CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
				TIM2CH1_CAPTURE_VAL=TIM_GetCapture1(TIM2);
		   		TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}else  								//��δ��ʼ,��һ�β���������
			{
				TIM2CH1_CAPTURE_STA=0;			//���
				TIM2CH1_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM2,0);
				TIM2CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
		   		TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			}		    
		}			     	    					   
 	}
 
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
	
	
	
		


	
	}

	
// TIM2CH1_CAPTURE_TIME = 	tim2IRQ_sta.UpdateOver_time ;
//TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update);//����жϺͲ����־λ

//}

