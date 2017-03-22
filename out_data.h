/*

*/



struct 	InterfaceHW1{		//	item
	unsigned int			FAN12_Status;		//	0x00
	unsigned int			FAN34_Status;		//	0x01
	unsigned int			FAN56_Status;		//	0x02
	unsigned int			FAN1_I;				//	0x03
	unsigned int			FAN2_I;				//	0x04
	unsigned int			FAN3_I;				//	0x05
	unsigned int			FAN4_I;				//	0x06
	unsigned int			FAN5_I;				//	0x07
	unsigned int			FAN6_I;				//	0x08
	unsigned int			FAN1_Rate;			//	0x09
	unsigned int			FAN2_Rate;			//	0x0A
	unsigned int			FAN3_Rate;			//	0x0B
	unsigned int			FAN4_Rate;			//	0x0C
	unsigned int			FAN5_Rate;			//	0x0D
	unsigned int			FAN6_Rate;			//	0x0E
	unsigned int			PS1_D9V;			//	0x0F
	unsigned int			PS1_D5V;			//	0x10
	unsigned int			PS2_D9V;			//	0x11
	unsigned int			PS2_D5V;			//	0x12
	unsigned int			PS_I_9V;			//	0x13
	unsigned int			PS_I_5V;			//	0x14
	unsigned int			PS_I_5VS;			//	0x15
							
}	PSinterface1		;				

struct 	InterfaceHW2{							//	item
//	unsigned int		PS_3V3;					//	0x16
	unsigned int		Gain_Temperature;		//	0x17
	unsigned int		T_C_R;					//	0x18
	unsigned int		FANSetSpeed	;			//	0x19
 	unsigned int		FANSetMedSpeed;			//	0x1A
	unsigned int		FAN_I_HighAlarm;		//	0x1B
	unsigned int		FAN_I_LowAlarm;			//	0x1C
	unsigned char		FAN_I_InsideGap;		//	0x1DL
	unsigned char		FAN_I_OutsideGap;		//	0x1DH
	unsigned int		FAN_Rate_HighAlarm;		//	0x1E
	unsigned int		FAN_Rate_LowAlarm;		//	0x1F
	unsigned char		FAN_Rate_InsideGap;		//	0x20L
	unsigned char		FAN_Rate_OutsideGap;	//	0x20H
	unsigned int		FAN12_AlarmMask;		//	0x21
	unsigned int		FAN34_AlarmMask;		//	0x22
	unsigned int		FAN56_AlarmMask;		//	0x23
	unsigned int 		FAN_I_A;				//	0x24
	unsigned int		FAN_I_B;				//	0x25
	unsigned int		FAN_Rate_A;				//	0x26
	unsigned int		FAN_Rate_B;				//	0x27
	unsigned int		VrefPS;					//	0x28
}	PSinterface2	;				

