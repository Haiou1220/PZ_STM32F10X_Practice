#include "delay.h"
#include "stm32f10x.h"
#include "stm32f10x_it.h"



extern __IO uint32_t DelayCountTimeNum;//中断stm32f10x_it.c 文件的systick 计数器

/*
@brief: 延时1us 时间间隔，模块初始化-启用systick功能
@prama: 
*/

void delay_init(void )
{

	uint32_t ticks  = SystemCoreClock/1000;
	/*
	 ticks = 72*10^6/1000 = 72*10^3
	 时钟频率72*10^6
	 减到零时间：（1/72*10^6）*72*10^3 = 1us
	*/
	
/*
	配置systick
	开启中断
*/
	if( SysTick_Config( ticks) != 0 )
	{
		 UsageFault_Handler();//配置systick失败
	}
	
}



/*
@brief: 微秒延时
@prama:time_us 
*/
void delay_us(u32 time_us )
{
	DelayCountTimeNum = time_us ;//systick中断函数中的DelayCountTimeNum操作减一
	while(DelayCountTimeNum!= 0x00)
	{	
		;//空，在等待
	}
	

}

/*
@brief: 毫秒延时
@prama:time_ms 
*/
void delay_ms(u32 time_ms )
{



}

