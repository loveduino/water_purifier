#ifndef __I2C_SOFT_H
#define __I2C_SOFT_H

#include <stdbool.h>

//i2c软件模拟
#include "stm32f0xx.h"

#define NO_ERROR            0x00
#define ACK_ERROR           0x01
#define CHECKSUM_ERROR      0x02
#define NULL_ERROR          0x03
#define TIME_OUT_ERROR		0x04

/* ----------------------根据需要修改-------------------------------------*/
#define SCL_PIN 		            GPIO_Pin_11
#define SDA_PIN 		            GPIO_Pin_12
#define I2C_PORT 		            GPIOA
#define RCC_AHBPeriph_I2C_PORT 	RCC_AHBPeriph_GPIOA

#define SDA_IN()  \
{\
  GPIO_InitTypeDef  GPIO_InitStructure;\
  GPIO_InitStructure.GPIO_Pin = SDA_PIN;\
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;\
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;\
  GPIO_Init(I2C_PORT, &GPIO_InitStructure);\
}
#define SDA_OUT() \
{\
  GPIO_InitTypeDef  GPIO_InitStructure;\
  GPIO_InitStructure.GPIO_Pin = SDA_PIN;\
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;\
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;\
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;\
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;\
  GPIO_Init(I2C_PORT, &GPIO_InitStructure);\
}

/* ------------------------------------------------------------------*/
#define SCL_H         do{GPIO_SetBits(I2C_PORT, SCL_PIN);}while(__LINE__==-1)
#define SCL_L         do{GPIO_ResetBits(I2C_PORT, SCL_PIN);}while(__LINE__==-1)
   
#define SDA_H         do{GPIO_SetBits(I2C_PORT, SDA_PIN);}while(__LINE__==-1)
#define SDA_L         do{GPIO_ResetBits(I2C_PORT, SDA_PIN);}while(__LINE__==-1)

#define SDA_read      GPIO_ReadInputDataBit(I2C_PORT, SDA_PIN)

bool I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NAck(void);
bool I2C_WaitAck(void);
void I2C_WriteByte(uint8_t WriteByte);
uint8_t I2C_ReadByte(void);

void I2C_Config(void);
int I2C_WriteCommand8(uint8_t slaveAddr, uint8_t cmd);
int I2C_WriteCommand16(uint8_t slaveAddr, uint16_t cmd);
int I2C_WriteBytesAfterCommand(uint8_t slaveAddr, const uint8_t *const pregValue, int len);
int I2C_ReadBytesAfterCommand(uint8_t slaveAddr, uint8_t *const pregValue, int len);
int I2C_WriteRegisterBytes(uint8_t slaveAddr, uint8_t regAddr, const uint8_t *const pregValue, int len);
int I2C_ReadRegisterBytes(uint8_t slaveAddr, uint8_t regAddr, uint8_t *const pregValue, int len);

#endif 



