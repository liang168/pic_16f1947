/*


*/

#include <pic.h>
#include "uart_lib.h"
#include "adc_lib.h"
#include "time1.h"
#include "eeprom_app.h"
#include "out_data.h"

void uart_init(void)
{
	unsigned char i;

	uart_rx_count	= 0;
	uart_start		= 0;	// UART 工作旗號
	uart_delay		= 0;
	for(i=0;i<BUFF_MAX;i++)
	{
		uart_buffer[i]=0;
	}
	for(i=0;i<10;i++)
	{
		uart_rx_buff[i]=0;
	}

	SP1BRGL	= UART_SET_DATA_L;		//
	SP1BRGH	= UART_SET_DATA_H;		//
	BAUD1CONbits.BRG16 = 1;			//使用16bit設定
	TX1STA	= 0b00100110;	// BRGH = 1
	RC1STA	= 0b10010000;	// 
	RCIF	= 0;			//
	RCIE	= 1;			// UART 中斷允許 單bit設定
}

void uart_tx_main(void)
{
	unsigned char i = 0;
	if( (uart_start & 0x80) == 0x80)
	{
		if(uart_rx_buff[0] != 0x10 && uart_rx_buff[1] != 0x02 &&
		uart_rx_buff[4] != 0x01 && uart_rx_buff[7] != 0x10 && 
		uart_rx_buff[8] != 0x03
		)
		{
			for(i = 0 ; i<10 ; i++)
			{	
				uart_rx_buff[i]=0;
			}
			uart_start = 0;
			return;
		}
		
		if(uart_rx_buff[2] == 0x01)
		{
			uart_start = uart_start + 1;
//			uart_buffer[0] = uart_rx_buff[3];
			uart_buffer[1] = uart_rx_buff[5];
			uart_buffer[2] = uart_rx_buff[6];
		}
		else if((uart_rx_buff[2] == 0x02) && (uart_rx_buff[3] == 0x01))
		{
			uart_start = uart_start + 2;
		}
		else if((uart_rx_buff[2] == 0x02) && (uart_rx_buff[3] == 0x02))
		{
			uart_start = uart_start + 3;
		}
		else if((uart_rx_buff[2] == 0x02) && (uart_rx_buff[3] == 0x03))
		{
			uart_start = uart_start + 4;
			uart_buffer[1] = uart_rx_buff[5];
			uart_buffer[2] = uart_rx_buff[6];
		}
		else if((uart_rx_buff[2] == 0x08) && (uart_rx_buff[3] == 0x73))
		{
			uart_start = uart_start + 8;
			uart_buffer[1] = uart_rx_buff[5];
			uart_buffer[2] = uart_rx_buff[6];
		}
		else
		{
			for(i = 0 ; i<10 ; i++)
			{	
				uart_rx_buff[i]=0;
			}
			uart_start = 0;
			return ;
		}

		if( (uart_start&0x03) == 0x01)
		{
			unsigned char address = uart_buffer[0];
			
			write_interface(address,uart_buffer[1],uart_buffer[2]);
			if(address >= 0x18)
			{
				//轉換成eeprom 位址
				address  = address - 0x18;
				address = address * 2;
			}
			write_int_eeprom(address,uart_buffer[1],uart_buffer[2]);
			uart_buffer[0] = 0;
			uart_buffer[1] = 0;
			uart_buffer[2] = 0;
			for(i = 0 ; i<10 ; i++)
			{	
				uart_rx_buff[i]=0;
			}
			uart_start = 0;
		}
		else if( (uart_start&0x03) == 0x02)
		{
			// 風扇工作狀態 1 2
			uart_buffer[0] = PSinterface1.FAN12_Status;
			// 風扇工作狀態 3 4
			uart_buffer[1] = PSinterface1.FAN34_Status;
			// 風扇工作狀態 5 6
			uart_buffer[2] = PSinterface1.FAN56_Status;
			// 2 ADC風扇讀取轉換後的值
			uart_buffer[3] = PSinterface1.FAN1_I;
			uart_buffer[4] = PSinterface1.FAN2_I;
			uart_buffer[5] = PSinterface1.FAN3_I;
			uart_buffer[6] = PSinterface1.FAN4_I;
			uart_buffer[7] = PSinterface1.FAN5_I;
			uart_buffer[8] = PSinterface1.FAN6_I;
			// 3 風扇RPM計算過的值
			uart_buffer[9] = PSinterface1.FAN1_Rate;
			uart_buffer[10] = PSinterface1.FAN2_Rate;
			uart_buffer[11] = PSinterface1.FAN3_Rate;
			uart_buffer[12] = PSinterface1.FAN4_Rate;
			uart_buffer[13] = PSinterface1.FAN5_Rate;
			uart_buffer[14] = PSinterface1.FAN6_Rate;
			// 4 ADC電壓轉換後的值 第一組
			uart_buffer[15] = PSinterface1.PS1_D9V;
			uart_buffer[16] = PSinterface1.PS1_D5V;
			uart_buffer[17] = PSinterface1.PS2_D9V;
			uart_buffer[18] = PSinterface1.PS2_D5V;
			// 5 ADC電壓轉換後的值 第二組
			uart_buffer[19] = PSinterface1.PS_I_9V;
			uart_buffer[20] = PSinterface1.PS_I_5V;
			uart_buffer[21] = PSinterface1.PS_I_5VS;

			uart_sned_buffer(22);
			for(i = 0 ; i<10 ; i++)
			{	
				uart_rx_buff[i]=0;
			}
			uart_start = 0;	
		}
		else if( (uart_start&0x03) == 0x03)
		{
			unsigned int temp = 0;
			uart_buffer[0] = 0x0000;
			uart_buffer[1] = 0x0000;

			uart_buffer[2] = PSinterface2.T_C_R;					//0x18
			uart_buffer[3] = PSinterface2.FANSetSpeed;				//0x19
			uart_buffer[4] = PSinterface2.FANSetMedSpeed;			//0x1a
			uart_buffer[5] = PSinterface2.FAN_I_HighAlarm;			//0x1b	fan_1 hi alarm
			uart_buffer[6] = PSinterface2.FAN_I_LowAlarm;			//0x1c	fan_l lo alarm
			temp = PSinterface2.FAN_I_InsideGap;					//0x1dl
			temp = temp + (PSinterface2.FAN_I_OutsideGap<<8);		//0x1dh
			uart_buffer[7] = temp ;
			uart_buffer[8] = PSinterface2.FAN_Rate_HighAlarm;		//0x1e	fan_rpm hi alarm
			uart_buffer[9] = PSinterface2.FAN_Rate_LowAlarm;		//0x1f	fan_rpm lo alarm
			temp = 0;
			temp = PSinterface2.FAN_Rate_InsideGap;					//0x20l
			temp = temp + (PSinterface2.FAN_Rate_OutsideGap<<8);	//0x20h
			uart_buffer[10] = temp;
			uart_buffer[11] = PSinterface2.FAN12_AlarmMask;			//0x21
			uart_buffer[12] = PSinterface2.FAN34_AlarmMask;			//0x22
			uart_buffer[13] = PSinterface2.FAN56_AlarmMask;			//0x23
			uart_buffer[14] = PSinterface2.FAN_I_A;					//0x24
			uart_buffer[15] = PSinterface2.FAN_I_B;					//0x25
			uart_buffer[16] = PSinterface2.FAN_Rate_A;				//0x26
			uart_buffer[17] = PSinterface2.FAN_Rate_B;				//0x27
			uart_buffer[18] = PSinterface2.VrefPS;					//0x28
			uart_sned_buffer(19);
			for(i = 0 ; i<10 ; i++)
			{	
				uart_rx_buff[i]=0;
			}
			uart_start = 0;
		}
		else if( (uart_start&0x04) == 0x04)
		{
			//更新溫度值 所以先清資料
			PSinterface2.Gain_Temperature = 0;
			PSinterface2.Gain_Temperature = uart_buffer[2] << 8;
			PSinterface2.Gain_Temperature = PSinterface2.Gain_Temperature + uart_buffer[1];
			
			uart_buffer[0] = 0;
			uart_buffer[1] = 0;
			uart_buffer[2] = 0;
			for(i = 0 ; i<10 ; i++)
			{	
				uart_rx_buff[i]=0;
			}
			uart_start = 0;
		}
		else if( (uart_start&0x08) == 0x08 )
		{
			write_int_eeprom(0,0x00,0x01);
			write_int_eeprom(2,0x00,0x0a);
			write_int_eeprom(4,0x00,0x0a);

			write_int_eeprom(6,0x02,0x58);
			write_int_eeprom(8,0x00,0x05);

			write_int_eeprom(12,0x16,0xda);
			write_int_eeprom(14,0x04,0x1a);	

			uart_buffer[0] = 0;
			uart_buffer[1] = 0;
			uart_buffer[2] = 0;
			for(i = 0 ; i<10 ; i++)
			{	
				uart_rx_buff[i]=0;
			}
			uart_start = 0;
		}
	}
}

// byte send
void uart_send(unsigned char Byte)
{
//	while(!TXIF);
	while( !(TX1STA&0x02) );
	TX1REG	= Byte;
}

// word send
void uart_SINT(unsigned int Uint)
{
	unsigned char temp = 0x00ff & Uint;
//	低資料先傳送	
//	while(!TXIF);
	while( !(TX1STA&0x02) );
	TX1REG = temp;
//	高資料後傳送
	temp = Uint >> 8;
//	while(!TXIF);
	while( !(TX1STA&0x02) );
	TX1REG = temp;
}
//send Big data
void uart_sned_buffer(unsigned char count)
{
	unsigned char i = 0;
	unsigned char crc = 0;

	for(i=0;i<count;i++)
	{
		unsigned char temp = 0x00ff &  uart_buffer[i];
	
		crc = crc + temp;
		temp = uart_buffer[i] >> 8;
		crc = crc + temp;
	}	
	uart_send(0x10);
	uart_send(0x02);
	uart_send(uart_rx_buff[2]);
	uart_send(uart_rx_buff[3]);
	uart_send( count*2 );
	for(i=0;i<count;i++)
	{
		uart_SINT(uart_buffer[i]);
	}
	uart_send(0x10);
	uart_send(0x03);
	uart_send(crc);	
	for(i=0;i<count;i++)
	{
		uart_buffer[i]=0;
	}
}

void write_interface(unsigned char address,unsigned char lo_data,unsigned char hi_data)
{
	unsigned int temp = 0x0000;

	temp = hi_data <<8;
	temp = temp + lo_data;

	switch(address)
	{
		case 0x18:
		PSinterface2.T_C_R = temp;
		break;
		case 0x19:
		PSinterface2.FANSetSpeed = temp;
		break;
		case 0x1a:
		PSinterface2.FANSetMedSpeed = temp;
		break;
		case 0x1b:
		PSinterface2.FAN_I_HighAlarm = temp;
		break;
		case 0x1c:
		PSinterface2.FAN_I_LowAlarm = temp;
		break;
		case 0x1d:
		PSinterface2.FAN_I_InsideGap = lo_data;
		PSinterface2.FAN_I_OutsideGap = hi_data;
		break;
		case 0x1e:
		PSinterface2.FAN_Rate_HighAlarm = temp;
		break;
		case 0x1f:
		PSinterface2.FAN_Rate_LowAlarm = temp;
		break;
		case 0x20:
		PSinterface2.FAN_Rate_InsideGap = lo_data;
		PSinterface2.FAN_Rate_OutsideGap = hi_data;
		break;
		case 0x21:
		PSinterface2.FAN12_AlarmMask = temp;
		break;
		case 0x22:
		PSinterface2.FAN34_AlarmMask = temp;
		break;
		case 0x23:
		PSinterface2.FAN56_AlarmMask = temp;
		break;
		case 0x24:
		PSinterface2.FAN_I_A = temp;
		break;
		case 0x25:
		PSinterface2.FAN_I_B = temp;
		break;
		case 0x26:
		PSinterface2.FAN_Rate_A = temp;
		break;
		case 0x27:
		PSinterface2.FAN_Rate_B = temp;
		break;
		case 0x28:
		PSinterface2.VrefPS = temp;
		break;
		default:
		temp = 0;
		break;
	}
}

void updates_interface(void)
{
	unsigned int temp = 0;

	PSinterface2.T_C_R = read_int_eeprom(0);				//0x18
	PSinterface2.FANSetSpeed = read_int_eeprom(2);			//0x19
	PSinterface2.FANSetMedSpeed = read_int_eeprom(4);		//0x1a
	PSinterface2.FAN_I_HighAlarm = read_int_eeprom(6);		//0x1b	fan_1 hi alarm
	PSinterface2.FAN_I_LowAlarm = read_int_eeprom(8);		//0x1c	fan_l lo alarm
	temp = read_int_eeprom(10);
	PSinterface2.FAN_I_InsideGap = temp & 0x00ff;			//0x1dl
	PSinterface2.FAN_I_OutsideGap =	temp >>8;				//0x1dh
	PSinterface2.FAN_Rate_HighAlarm = read_int_eeprom(12);	//0x1e	fan_rpm hi alarm
	PSinterface2.FAN_Rate_LowAlarm = read_int_eeprom(14);	//0x1f	fan_rpm lo alarm
	temp = read_int_eeprom(16);		
	PSinterface2.FAN_Rate_InsideGap = temp & 0x00ff;		//0x20l
	PSinterface2.FAN_Rate_OutsideGap =	temp >>8;			//0x20h
	PSinterface2.FAN12_AlarmMask = read_int_eeprom(18);		//0x21
	PSinterface2.FAN34_AlarmMask = read_int_eeprom(20);		//0x22
	PSinterface2.FAN56_AlarmMask = read_int_eeprom(22);		//0x23
	PSinterface2.FAN_I_A = read_int_eeprom(24);				//0x24
	PSinterface2.FAN_I_B = read_int_eeprom(26);				//0x25
	PSinterface2.FAN_Rate_A = read_int_eeprom(28);			//0x26
	PSinterface2.FAN_Rate_B = read_int_eeprom(30);			//0x27
	PSinterface2.VrefPS = read_int_eeprom(32);				//0x28	
}
