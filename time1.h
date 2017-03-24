/*


*/
//需要開機delay 設定為1 不需要設0
//delay 程式在計時中斷中使用
#define	START_TIME1_DELAY	0
//time1 設定值 32mhz 0.1ms 設定
#define	TIME1_SET_DATA_H	0xfc
#define	TIME1_SET_DATA_L0	0xe0
#define	TIME1_SET_DATA_L1	0xee
//time1 設定值 16Mhz 0.1ms 設定
//#define TIME1_SET_DATA_H	0xfe
//#define	TIME1_SET_DATA_L0	0x70
//#define	TIME1_SET_DATA_L1	0x7e	

#define	MAX_FAN_COUNT 	0xa8	//超標值設定

unsigned char time1_out;		//time1 中斷時間到辯識用
unsigned char fan_status;		//每次抓到的fan狀態
unsigned char check_fan;		//讀取那個fan指示
unsigned char fan_flag;			//目前讀取階段
unsigned char fan_count[9];		//讀取到的fan長度

#if		START_TIME1_DELAY == 1
unsigned int time1_delay;
#endif

void time1_init(void);
unsigned int calculate_fan_rpm(unsigned int rpm_data);
void read_fan_rpm(void);

