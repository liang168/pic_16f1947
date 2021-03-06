/*


*/
#include <pic.h>
#include "led_control.h"
#include "adc_lib.h"
#include "eeprom_app.h"
#include "pwm_lib.h"
#include "out_data.h"

void led_control(void)
{
	unsigned int temp = PSinterface2.T_C_R;		//addr 0x18

	switch( temp )
	{
		case 0:		//手動控制
		{
			//0x19
			pwm1_out_set(PSinterface2.FANSetSpeed);
		}
		break;
		case 1:		//根據溫度自動控制
		if(PSinterface2.Gain_Temperature < 250)
		{
			pwm1_out_set(0x04);
		}
		else if( (PSinterface2.Gain_Temperature > 450) && (PSinterface2.Gain_Temperature < 500) )
		{
			pwm1_out_set(0x0a);
		}
		else if(PSinterface2.Gain_Temperature > 500 )
		{
			pwm1_out_set(0x14);
		}
		break;
		case 2:		//強制低轉速
		pwm1_out_set(0x04);		//0x03 低於1000轉 會偵測不到轉速
		break;
		case 3:		//強制中轉速
		{
			//0x1a
			//pwm1_out_set(PSinterface2.FANSetMedSpeed);
			pwm1_out_set(0x0a);
		}
		break;
		case 4:		//強制高轉速
		pwm1_out_set(0x14);
		break;
		default:
		break;
	}


	if ( (PORTD & 0b01000000) == 0 )	//風扇有接上
	{
		//亮綠燈
		if( (PSinterface1.FAN12_Status & 0x001e) != 0 )
		{
			LATB |= (1<<0);
			LATD &= ~(1<<0);
		}
		else
		{
			LATB &= ~(1<<0);
			LATD |= (1<<0);		//hi動作
		}
		if( (PSinterface1.FAN12_Status & 0x1e00) != 0 )
		{
			LATB |= (1<<1);
			LATD &= ~(1<<1);
		}
		else
		{
			LATB &= ~(1<<1);
			LATD |= (1<<1);
		}
		if( (PSinterface1.FAN34_Status & 0x001e) != 0 )
		{
			LATB |= (1<<2);
			LATD &= ~(1<<2);
		}
		else
		{
			LATB &= ~(1<<2);
			LATD |= (1<<2);	
		}
	}
	else	//風扇沒接上
	{
		//亮紅燈
		LATB |= (1<<0);
		LATB |= (1<<1);
		LATB |= (1<<2);
		LATD &= ~(1<<0);
		LATD &= ~(1<<1);
		LATD &= ~(1<<2);
	}
	
	if ( (PORTD & 0b10000000) == 0 )
	{
		if( (PSinterface1.FAN34_Status & 0x1e00) != 0 )
		{
			LATD &= ~(1<<3);
			LATB |= (1<<3);
		}
		else
		{
			LATB &= ~(1<<3);
			LATD |= (1<<3);		//hi動作
		}
		if( (PSinterface1.FAN56_Status & 0x001e) != 0 )
		{
			LATD &= ~(1<<4);
			LATB |= (1<<4);
		}
		else
		{
			LATB &= ~(1<<4);
			LATD |= (1<<4);
		}
		if( (PSinterface1.FAN56_Status & 0x1e00) != 0 )
		{
			LATD &= ~(1<<5);
			LATB |= (1<<5);
		}
		else
		{
			LATB &= ~(1<<5);
			LATD |= (1<<5);	
		}
	}
	else
	{
		LATD &= ~(1<<3);
		LATD &= ~(1<<4);
		LATD &= ~(1<<5);
		LATB |= (1<<3);
		LATB |= (1<<4);
		LATB |= (1<<5);
	}

	if ( (PORTE & 0x40) == 0 )
		LATC |= (1<<0);		//PS1_IN
	else
		LATC &= ~(1<<0);	//PS1_IN

	if ( (PORTE & 0x80) == 0 )
		LATC |= (1<<1);		//PS2_IN
	else
		LATC &= ~(1<<1);	//PS2_IN
}
