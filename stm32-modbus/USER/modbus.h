#ifndef __MODBUS_H__
#define __MODBUS_H__



unsigned int crc16(unsigned char* puchMsg,unsigned char usDataLen);
void Delay_my(unsigned short time);
void ParseRecieve8(void);
void ParseRecieve11(void);

#endif
