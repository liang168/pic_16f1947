/*


*/
#include <pic.h>
#include "adc_lib.h"
#include "out_data.h"

//�o�u���Ӱ��p�⵲�G��
union {			//�ϥε��c�p�X �ئa�O�t�X���P�B���
	unsigned char 	Ubyte[4];
	signed char		Wchar[4];
	unsigned int  	Uint[2];
	int  			wint[2];
	unsigned long 	UW4;
	long 			W4;
}CalValueIn;

//ADC �w��\��]�w
void adc_hw_init(void)
{
#if		ADC_DEBUG	== 1
	unsigned char i;

	for(i = 0 ; i< ADC_NUM ; i++)
		adc_data[i] = 0;	//�]�w�����ܼ�
#endif
	PSinterface1.PS_I_9V = 0;
	PSinterface1.PS_I_5V = 0;
	PSinterface1.PS_I_5VS = 0;

	PSinterface1.PS1_D9V = 0;
	PSinterface1.PS1_D5V = 0;
	PSinterface1.PS2_D9V = 0;
	PSinterface1.PS2_D5V = 0;

	PSinterface1.FAN1_I	= 0;
	PSinterface1.FAN2_I	= 0;
	PSinterface1.FAN3_I	= 0;
	PSinterface1.FAN4_I	= 0;
	PSinterface1.FAN5_I	= 0;
	PSinterface1.FAN6_I	= 0;

	PSinterface2.Gain_Temperature		= 300;	//�ūװ_�l�ȳ]�w

	//�]�wADC ��ƥk���� FOSC/64 VREF+���~���Ѧҹq��
	ADCON1 	= 0b11100010;
	//�]�w�nŪ�� ADC �q�D �M�_��BIT
//	ADCON0 	= 0x01;
	ADCON0	= 0x00;
	//	FVREN:		1 = Fixed Voltage Reference is enabled
	//	FVRRDY		0 = Fixed Voltage Reference output is not ready or not enabled
	//	TSEN:		1 = Temperature Indicator is enabled
	//	TSRNG:		1 = VOUT = VDD - 4VT (High Range)
	//	CDAFVR<1:0>:01 = Comparator and DAC and CPS Fixed Voltage Reference Peripheral output is 1x (1.024V)
	//	ADFVR<1:0>: 01 = ADC Fixed Voltage Reference Peripheral output is 1x (1.024V)
	FVRCON = 0b10110101;
}
//ADC Ū��
void adc_read(unsigned char channel)
{
	ADCON0 = (channel<<2)+1;	//�첾�ϥγq�D�[�_��bit
	__delay_us(200);
	ADGO = 1;
	while(ADGO) continue;
	CalValueIn.W4 = 0;
	CalValueIn.Ubyte[0] = ADRESL ;
	CalValueIn.Ubyte[1] = ADRESH ;
}
//ADC Ū����
//Ū����ഫ��s�JPSinterface1
void adc_change(void)
{
#if		ADC_DEBUG	== 1
	unsigned char i;

	for(i = 0 ; i< ADC_NUM ; i++)
		adc_data[i] = 0;
#endif

	//�q��Ū��
	//���q�N�@�wŪ�o�쪺�q��
	adc_read(DC5V_CURRENT);
#if		ADC_DEBUG	== 1
	adc_data[10] = CalValueIn.Uint[0];
#endif
	CalValueIn.UW4 = CalValueIn.UW4 * 941;				
	CalValueIn.UW4 = CalValueIn.UW4 / 64;
	CalValueIn.UW4 = CalValueIn.UW4 - 6323;
	if( CalValueIn.Uint[0] < 50000 )
		PSinterface1.PS_I_5V = CalValueIn.Uint[0];
	else
		PSinterface1.PS_I_5V = 0;	

//	if( PSinterface1.PS_I_5V > 50000 )
//		PSinterface1.PS_I_5V = 0;

	adc_read(DC9V_CURRENT);
#if		ADC_DEBUG	== 1
	adc_data[11] = CalValueIn.Uint[0];
#endif
	CalValueIn.UW4 = CalValueIn.UW4 * 2837;				
	CalValueIn.UW4 = CalValueIn.UW4 / 128;
	CalValueIn.UW4 = CalValueIn.UW4 - 16012;
	PSinterface1.PS_I_9V = CalValueIn.Uint[0];

	adc_read(DC5VS_CURRENT);
#if		ADC_DEBUG	== 1
	adc_data[12] = CalValueIn.Uint[0];
#endif
	CalValueIn.UW4 = CalValueIn.UW4 * 941;				
	CalValueIn.UW4 = CalValueIn.UW4 / 64;
	CalValueIn.UW4 = CalValueIn.UW4 - 6323;
	PSinterface1.PS_I_5VS = CalValueIn.Uint[0];
	//----------------------------

	//�ھڤ��P����Ū�쪺�q�����p
	adc_read(ADC_PS2_D5);
#if		ADC_DEBUG	== 1
	adc_data[ADC_PS2_D5-1] = CalValueIn.Uint[0];		//6
#endif
	CalValueIn.UW4 = CalValueIn.UW4 / 11;				
	PSinterface1.PS2_D5V = CalValueIn.Ubyte[0];

	adc_read(ADC_PS2_D9);	
#if		ADC_DEBUG	== 1
	adc_data[ADC_PS2_D9-1] = CalValueIn.Uint[0];		//7
#endif	
	CalValueIn.UW4 = CalValueIn.UW4 * 35;			
	CalValueIn.UW4 = CalValueIn.UW4 / 256;
	PSinterface1.PS2_D9V = CalValueIn.Ubyte[0];

	adc_read(ADC_PS1_D5);	
#if		ADC_DEBUG	== 1
	adc_data[ADC_PS1_D5-1] = CalValueIn.Uint[0];		//8
#endif
	CalValueIn.UW4 = CalValueIn.UW4 / 11;				
	PSinterface1.PS1_D5V = CalValueIn.Ubyte[0];

	adc_read(ADC_PS1_D9);
#if		ADC_DEBUG	== 1
	adc_data[ADC_PS1_D9-1] = CalValueIn.Uint[0];		//9
#endif	
	CalValueIn.UW4 = CalValueIn.UW4 * 35;			
	CalValueIn.UW4 = CalValueIn.UW4 / 256;
	PSinterface1.PS1_D9V = CalValueIn.Ubyte[0];
	//------------------------------

	//����Ū��
	adc_read(FAN6_IN);
#if		ADC_DEBUG	== 1
	adc_data[FAN6_IN-1] = CalValueIn.Uint[0];	//5
#endif
	CalValueIn.UW4 = CalValueIn.UW4 * 491;
	CalValueIn.UW4 = CalValueIn.UW4 / 1024;

	if(PSinterface1.FAN6_Rate == 0)
		PSinterface1.FAN6_I = 0;
	else
		PSinterface1.FAN6_I = CalValueIn.Uint[0];

	adc_read(FAN5_IN);
#if		ADC_DEBUG	== 1
	adc_data[FAN5_IN-1] = CalValueIn.Uint[0];	//4
#endif
	CalValueIn.UW4 = CalValueIn.UW4 * 491;
	CalValueIn.UW4 = CalValueIn.UW4 / 1024;

	if(PSinterface1.FAN5_Rate == 0)
		PSinterface1.FAN5_I = 0;
	else
		PSinterface1.FAN5_I = CalValueIn.Uint[0];

	adc_read(FAN4_IN);
#if		ADC_DEBUG	== 1
	adc_data[FAN4_IN-1] = CalValueIn.Uint[0];	//3
#endif
	CalValueIn.UW4 = CalValueIn.UW4 * 491;
	CalValueIn.UW4 = CalValueIn.UW4 / 1024;

	if(PSinterface1.FAN4_Rate == 0)
		PSinterface1.FAN4_I = 0;
	else
		PSinterface1.FAN4_I = CalValueIn.Uint[0];

	adc_read(FAN3_IN);
#if		ADC_DEBUG	== 1
	adc_data[FAN3_IN] = CalValueIn.Uint[0];
#endif
	CalValueIn.UW4 = CalValueIn.UW4 * 491;
	CalValueIn.UW4 = CalValueIn.UW4 / 1024;

	if(PSinterface1.FAN3_Rate == 0)
		PSinterface1.FAN3_I = 0;
	else
		PSinterface1.FAN3_I = CalValueIn.Uint[0];

	adc_read(FAN2_IN);
#if		ADC_DEBUG	== 1
	adc_data[FAN2_IN] = CalValueIn.Uint[0];
#endif
	CalValueIn.UW4 = CalValueIn.UW4 * 491;
	CalValueIn.UW4 = CalValueIn.UW4 / 1024;

	if(PSinterface1.FAN2_Rate == 0)
		PSinterface1.FAN2_I = 0;
	else
		PSinterface1.FAN2_I = CalValueIn.Uint[0];

	adc_read(FAN1_IN);
#if		ADC_DEBUG	== 1
	adc_data[FAN1_IN] = CalValueIn.Uint[0];
#endif
	CalValueIn.UW4 = CalValueIn.UW4 * 491;
	CalValueIn.UW4 = CalValueIn.UW4 / 1024;

	if(PSinterface1.FAN1_Rate == 0)
		PSinterface1.FAN1_I = 0;
	else
		PSinterface1.FAN1_I = CalValueIn.Uint[0];

	//���U�������A
	PSinterface1.FAN12_Status = check_fan_alarm(0);
	PSinterface1.FAN34_Status = check_fan_alarm(1);
	PSinterface1.FAN56_Status = check_fan_alarm(2);
}

//Ū�X�����q��
unsigned int check_fan_alarm(unsigned char read_fan)
{
	unsigned int out_data = 0;

	switch(read_fan)
	{
		case 0:
		out_data = check_alarm_lo(PSinterface1.FAN1_I,PSinterface1.FAN2_I,PSinterface1.FAN1_Rate,PSinterface1.FAN2_Rate,0);
		break;
		case 1:
		out_data = check_alarm_lo(PSinterface1.FAN3_I,PSinterface1.FAN4_I,PSinterface1.FAN3_Rate,PSinterface1.FAN4_Rate,1);
		break;
		case 2:
		out_data = check_alarm_lo(PSinterface1.FAN5_I,PSinterface1.FAN6_I,PSinterface1.FAN5_Rate,PSinterface1.FAN6_Rate,2);
		break;
	}
	return out_data;
}

//���O�_���b�d�򤺦��W�L�]�wĵ��
unsigned int check_alarm_lo(unsigned int fan1_adc,unsigned int fan2_adc,
			unsigned int fan1_rpm,unsigned int fan2_rpm,unsigned char num)
{
	unsigned int out_alarm = 0;

//------------------------------------
//�Ĥ@�խ����T�w
	if( testbit(out_alarm,1) == 1)
	{
		if( fan1_adc < (PSinterface2.FAN_I_HighAlarm - PSinterface2.FAN_I_InsideGap) )
			clrbit(out_alarm,1);
	}
	else
	{
		if( fan1_adc < (PSinterface2.FAN_I_HighAlarm + PSinterface2.FAN_I_OutsideGap) )		//�T�w�O�_�p��̰��q��
			clrbit(out_alarm,1);
		else
			setbit(out_alarm,1);
	}
	
	if( testbit(out_alarm,2) == 1)
	{
		if( fan1_adc > (PSinterface2.FAN_I_LowAlarm - PSinterface2.FAN_I_OutsideGap) )
			clrbit(out_alarm,2);
	}
	else
	{
		if( fan1_adc > (PSinterface2.FAN_I_LowAlarm + PSinterface2.FAN_I_InsideGap) )		//�T�w�O�_�j��̧C�q��
			clrbit(out_alarm,2);
		else
			setbit(out_alarm,2);
	}

	if( testbit(out_alarm,3) == 1)
	{
		if( fan1_rpm < (PSinterface2.FAN_Rate_HighAlarm - PSinterface2.FAN_Rate_InsideGap) )
			clrbit(out_alarm,3);
	}
	else
	{
		if( fan1_rpm < (PSinterface2.FAN_Rate_HighAlarm + PSinterface2.FAN_Rate_OutsideGap) )	//�T�w�O�_�p��̰���t
			clrbit(out_alarm,3);
		else
			setbit(out_alarm,3);
	}

	if( testbit(out_alarm,4) == 1)
	{
		if( fan1_rpm > (PSinterface2.FAN_Rate_LowAlarm + PSinterface2.FAN_Rate_OutsideGap) )
			clrbit(out_alarm,4);
	}
	else
	{
		if( fan1_rpm > (PSinterface2.FAN_Rate_LowAlarm - PSinterface2.FAN_Rate_InsideGap) )	//�T�w�O�_�j��̧C��t
			clrbit(out_alarm,4);
		else
			setbit(out_alarm,4);
	}
//------------------------------------
//�ĤG�խ����T�w
	if( testbit(out_alarm,9) == 1)
	{
		if( fan2_adc < (PSinterface2.FAN_I_HighAlarm - PSinterface2.FAN_I_InsideGap) )
			clrbit(out_alarm,9);
	}
	else
	{
		if( fan2_adc < (PSinterface2.FAN_I_HighAlarm + PSinterface2.FAN_I_OutsideGap) )
			clrbit(out_alarm,9);
		else
			setbit(out_alarm,9);
	}

	if( testbit(out_alarm,10) == 1)
	{
		if( fan2_adc > (PSinterface2.FAN_I_LowAlarm - PSinterface2.FAN_I_OutsideGap) )
			clrbit(out_alarm,10);
	}
	else
	{
		if( fan2_adc > (PSinterface2.FAN_I_LowAlarm + PSinterface2.FAN_I_InsideGap) )
			clrbit(out_alarm,10);
		else
			setbit(out_alarm,10);
	}

	if( testbit(out_alarm,11) == 1)
	{
		if( fan2_rpm < (PSinterface2.FAN_Rate_HighAlarm - PSinterface2.FAN_Rate_InsideGap) )
			clrbit(out_alarm,11);
	}
	else
	{
		if( fan2_rpm < (PSinterface2.FAN_Rate_HighAlarm + PSinterface2.FAN_Rate_OutsideGap) )
			clrbit(out_alarm,11);
		else
			setbit(out_alarm,11);
	}

	if( testbit(out_alarm,12) == 1)
	{
		if( fan2_rpm > (PSinterface2.FAN_Rate_LowAlarm + PSinterface2.FAN_Rate_OutsideGap) )
			clrbit(out_alarm,12);
	}
	else
	{
		if( fan2_rpm > (PSinterface2.FAN_Rate_LowAlarm - PSinterface2.FAN_Rate_InsideGap) )
			clrbit(out_alarm,12);
		else
			setbit(out_alarm,12);
	}
//------------------------------------
	if(num == 0)
	{
		//���ѽT�w
		if ( (PORTE&0x40) == 0 )
			clrbit(out_alarm,6);
		else
			setbit(out_alarm,6);

		if ( (PORTE&0x80) == 0 )
			clrbit(out_alarm,5);
		else
			setbit(out_alarm,5);
		//���W��������
		if( (PORTD & 0x40) == 0 )
			clrbit(out_alarm,0);
		else
			setbit(out_alarm,0);
	}
	else if(num == 1)
	{
		if( (PORTD & 0x80) == 0 )
			clrbit(out_alarm,8);
		else
			setbit(out_alarm,8);		
	}
//------------------------------------
	return out_alarm;
}
