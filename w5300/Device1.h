#define true	0xff
#define false	0x00

typedef unsigned char	SOCKET;

#define BUFFER_SIZE	2048

#define S_RX_SIZE	8192
#define S_TX_SIZE	8192

extern unsigned char xdata Data_Buffer[];

extern unsigned char xdata Parameters[];
extern void Delay(unsigned int d);

/* SOCKET0 */
extern bit S0_SendOK;
extern bit S0_Receive;
extern bit S0_Timeout;
extern bit S0_Connect;
extern bit S0_Init;

/* SOCKET1 */
extern bit S1_SendOK;
extern bit S1_Receive;
extern bit S1_Timeout;
extern bit S1_Connect;
extern bit S1_Init;

/* SOCKET2 */
extern bit S2_SendOK;
extern bit S2_Receive;
extern bit S2_Timeout;
extern bit S2_Connect;
extern bit S2_Init;

/* SOCKET3 */
extern bit S3_SendOK;
extern bit S3_Receive;
extern bit S3_Timeout;
extern bit S3_Connect;
extern bit S3_Init;

/* SOCKET4 */
extern bit S4_SendOK;
extern bit S4_Receive;
extern bit S4_Timeout;
extern bit S4_Connect;
extern bit S4_Init;

/* SOCKET5 */
extern bit S5_SendOK;
extern bit S5_Receive;
extern bit S5_Timeout;
extern bit S5_Connect;
extern bit S5_Init;

/* SOCKET6 */
extern bit S6_SendOK;
extern bit S6_Receive;
extern bit S6_Timeout;
extern bit S6_Connect;
extern bit S6_Init;

/* SOCKET7 */
extern bit S7_SendOK;
extern bit S7_Receive;
extern bit S7_Timeout;
extern bit S7_Connect;
extern bit S7_Init;
