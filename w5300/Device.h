#define true	0xff
#define false	0x00

#define CR		0x0d
#define LF		0x0a

typedef unsigned char	SOCKET;

#define BUFFER_SIZE	2048

#define S_RX_SIZE	8192
#define S_TX_SIZE	8192

sbit RST_W5300=		P0^0;
sbit D_OUTPUT1=		P0^1;
sbit D_OUTPUT2=		P0^2;

sbit D_INPUT1=		P1^0;
sbit D_INPUT2=		P1^1;
sbit L_LINK=		P1^3;

unsigned char xdata Data_Buffer[BUFFER_SIZE];
unsigned int UART_Size, UART_idx;

unsigned int timer0_cnt;

unsigned char xdata Parameters[90];

unsigned char xdata UART_RX_Data[32];		/* Received Data from UART */
unsigned char UART_RX_idx;
bit UART_RX;

unsigned char xdata UART_TX_Data[32];
unsigned char UART_TX_idx;
bit UART_TXOK;
bit UART_CMD;

unsigned char idata temp[3];
unsigned char idata temporary[4];

void Erase_Page(void);
void Write_FALSH_Data(void);

/* SOCKET0 */
bit S0_SendOK;
bit S0_Receive;
bit S0_Timeout;
bit S0_Connect;
bit S0_Init;

/* SOCKET1 */
bit S1_SendOK;
bit S1_Receive;
bit S1_Timeout;
bit S1_Connect;
bit S1_Init;

/* SOCKET2 */
bit S2_SendOK;
bit S2_Receive;
bit S2_Timeout;
bit S2_Connect;
bit S2_Init;

/* SOCKET3 */
bit S3_SendOK;
bit S3_Receive;
bit S3_Timeout;
bit S3_Connect;
bit S3_Init;

/* SOCKET4 */
bit S4_SendOK;
bit S4_Receive;
bit S4_Timeout;
bit S4_Connect;
bit S4_Init;

/* SOCKET5 */
bit S5_SendOK;
bit S5_Receive;
bit S5_Timeout;
bit S5_Connect;
bit S5_Init;

/* SOCKET6 */
bit S6_SendOK;
bit S6_Receive;
bit S6_Timeout;
bit S6_Connect;
bit S6_Init;

/* SOCKET7 */
bit S7_SendOK;
bit S7_Receive;
bit S7_Timeout;
bit S7_Connect;
bit S7_Init;

extern void W5300_Interrupt_Process(void);
extern void W5300_Init(void);
extern unsigned char Detect_Gateway(void);
extern unsigned char SOCKET_Listen(SOCKET s);
extern unsigned char SOCKET_Connect(SOCKET s);
extern unsigned char SOCKET_UDP(SOCKET s);
extern unsigned int Rx_Process(SOCKET s);
extern unsigned char Tx_Process(SOCKET s, unsigned int size);
extern void Close_Socket(SOCKET s);

extern void Hex_TO_Dec(unsigned int x);
extern unsigned int Dec_TO_Hex(unsigned char k,unsigned int x);
