#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"


/*
@brief: ��ʱ1us ʱ������ģ���ʼ��-����systick����
@prama: 
*/
void delay_init(void );
	

/*
@brief: ΢����ʱ
@prama:time_us 
*/
void delay_us(u32 time_us );



#endif



