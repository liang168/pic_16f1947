/*


*/

#include <pic.h>
#include "pwm_lib.h"
#include "adc_lib.h"

/*
//不使用
void _delay_ms(unsigned int ms)
{
	unsigned char i;  
	if (ms == 0) return;
	do 
	{
		i = 4;
		do 
		{
			_delay_us(164);
		} while(--i);
	} while(--ms);
}
*/
void pwm1_init(void)
{
//這是 32mhz 的設定

	// TMR2 SET
	T2CON	= SET_T2CON;
	PR2		= SET_PR2;
//	TMR2	= 0;	//清計數器
	TMR2ON	= 1;	//設定TMR2 ON BIT 起動
	//PWM1
	CCP1CON	= SET_CCP1CON;	

	pwm1_out_set(0x0a);	//50%
}

void pwm1_out_set(unsigned char pwm_out)
{
	CCPR1L	= pwm_out;
	__delay_ms(100);
}