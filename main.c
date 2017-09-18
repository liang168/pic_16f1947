/*


*/

#include <pic.h>
#include "config_16f1947.h"
#include "adc_lib.h"
#include "pwm_lib.h"
#include "uart_lib.h"
#include "led_control.h"
#include "time1.h"

unsigned char status_flag ;	//主程式用狀態旗號

void hardware_config(void)
{
	//GPIO 設定
	ANSELA	= 0x2f;		// for adc set reg
	TRISA	= 0x2f;
	LATA	= 0x00;
	PORTA	= 0x00;

	TRISB	= 0x00;		//led r
	LATB	= 0x00;
	PORTB	= 0x00;
	WPUB	= 0x00;

	LATC	= 0xf0;
	TRISC	= 0x80;		// pwm ps1 ps2 rx tx
	PORTC	= 0x00;

	TRISD	= 0xc0;		//led g
	LATD	= 0x00;
	PORTD	= 0x00;

	ANSELE	= 0x00;		//不使用 ADC功能
	TRISE	= 0xff;		//read fan clock
	LATE	= 0x00;
	PORTE	= 0x00;

	ANSELF	= 0xff;
	TRISF	= 0xff;
	LATF	= 0x00;
	PORTF	= 0x00;

	ANSELG	= 0x12;
	TRISG	= 0x12;
	LATG	= 0x00;
	PORTG	= 0x00;
}

//中斷函式
//這裡要盡量避免用外部副程式要用也不要和其他程式共用到
void interrupt ISR(void)
{
	if( RCIF )	//UART 讀取中斷
	{
		RCIF = 0;
		unsigned char data = RC1REG;
		
		if(uart_start == 0)		//確定沒有忙線就收資料
		{
			uart_rx_buff[uart_rx_count] = data;
			uart_rx_count ++;
			if(uart_rx_count >= 10)			//收滿十個Byte才比對資料
			{
				unsigned char i = 0;

				uart_rx_count = 0;			//count 清為零
				uart_delay = 0;				//接收超時count 清零

				//先比對固定位址資料是否正確
				if(uart_rx_buff[0] == 0x10 && uart_rx_buff[1] == 0x02 &&
				uart_rx_buff[4] == 0x01 && uart_rx_buff[7] == 0x10 &&
				uart_rx_buff[8] == 0x03 )
				{
					unsigned char crc = 0;

					for(i = 2; i<7 ; i++)
					{
						crc = crc + uart_rx_buff[i];
					}
					if(crc == uart_rx_buff[9])
						uart_start |= (1<<7);	//都正確才起動uart處理
					else
					{
						for(i = 0 ; i<10 ; i++)
						{	
							uart_rx_buff[i]=0;
						}		
					}
				}
				else	//資料不對清除Buffer為零
				{
					for(i = 0 ; i<10 ; i++)
					{	
						uart_rx_buff[i]=0;
					}				
				}		
			}
		}
	}
#if		NOT_TX_INT	== 0
	if( TXIF )
	{
		TXIF = 0;
		
		TX1REG = uart_tx_buff[uart_tx_start];
		uart_tx_start ++;
		if(uart_tx_start == uart_tx_count)
		{		
			uart_tx_start = 0;
			uart_tx_count = 0;
			TX1STAbits.TXEN = 0;
		}
	}
#endif
	if( TMR1IF )	//TMR1 計時中斷
	{
		TMR1H	= TIME1_SET_DATA_H;
		TMR1L	= TIME1_SET_DATA_L1;
		TMR1IF = 0;

#if		START_TIME1_DELAY == 1
		if(status_flag == 0)
		{
			if(time1_delay < 60000 )		//開機Delay用 只用於上電後第一次使用
			{
				time1_delay ++;
			}
			else
			{
			//使用時注意
			//如果計時器停止程式會卡住無法到其他功能動作
				time1_delay = 0;
				status_flag = 1;
			}
		}
		else if(status_flag == 1 && time1_out == 0)
#else
		if(status_flag == 1 && time1_out == 0)
#endif
		{
			time1_out = 1;
			fan_status = PORTE & 0x3f;		//存入目前風扇狀態
		}
		//uart 接收中斷正常一次收10筆資料
		//uart buff 有資料又沒有到10筆資料
		//又超過一定時間沒收到其他資料 清空buff
		if(uart_rx_count != 0 && uart_start == 0)	//比對count有資料而且還沒到起動處理動作時
		{
			uart_delay ++;
			if(uart_delay == 300)	//30ms
			{
				uart_delay =0;
				uart_rx_count = 0;
				unsigned char i;
				for(i = 0 ; i<10 ; i++)
				{	
					uart_rx_buff[i]=0;
				}		
			}
		}
	}
}

void main(void)
{
	//暫存器設定
	OSCCON = 0x70;			//32Mhz pll 相關設定在config中設定
	OSCTUNE = 0b00000000;

	hardware_config();		//硬體PIN定義

#if	START_TIME1_DELAY == 1
	//工作狀態旗號
	//reset後delay 靠time1運作
	//如果time1沒開或有問題程式會卡住無法工作
	status_flag= 0;			
#else
	status_flag= 1;			//reset後直接運作
#endif
	uart_init();			//UART中斷
	pwm1_init();			//PWM1輸出
	adc_hw_init();			//ADC讀取設定
	time1_init();			//計時中斷
	updates_interface();	//讀出eeprom資料

	GIE	= 1;				//開中斷總開關
	PEIE = 1;				//

	while(1)
	{
		switch(status_flag)	//根據狀態一個時間只做一件事
		{
			case 0x01:			//狀態一 讀風扇
			if(time1_out == 1)
			{
				time1_out = 0;
				read_fan_rpm();
			}
			break;
			case 0x02:			//狀態二 讀adc 控制 led
			adc_change();
		//	status_flag = 3;
		//	break;
		//	case 0x03:
		//	updates_interface();
			led_control();
			status_flag = 4;
			break;				
			case 0x04:			//狀態四 uart 輸出控制
			uart_tx_main();
			status_flag = 1;
			break;
		}
	}
}
