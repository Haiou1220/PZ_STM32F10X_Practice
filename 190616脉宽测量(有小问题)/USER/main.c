#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"	 


//uint16_t arr = 899;  
//uint16_t psc = 0; //不分频,PWM 频率=72000/900=80Khz
/************************************************
 ALIENTEK 战舰STM32F103开发板实验0
 工程模板
 注意，这是手册中的新建工程章节使用的main文件 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
 u8  TIM2CH1_CAPTURE_STA;		//输入捕获状态		    				
 u16	TIM2CH1_CAPTURE_VAL;	//输入捕获值	

void led_VariPWM_init(uint16_t arr,uint16_t psc);//PB5	->LED灯
void chang_pwm(void); //改变比较值，改变pwm的宽度
void  Cap_init(uint16_t arr ,uint16_t psc);   //tim2输入捕获init

 
 int main(void)
 {	

	 	u32 temp=0; 


//		/*led的 IO端口配置*/	 
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	    //使能PC13,--D2	->	PC13	->LED灯

//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;			    //LED
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //推挽输出
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
//		GPIO_Init(GPIOC, &GPIO_InitStructure);			     //初始化GPIO
//		GPIO_SetBits(GPIOC,GPIO_Pin_13);					// 输出高
	 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 

		
		
		
	 led_VariPWM_init( 719, 100);//PB5	->LED灯////不分频,PWM 频率=72000/720=100Khz->1kHZ
	  delay_init();//tick 定时器延时init
		 uart_init(9600);//串口init
		TIM_SetCompare2(TIM3,0);//比较值 //80k方波
	
  Cap_init(0XFFFF,72-1);  ////以1Mhz的频率计数 ???-1us
  while(1)
	{
	    



	

		delay_ms(10);
		TIM_SetCompare2(TIM3,TIM_GetCapture2(TIM3)+1);

		if(TIM_GetCapture2(TIM3)==300)TIM_SetCompare2(TIM3,0);	
		 		 
 		if(TIM2CH1_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
		{
			temp=TIM2CH1_CAPTURE_STA&0X3F;
			temp*=65536;//溢出时间总和
			temp+=TIM2CH1_CAPTURE_VAL;//得到总的高电平时间
			printf("HIGH:%d us\r\n",temp);//打印总的高点平时间
			TIM2CH1_CAPTURE_STA=0;//开启下一次捕获
		
		}
		
		
		
	}
	
 }
 
 
 
 
 void led_VariPWM_init(uint16_t arr,uint16_t psc)//PB5	->LED灯
 {
 
	   GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	 
 /*pwm的 配置-定时器使能-AFIO*/	 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//开启端口复用
	 GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);//定时器3部分remap-pwm的ch2到PB5
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //开启定时器3——pwm输出比较通道2--端口复用到PB5
	 
	 
	 	/*pwm下的PB5--IO端口配置*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	    //pwm输出--端口PB5--时钟使能
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;			    //pwm输出比较通道2--端口复用到PB5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	 //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);			     //初始化GPIO
  GPIO_SetBits(GPIOB,GPIO_Pin_5);					// PB5输出高
 
 
 	 
	 	/*定时器3的arr与psc模式配置*/
		

		TIM_TimeBaseInitStruct.TIM_Period = arr;//自动从装载值
		TIM_TimeBaseInitStruct.TIM_Prescaler = psc; ////设置预分频值
		TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//不分频
		TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
		
		TIM_TimeBaseInit( TIM3,  &TIM_TimeBaseInitStruct);
 
 /*定时器3的pwm模式配置*/

     
		TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;//选择 PWM 模式 2
		TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable ;  //比较输出使能 
		TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;    //输出极性高
		TIM_OCInitStruct.TIM_Pulse = 0;    //比较值
		TIM_OC2Init( TIM3,  &TIM_OCInitStruct);
		
		
		/*使能 TIM3与设置比较值arr*/
		TIM_OC2PreloadConfig( TIM3,  TIM_OCPreload_Enable);//使能预装载寄存器
	  TIM_ARRPreloadConfig( TIM3,  ENABLE); //有无问题不大//使能arr自动重装载，arpe=1,有缓冲器
		TIM_Cmd( TIM3,  ENABLE);//使能 TIM3
 
 
 
 
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
//	TIM_SetCompare2(TIM3,led_PB5_pwmval);//比较值
		

				{//暗->亮的循环
					TIM_SetCompare2(TIM3,TIM_GetCapture2(TIM3)+1);
			if(TIM_GetCapture2(TIM3)==300)
				TIM_SetCompare2(TIM3,0);
				}	
}

void  Cap_init(uint16_t arr,uint16_t psc)  //tim2输入捕获init
{
	
	
//	 u8  TIM2CH1_CAPTURE_STA;		//输入捕获状态		    				
// u16	TIM2CH1_CAPTURE_VAL;	//输入捕获值	
//	
	
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
		TIM_ICInitTypeDef TIM_ICInitStruct;
		GPIO_InitTypeDef GPIO_InitStruct;
		NVIC_InitTypeDef NVIC_InitStruc;
	
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//开启定时器2时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启Tim2_ch1的输入引脚PA15时钟
	
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD; // 高电平有效
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15; //im2_ch1的输入引脚PA15时钟
		GPIO_Init( GPIOA, &GPIO_InitStruct);
		GPIO_ResetBits(GPIOA,GPIO_Pin_15);//默认状态下拉
	
		TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//不分频率
		TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInitStruct.TIM_Period =arr;
		TIM_TimeBaseInitStruct.TIM_Prescaler =psc;
		TIM_TimeBaseInit( TIM2, &TIM_TimeBaseInitStruct);//定时器2init
	
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;//tim2_ch1-->pa15
	TIM_ICInitStruct.TIM_ICFilter = 0x0;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;//上升延
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;//不分频
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit( TIM2, & TIM_ICInitStruct);//输入捕获init
	
	//设置中断优先级分组
	 NVIC_InitStruc.NVIC_IRQChannel = TIM2_IRQn;
	 NVIC_InitStruc.NVIC_IRQChannelCmd =  ENABLE;
	 NVIC_InitStruc.NVIC_IRQChannelPreemptionPriority = 2;
	 NVIC_InitStruc.NVIC_IRQChannelSubPriority = 0;//先占优先级 2 级,//从优先级 0 级
	 NVIC_Init(& NVIC_InitStruc);

   TIM_ITConfig( TIM2,  TIM_IT_Update|TIM_IT_CC1, ENABLE);//使能 更新中断与 捕获中断
	
	TIM_Cmd(TIM2,ENABLE ); //使能定时器
}


void TIM2_IRQHandler(void)
{
//	extern u16 TIM2CH1_CAPTURE_TIME ;
//	extern u16	TIM2CH1_CAPTURE_VAL ;	//输入捕获值
//	extern u8 Flag_TIM2_CAPTURE_OK ; //成功捕获标志位
//	
//	struct bs{		//静态变量位域
//				unsigned char capture_ok:1;
//				unsigned char rising_flagbit:1;
//        unsigned char falling_flagbit: 1;
//        unsigned char UpdateOver_time: 5;
//    } static tim2IRQ_sta = { 0, 0, 0};
//		


//	if((tim2IRQ_sta.capture_ok)!= SET)//上一次中断，未成功捕获脉宽	
//	{
//				// 有更新中断
//				if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
//				{
//						if(tim2IRQ_sta.rising_flagbit==SET)//已经捕获到高电平了
//						{
//										if(tim2IRQ_sta.UpdateOver_time>=0X1F)//高电平太长了
//										{ 
//											tim2IRQ_sta.capture_ok = SET;//标记成功捕获了一次
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
//				// 有捕获中断 
//			if (TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET) 
//			{
//											
//						if(tim2IRQ_sta.rising_flagbit == SET)		//现在捕获到一个下降沿 ，之前捕获了上升沿，		
//						{	  			
//							tim2IRQ_sta.capture_ok = SET ;		//标记成功捕获到一次高电平脉宽
//							Flag_TIM2_CAPTURE_OK  = SET;
//								{
//								tim2IRQ_sta.capture_ok = 0;
//								tim2IRQ_sta.falling_flagbit = 0;
//								tim2IRQ_sta.rising_flagbit = 0;
//								
//								}
//							TIM2CH1_CAPTURE_VAL=TIM_GetCapture1(TIM2); //获得计数器值
//							TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
//						}
//						else //第一次捕获到上升沿						
//						{
//							
//							//清空tim2IRQ_sta，TIM5CH1_CAPTURE_VAL，标记rising_flagbit 
//							tim2IRQ_sta.capture_ok = 0;
//							tim2IRQ_sta.falling_flagbit = 0;
//							tim2IRQ_sta.UpdateOver_time = 0;
//							TIM2CH1_CAPTURE_VAL=0;
//							
//							TIM_SetCounter(TIM2,0);//清空计数器
//							tim2IRQ_sta.rising_flagbit = 1; //标记捕获到了上升沿
//							TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
//						}		    		
//						
//			}
	

if((TIM2CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{	  
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		{	    
			if(TIM2CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM2CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM2CH1_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM2CH1_CAPTURE_VAL=0XFFFF;
				}else TIM2CH1_CAPTURE_STA++;
			}	 
		}
		
		
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
		{	
			if(TIM2CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM2CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
				TIM2CH1_CAPTURE_VAL=TIM_GetCapture1(TIM2);
		   		TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}else  								//还未开始,第一次捕获上升沿
			{
				TIM2CH1_CAPTURE_STA=0;			//清空
				TIM2CH1_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM2,0);
				TIM2CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
		   		TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
			}		    
		}			     	    					   
 	}
 
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
	
	
	
		


	
	}

	
// TIM2CH1_CAPTURE_TIME = 	tim2IRQ_sta.UpdateOver_time ;
//TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update);//清除中断和捕获标志位

//}

