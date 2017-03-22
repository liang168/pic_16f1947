/*


*/

// Clock	8Mhz	9600bps set
//#define		UART_SET_DATA_L		0xcf
//#define		UART_SET_DATA_H		0x00
// Clock	32Mhz	9600bps set
#define		UART_SET_DATA_L		0x40
#define		UART_SET_DATA_H		0x03
// Clock	16Mhz	9600bps set
//#define			UART_SET_DATA_L		0xa0
//#define			UART_SET_DATA_H		0x01
#define		BUFF_MAX	22			//發送buf最大容量設定

unsigned char uart_rx_count;
unsigned char uart_start;
unsigned int uart_delay;
unsigned int uart_buffer[BUFF_MAX];
unsigned char uart_rx_buff[10];

void uart_init(void);
void uart_send(unsigned char Byte);
void uart_SINT(unsigned int Uint);
void uart_tx_main(void);
void uart_sned_buffer(unsigned char count);

void write_interface(unsigned char address,unsigned char lo_data,unsigned char hi_data);
void updates_interface(void);
