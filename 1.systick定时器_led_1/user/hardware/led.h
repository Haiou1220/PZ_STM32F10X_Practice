#ifndef __LED_H
#define __LED_H

/*普中下载器迷你开发版管脚分配：
D2	->	PC13	->LED灯
S2	->	PA0	  ->按键

I2C_SDA	->	PB6-----+
I2C_SCL	->	PB7-----+
										|
		E2数据存储器-ADD=0

1个高级Timer3个通用Timer
*/

/*
@brief:LedInitPin(),初始化led管脚配置,推挽输出
*/
void LedInitPin(void );

/*
@brief:LedOn(),点亮led
*/
void LedOn(void );

/*
@brief:LedOff(),熄灭led
*/
void LedOff(void );



#endif /* __LED_H */

