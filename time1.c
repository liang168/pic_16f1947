/*


*/

#include <pic.h>
#include "time1.h"
#include "out_data.h"

extern unsigned char status_flag ;	//使用外部宣告記憶體

void time1_init(void)
{
	unsigned char i ;

	T1CON	= 0b00000000;
	TMR1H	= TIME1_SET_DATA_H;
	TMR1L	= TIME1_SET_DATA_L0;

	TMR1IF	= 0;
	TMR1IE	= 1;

	time1_out	= 0;
	fan_status	= 0;		//存計時器存入的風扇狀態
	check_fan	= 0x01;		//設定起始點 FAN1
	fan_flag	= 0;		//風扇工作點
	PSinterface1.FAN1_Rate	= 0;
	PSinterface1.FAN2_Rate	= 0;
	PSinterface1.FAN3_Rate	= 0;
	PSinterface1.FAN4_Rate	= 0;
	PSinterface1.FAN5_Rate	= 0;
	PSinterface1.FAN6_Rate	= 0;
	for(i = 0 ; i < 8 ; i++)
	{
		fan_count[i] = 0;
	}

	TMR1ON	= 1;		//打開計時器1功能
}

//rpm count值轉換
unsigned int calculate_fan_rpm(unsigned int rpm_data)
{
	unsigned int temp = 0;
	unsigned long cal_data = 0;

	//這是公式
	temp = rpm_data/7;		//做七八次平均計算
	cal_data = temp*4;		//
	cal_data = 600000/cal_data;

	return (int)cal_data;
}

void read_fan_rpm(void)
{
	switch(fan_flag)
	{
		case 0:
		fan_count[0]++;
		if(fan_count[0] > MAX_FAN_COUNT)	//讀取時間超標
			fan_flag = 8;					//下一階結束
		else if( (fan_status & check_fan) != 0)	//讀到為HI
			fan_flag = 1;					//到下一階計數
		break;
		case 1:
		fan_count[1]++;
		if(fan_count[1] > MAX_FAN_COUNT)	//讀取時間超標
			fan_flag = 8;		
		else if( (fan_status & check_fan) == 0)	//讀到為LO
			fan_flag = 2;
		break;
		case 2:
		fan_count[2] ++;
		if(fan_count[2] > MAX_FAN_COUNT)	
			fan_flag = 8;
		else if( (fan_status & check_fan) != 0)	//讀到為HI
			fan_flag = 3;
		break;
		case 3:
		fan_count[3] ++;
		if(fan_count[3] > MAX_FAN_COUNT)	
			fan_flag = 8;
		else if( (fan_status & check_fan) == 0)	//讀到為LO
			fan_flag = 4;
		break;
		case 4:
		fan_count[4] ++;
		if(fan_count[4] > MAX_FAN_COUNT)
			fan_flag = 8;
		else if( (fan_status & check_fan) != 0)	//讀到為HI
			fan_flag = 5;
		break;
		case 5:
		fan_count[5] ++;
		if(fan_count[5] > MAX_FAN_COUNT)	
			fan_flag = 8;
		else if( (fan_status & check_fan) == 0)	//讀到為LO
			fan_flag = 6;
		break;
		case 6:
		fan_count[6] ++;
		if(fan_count[6] > MAX_FAN_COUNT)	
			fan_flag = 8;
		else if( (fan_status & check_fan) != 0)	//讀到為HI
			fan_flag = 7;
		break;
		case 7:
		fan_count[7] ++;
		if(fan_count[7] > MAX_FAN_COUNT)	
			fan_flag = 8;	
		else if( (fan_status & check_fan) == 0)	//讀到為LO
			fan_flag = 8;
		break;
		case 8:		//結束一個fan處理
		{
			unsigned char i ;
			unsigned char effective = 0;	//有效值
			unsigned int out_data =0 ;
			for(i=0 ; i<8 ; i++)	//所有count加總
			{
				if(fan_count[i] != 0)
				{
					effective ++;	//不為零有效次數加一
					out_data = out_data + fan_count[i];
				}
			}
			fan_flag = 0;			//回歸0階計數
			switch(check_fan)
			{
				case 0x01:
					check_fan = check_fan<<1;
					if(effective == 8)
						PSinterface1.FAN1_Rate = calculate_fan_rpm(out_data);
					else
						PSinterface1.FAN1_Rate = 0;
				break;
				case 0x02:
					check_fan = check_fan<<1;
					if(effective == 8)
						PSinterface1.FAN2_Rate = calculate_fan_rpm(out_data);
					else
						PSinterface1.FAN2_Rate = 0;
				break;
				case 0x04:
					check_fan = check_fan<<1;
					if(effective == 8)
						PSinterface1.FAN3_Rate = calculate_fan_rpm(out_data);
					else
						PSinterface1.FAN3_Rate = 0;
				break;
				case 0x08:
					check_fan = check_fan<<1;
					if(effective == 8)
						PSinterface1.FAN4_Rate = calculate_fan_rpm(out_data);
					else
						PSinterface1.FAN4_Rate = 0;
				break;
				case 0x10:
					check_fan = check_fan<<1;
					if(effective == 8)
						PSinterface1.FAN5_Rate = calculate_fan_rpm(out_data);
					else
						PSinterface1.FAN5_Rate = 0;
				break;
				case 0x20:
					check_fan = check_fan<<1;
					if(effective == 8)
						PSinterface1.FAN6_Rate = calculate_fan_rpm(out_data);
					else
						PSinterface1.FAN6_Rate = 0;
				break;
				case 0x40:
					check_fan = 0x01;
					status_flag = 2;
				break;
			}
			for(i=0; i<9 ; i++)
			{
				fan_count[i] = 0;
			}
		}
		break;
	}
}

