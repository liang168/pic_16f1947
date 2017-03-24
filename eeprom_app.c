/*


*/

#include <pic.h>
#include "eeprom_app.h"

//從左到右
__EEPROM_DATA( 0x00, 0x01, 0x00, 0x0a, 0x00, 0x0a, 0x02, 0x58 );	// 0  1  2  3  4  5  6  7
__EEPROM_DATA( 0x00, 0x05, 0x0a, 0x0b, 0x16, 0xda, 0x04, 0x1a );	// 8  9  10 11 12 13 14 15
__EEPROM_DATA( 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 );	// 
__EEPROM_DATA( 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f );	// 
__EEPROM_DATA( 0x30, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 );	// 

unsigned int read_int_eeprom(unsigned char address)
{
	//讀寫使用 先hi 再lo
	unsigned int temp = 0x0000;

	temp = read_low_eeprom(address) << 8;		//前byte
	temp = temp + read_low_eeprom(address+1);	//後byte int 資料分前後

	return temp;
}

void write_int_eeprom(unsigned char address,unsigned char lo,unsigned char hi)
{
	write_low_eeprom(address,hi);
	write_low_eeprom(address+1,lo);
}

void write_low_eeprom(unsigned char write_add,unsigned char write_data)
{
//	while(WR == 1);

	EEADR	= write_add;
	EEDATA	= write_data;

	GIE		= 0;
	PEIE	= 0;

	EEPGD	= 0;
	WREN	= 1;

	EECON2	= 0x55;
	EECON2	= 0xaa;
	WR		= 1;
	while(WR == 1);
	GIE		= 1;
	PEIE	= 1;

	WREN	= 0;
}

unsigned char read_low_eeprom(unsigned char x1)
{
//	while(RD == 1);
	EEADR	= x1;

	GIE		= 0;
	PEIE	= 0;

	EEPGD	= 0;
	RD		= 1;
	while(RD == 1);
	GIE		= 1;
	PEIE	= 1;

	return EEDAT;
}
