/*


*/

#define		_XTAL_FREQ 	32000000
//#define		_XTAL_FREQ 	16000000

#define  testbit(var, bit)  ((var) & (1 <<(bit)))
#define  setbit(var, bit)   ((var) |= (1 << (bit)))
#define  clrbit(var, bit)   ((var) &= ~(1 << (bit)))

#define		DC5VS_CURRENT		15
#define		DC9V_CURRENT		12
#define		DC5V_CURRENT		11
#define		ADC_PS1_D9			10
#define		ADC_PS1_D5			9
#define		ADC_PS2_D9			8
#define		ADC_PS2_D5			7

#define		FAN6_IN			6
#define		FAN5_IN			5
#define		FAN4_IN			4

#define		FAN3_IN			2
#define		FAN2_IN			1
#define		FAN1_IN			0

//全域變數
//這個值只用在debug
#define		ADC_DEBUG		0
#if		ADC_DEBUG	== 1
#define			ADC_NUM			13		//ADC存值總數
unsigned int 	adc_data[ADC_NUM];		//存放ADC讀取值
#endif

void adc_hw_init(void);
void adc_read(unsigned char channel);
void adc_change(void);

unsigned int check_fan_alarm(unsigned char read_fan);
unsigned int check_alarm_lo(unsigned int fan1_adc,unsigned int fan2_adc,unsigned int fan1_rpm,unsigned int fan2_rpm,unsigned char num);
