/*


*/

// Clock	8Mhz	9600bps set
//#define		UART_SET_DATA_L		0xcf
//#define		UART_SET_DATA_H		0x00
// Clock	32Mhz	9600bps set
//#define		UART_SET_DATA_L		0x40
//#define		UART_SET_DATA_H		0x03
// Clock	16Mhz	9600bps set
//#define			UART_SET_DATA_L		0xa0
//#define			UART_SET_DATA_H		0x01
// Clock	32Mhz	115200bps set
#define		UART_SET_DATA_L		0x44
#define		UART_SET_DATA_H		0x00

#define		NOT_TX_INT		0

unsigned char uart_rx_count;
unsigned char uart_start;
unsigned int uart_delay;
unsigned char uart_rx_buff[10];
#if		NOT_TX_INT == 1
#define		BUFF_MAX	22			//發送buf最大容量設定
#else
#define		BUFF_MAX	3

#define		TX_BUFF_MAX	54
unsigned char uart_tx_buff[TX_BUFF_MAX];
unsigned char uart_tx_count;
unsigned char uart_tx_start;
#endif
unsigned int uart_buffer[BUFF_MAX];

void uart_init(void);

void uart_tx_main(void);
#if		NOT_TX_INT == 1
void uart_send(unsigned char Byte);
void uart_SINT(unsigned int Uint);
void uart_sned_buffer(unsigned char count);
#else
void uart_get_tx_buff(unsigned char addr,unsigned int data);
void uart_tx_int_en(unsigned char count);
#endif
void write_interface(unsigned char address,unsigned char lo_data,unsigned char hi_data);
void updates_interface(void);
