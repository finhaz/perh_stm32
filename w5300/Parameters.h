#define FLASH_MEM_BASE	0xf000

typedef struct 
{
	unsigned char GATEWAY[4];
	unsigned char SUBNET[4];
	unsigned char MAC_ADDR[6];
	unsigned char IP[4];

	unsigned char Sn_SPORT[8][2];
	unsigned char Sn_DIP[8][4];
	unsigned char Sn_DPORT[8][2];
	unsigned char Sn_MODE[8];
}Para;

#define TCP_SERVER	0
#define TCP_CLIENT	1
#define UDP_MODE	2
