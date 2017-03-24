/*


*/
//�ݭn�}��delay �]�w��1 ���ݭn�]0
//delay �{���b�p�ɤ��_���ϥ�
#define	START_TIME1_DELAY	0
//time1 �]�w�� 32mhz 0.1ms �]�w
#define	TIME1_SET_DATA_H	0xfc
#define	TIME1_SET_DATA_L0	0xe0
#define	TIME1_SET_DATA_L1	0xee
//time1 �]�w�� 16Mhz 0.1ms �]�w
//#define TIME1_SET_DATA_H	0xfe
//#define	TIME1_SET_DATA_L0	0x70
//#define	TIME1_SET_DATA_L1	0x7e	

#define	MAX_FAN_COUNT 	0xa8	//�W�Эȳ]�w

unsigned char time1_out;		//time1 ���_�ɶ����G�ѥ�
unsigned char fan_status;		//�C����쪺fan���A
unsigned char check_fan;		//Ū������fan����
unsigned char fan_flag;			//�ثeŪ�����q
unsigned char fan_count[9];		//Ū���쪺fan����

#if		START_TIME1_DELAY == 1
unsigned int time1_delay;
#endif

void time1_init(void);
unsigned int calculate_fan_rpm(unsigned int rpm_data);
void read_fan_rpm(void);

