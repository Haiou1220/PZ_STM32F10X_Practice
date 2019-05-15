#include  "led.h"
#include  "stm32f10x.h"//顶层头文件

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
@brief:初始化led管脚配置,推挽输出

*/


#define LED_PIN  		 GPIO_Pin_13
#define LED_PORT 		 GPIOC

void LedInitPin(void )
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/*开启GPIOC时钟*/
 RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC,  ENABLE);
	
	/*初始化管脚*/
	/*
	D2	->	PC13	->LED灯
	*/
	
	GPIO_InitStruct.GPIO_Pin = LED_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;

  GPIO_Init( LED_PORT, &GPIO_InitStruct);
	
	/*熄灭*/
	 LedOff( );
	
}

/*
@brief:LedOn(),点亮led
*/
void LedOn(void )
{
	GPIO_ResetBits( LED_PORT,  LED_PIN);

}


/*
@brief:LedOff(),熄灭led
*/
void LedOff(void )
{
	GPIO_SetBits( LED_PORT,  LED_PIN);

}

