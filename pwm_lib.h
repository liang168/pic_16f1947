/*


*/
/*
//���ϥ�
#define FOSC 8000000L// Delay Function
//#define	FOSC	32000000L	//
#define	_delay_us(x) { unsigned char us; \
		us = (x)/(12000000/FOSC)|1; \
		while(--us != 0) continue;  }
*/		

//32Mhz �W�v25k pwm out�]�w
// �d�� 0x01~0x14 50% = 0x0a
#define		SET_T2CON		0x02	//�]�w�w����1:16 
#define		SET_PR2			0x13	//�p�ƾ������ȳ]�w PWM�W�v�]�w
#define		SET_CCP1CON		0x0c	//�]�wPWM��X�\����X

void pwm1_init(void);
void pwm1_out_set(unsigned char pwm_out);