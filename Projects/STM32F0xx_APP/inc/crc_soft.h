#ifndef __CRC_SOFT_H
#define __CRC_SOFT_H

#include "stdint.h"

//CRC16_CCITT：多项式x16+x12+x5+1（0x1021），初始值0x0000，低位在前，高位在后，结果与0x0000异或
unsigned short CRC16_CCITT(unsigned char *puchMsg, unsigned int usDataLen);

//CRC16_CCITT_FALSE：多项式x16+x12+x5+1（0x1021），初始值0xFFFF，低位在后，高位在前，结果与0x0000异或
unsigned short CRC16_CCITT_FALSE(unsigned char *puchMsg, unsigned int usDataLen);

//CRC16_XMODEM：多项式x16+x12+x5+1（0x1021），初始值0x0000，低位在后，高位在前，结果与0x0000异或
unsigned short CRC16_XMODEM(unsigned char *puchMsg, unsigned int usDataLen);

//CRC16_X25：多项式x16+x12+x5+1（0x1021），初始值0x0000，低位在前，高位在后，结果与0xFFFF异或
unsigned short CRC16_X25(unsigned char *puchMsg, unsigned int usDataLen);

//CRC16_MODBUS：多项式x16+x15+x2+1（0x8005），初始值0xFFFF，低位在前，高位在后，结果与0x0000异或
unsigned short CRC16_MODBUS(unsigned char *puchMsg, unsigned int usDataLen);

//CRC16_IBM：多项式x16+x15+x2+1（0x8005），初始值0x0000，低位在前，高位在后，结果与0x0000异或
unsigned short CRC16_IBM(unsigned char *puchMsg, unsigned int usDataLen); 

//CRC16_MAXIM：多项式x16+x15+x2+1（0x8005），初始值0x0000，低位在前，高位在后，结果与0xFFFF异或
unsigned short CRC16_MAXIM(unsigned char *puchMsg, unsigned int usDataLen);

//CRC16_USB：多项式x16+x15+x2+1（0x8005），初始值0xFFFF，低位在前，高位在后，结果与0xFFFF异或
unsigned short CRC16_USB(unsigned char *puchMsg, unsigned int usDataLen);

#endif



