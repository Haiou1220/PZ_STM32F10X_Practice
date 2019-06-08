#include "stm32f10x.h"
#include "delay.h"



uint16_t arr = 899;  
uint16_t psc = 0; //不分频,PWM 频率=72000/900=80Khz
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
	 
	/*led的 IO端口配置*/	 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	    //使能PC13,--D2	->	PC13	->LED灯
	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;			    //LED
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);			     //初始化GPIO
  GPIO_SetBits(GPIOC,GPIO_Pin_13);					// 输出高
	 
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
		TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
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
		
	  delay_init();//tick 定时器延时init
		//TIM_SetCompare2(TIM3,0);//比较值
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
		TIM_SetCompare2(TIM3,led_PB5_pwmval);//比较值
		//delay_ms(10);
	}
	
 }
