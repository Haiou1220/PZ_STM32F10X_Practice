#include  "led.h"
#include  "stm32f10x.h"//����ͷ�ļ�

/*�������������㿪����ܽŷ��䣺
D2	->	PC13	->LED��
S2	->	PA0	  ->����

I2C_SDA	->	PB6-----+
I2C_SCL	->	PB7-----+
										|
		E2���ݴ洢��-ADD=0

1���߼�Timer3��ͨ��Timer
*/

/*
@brief:��ʼ��led�ܽ�����,�������

*/


#define LED_PIN  		 GPIO_Pin_13
#define LED_PORT 		 GPIOC

void LedInitPin(void )
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/*����GPIOCʱ��*/
 RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC,  ENABLE);
	
	/*��ʼ���ܽ�*/
	/*
	D2	->	PC13	->LED��
	*/
	
	GPIO_InitStruct.GPIO_Pin = LED_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;

  GPIO_Init( LED_PORT, &GPIO_InitStruct);
	
	/*Ϩ��*/
	 LedOff( );
	
}

/*
@brief:LedOn(),����led
*/
void LedOn(void )
{
	GPIO_ResetBits( LED_PORT,  LED_PIN);

}


/*
@brief:LedOff(),Ϩ��led
*/
void LedOff(void )
{
	GPIO_SetBits( LED_PORT,  LED_PIN);

}

