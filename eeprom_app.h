/*


*/

unsigned int read_int_eeprom(unsigned char address);
void write_int_eeprom(unsigned char address,unsigned char lo,unsigned char hi);
void write_low_eeprom(unsigned char write_add,unsigned char write_data);
unsigned char read_low_eeprom(unsigned char x1);

