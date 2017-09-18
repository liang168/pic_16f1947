/*


*/

#include <pic.h>
#include "config_16f1947.h"
#include "adc_lib.h"
#include "pwm_lib.h"
#include "uart_lib.h"
#include "led_control.h"
#include "time1.h"

unsigned char status_flag ;	//�D�{���Ϊ��A�X��

void hardware_config(void)
{
	//GPIO �]�w
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

	ANSELE	= 0x00;		//���ϥ� ADC�\��
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

//���_�禡
//�o�̭n�ɶq�קK�Υ~���Ƶ{���n�Τ]���n�M��L�{���@�Ψ�
void interrupt ISR(void)
{
	if( RCIF )	//UART Ū�����_
	{
		RCIF = 0;
		unsigned char data = RC1REG;
		
		if(uart_start == 0)		//�T�w�S�����u�N�����
		{
			uart_rx_buff[uart_rx_count] = data;
			uart_rx_count ++;
			if(uart_rx_count >= 10)			//�����Q��Byte�~�����
			{
				unsigned char i = 0;

				uart_rx_count = 0;			//count �M���s
				uart_delay = 0;				//�����W��count �M�s

				//�����T�w��}��ƬO�_���T
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
						uart_start |= (1<<7);	//�����T�~�_��uart�B�z
					else
					{
						for(i = 0 ; i<10 ; i++)
						{	
							uart_rx_buff[i]=0;
						}		
					}
				}
				else	//��Ƥ���M��Buffer���s
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
	if( TMR1IF )	//TMR1 �p�ɤ��_
	{
		TMR1H	= TIME1_SET_DATA_H;
		TMR1L	= TIME1_SET_DATA_L1;
		TMR1IF = 0;

#if		START_TIME1_DELAY == 1
		if(status_flag == 0)
		{
			if(time1_delay < 60000 )		//�}��Delay�� �u�Ω�W�q��Ĥ@���ϥ�
			{
				time1_delay ++;
			}
			else
			{
			//�ϥήɪ`�N
			//�p�G�p�ɾ�����{���|�d��L�k���L�\��ʧ@
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
			fan_status = PORTE & 0x3f;		//�s�J�ثe�������A
		}
		//uart �������_���`�@����10�����
		//uart buff ����ƤS�S����10�����
		//�S�W�L�@�w�ɶ��S�����L��� �M��buff
		if(uart_rx_count != 0 && uart_start == 0)	//���count����ƦӥB�٨S��_�ʳB�z�ʧ@��
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
	//�Ȧs���]�w
	OSCCON = 0x70;			//32Mhz pll �����]�w�bconfig���]�w
	OSCTUNE = 0b00000000;

	hardware_config();		//�w��PIN�w�q

#if	START_TIME1_DELAY == 1
	//�u�@���A�X��
	//reset��delay �atime1�B�@
	//�p�Gtime1�S�}�Φ����D�{���|�d��L�k�u�@
	status_flag= 0;			
#else
	status_flag= 1;			//reset�᪽���B�@
#endif
	uart_init();			//UART���_
	pwm1_init();			//PWM1��X
	adc_hw_init();			//ADCŪ���]�w
	time1_init();			//�p�ɤ��_
	updates_interface();	//Ū�Xeeprom���

	GIE	= 1;				//�}���_�`�}��
	PEIE = 1;				//

	while(1)
	{
		switch(status_flag)	//�ھڪ��A�@�Ӯɶ��u���@���
		{
			case 0x01:			//���A�@ Ū����
			if(time1_out == 1)
			{
				time1_out = 0;
				read_fan_rpm();
			}
			break;
			case 0x02:			//���A�G Ūadc ���� led
			adc_change();
		//	status_flag = 3;
		//	break;
		//	case 0x03:
		//	updates_interface();
			led_control();
			status_flag = 4;
			break;				
			case 0x04:			//���A�| uart ��X����
			uart_tx_main();
			status_flag = 1;
			break;
		}
	}
}
