#include "delay.h"
#include "stm32f10x.h"
#include "stm32f10x_it.h"



extern __IO uint32_t DelayCountTimeNum;//�ж�stm32f10x_it.c �ļ���systick ������

/*
@brief: ��ʱ1us ʱ������ģ���ʼ��-����systick����
@prama: 
*/

void delay_init(void )
{

	uint32_t ticks  = SystemCoreClock/1000;
	/*
	 ticks = 72*10^6/1000 = 72*10^3
	 ʱ��Ƶ��72*10^6
	 ������ʱ�䣺��1/72*10^6��*72*10^3 = 1us
	*/
	
/*
	����systick
	�����ж�
*/
	if( SysTick_Config( ticks) != 0 )
	{
		 UsageFault_Handler();//����systickʧ��
	}
	
}



/*
@brief: ΢����ʱ
@prama:time_us 
*/
void delay_us(u32 time_us )
{
	DelayCountTimeNum = time_us ;//systick�жϺ����е�DelayCountTimeNum������һ
	while(DelayCountTimeNum!= 0x00)
	{	
		;//�գ��ڵȴ�
	}
	

}

/*
@brief: ������ʱ
@prama:time_ms 
*/
void delay_ms(u32 time_ms )
{



}

