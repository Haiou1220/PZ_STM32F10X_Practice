#ifndef __LED_H
#define __LED_H

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
@brief:LedInitPin(),��ʼ��led�ܽ�����,�������
*/
void LedInitPin(void );

/*
@brief:LedOn(),����led
*/
void LedOn(void );

/*
@brief:LedOff(),Ϩ��led
*/
void LedOff(void );



#endif /* __LED_H */

