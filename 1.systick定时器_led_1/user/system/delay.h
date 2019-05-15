#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"


/*
@brief: 延时1us 时间间隔，模块初始化-启用systick功能
@prama: 
*/
void delay_init(void );
	

/*
@brief: 微秒延时
@prama:time_us 
*/
void delay_us(u32 time_us );



#endif



