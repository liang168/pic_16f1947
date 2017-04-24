/*


*/

#include <pic.h>
#include "eeprom_app.h"

//從左到右
__EEPROM_DATA( 0x00, 0x01, 0x00, 0x0a, 0x00, 0x0a, 0x02, 0x58 );	// 00 01 02 03 04 05 06 07
__EEPROM_DATA( 0x00, 0x05, 0x04, 0x04, 0x1e, 0x78, 0x04, 0x1a );	// 08 09 10 11 12 13 14 15
__EEPROM_DATA( 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 );	// 16 17 18 19 20 21 22 23
__EEPROM_DATA( 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 );	// 24 25 26 27 28 29 30 31
__EEPROM_DATA( 0x30, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 );	// 32 33

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
