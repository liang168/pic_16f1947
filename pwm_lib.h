/*


*/
/*
//不使用
#define FOSC 8000000L// Delay Function
//#define	FOSC	32000000L	//
#define	_delay_us(x) { unsigned char us; \
		us = (x)/(12000000/FOSC)|1; \
		while(--us != 0) continue;  }
*/		

//32Mhz 頻率25k pwm out設定
// 範圍 0x01~0x14 50% = 0x0a
#define		SET_T2CON		0x02	//設定預先除1:16 
#define		SET_PR2			0x13	//計數器結束值設定 PWM頻率設定
#define		SET_CCP1CON		0x0c	//設定PWM輸出功能單輸出

void pwm1_init(void);
void pwm1_out_set(unsigned char pwm_out);