#include "i2c_soft.h"   
#include <stdbool.h>

void I2C_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_I2C_PORT, ENABLE);
    /* Configure I2C1 pins: SCL and SDA */
    GPIO_InitStructure.GPIO_Pin 	= SCL_PIN | SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz  
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉  
    GPIO_Init(I2C_PORT, &GPIO_InitStructure);
}

static void I2C_Delay(void)
{	
	int i = 10; 						//这里可以优化速度	，经测试最低到5还能写入
	
	while(i--)
	{
        asm("nop");
	}
}

//START:when CLK is high,DATA change form high to low 
bool I2C_Start(void)
{
	SDA_OUT();
	SDA_H;
	SCL_H;
	I2C_Delay();
	if (!SDA_read)return false;	//SDA线为低电平则总线忙,退出
	SDA_L;
	I2C_Delay();
	if (SDA_read) return false;	//SDA线为高电平则总线出错,退出
	SDA_L;
	I2C_Delay();				//钳住I2C总线，准备发送或接收数据 
	return true;
}

//STOP:when CLK is high DATA change form low to high
void I2C_Stop(void)
{
	SDA_OUT(); 
	SCL_L;
	I2C_Delay();
	SDA_L;
	I2C_Delay();
	SCL_H;
	I2C_Delay();
	SDA_H;//发送I2C总线结束信号
	I2C_Delay();
}

//产生ACK应答
void I2C_Ack(void)
{	
	SDA_OUT();
	SCL_L;
	I2C_Delay();
	SDA_L;
	I2C_Delay();
	SCL_H;
	I2C_Delay();
	SCL_L;
	I2C_Delay();
}

//不产生ACK应答	
void I2C_NAck(void)
{	
	SDA_OUT();
	SCL_L;
	I2C_Delay();
	SDA_H;
	I2C_Delay();
	SCL_H;
	I2C_Delay();
	SCL_L;
	I2C_Delay();
}

//等待应答信号到来
//返回值：false，接收应答失败
//       true，接收应答成功
bool I2C_WaitAck(void)
{
	SDA_IN();
	SCL_L;
	I2C_Delay();
	SDA_H;
	I2C_Delay();
	SCL_H;
	I2C_Delay();
	if (SDA_read)
	{
		SCL_L;
		return false;
	}
	SCL_L;
	return true;
}

void I2C_WriteByte(uint8_t WriteByte) //数据从高位到低位
{
	uint8_t i = 8;
	
	SDA_OUT(); 
	while (i--)
	{
		SCL_L;
		I2C_Delay();
		if (WriteByte & 0x80)
			SDA_H;  
		else 
			SDA_L;   
		WriteByte <<= 1;
		I2C_Delay();
		SCL_H;
		I2C_Delay();
	}
	SCL_L;
}

uint8_t I2C_ReadByte(void)  //数据从高位到低位
{
	uint8_t i = 8;
	uint8_t ReadByte = 0;

	SDA_IN();
	SDA_H;
	while (i--)
	{
		ReadByte <<= 1;
		SCL_L;
		I2C_Delay();
		SCL_H;
		I2C_Delay();	
		if (SDA_read)
		{
			ReadByte |= 0x01;
		}
	}
	SCL_L;
	return ReadByte;
}

int I2C_WriteCommand8(uint8_t slaveAddr, uint8_t cmd)
{
	int err = 0;

	I2C_Start();
	I2C_WriteByte(slaveAddr & 0xFE);
	if (false == I2C_WaitAck())
		err |=  ACK_ERROR;
	I2C_WriteByte(cmd);
	if (false == I2C_WaitAck())
		err |=  ACK_ERROR;
	//I2C_Stop();
    return err;
}

int I2C_WriteCommand16(uint8_t slaveAddr, uint16_t cmd)
{
	int err = 0;

	I2C_Start();
	I2C_WriteByte(slaveAddr & 0xFE);
	if (false == I2C_WaitAck())
		err |=  ACK_ERROR;
	I2C_WriteByte((uint8_t)(cmd >> 8));
	if (false == I2C_WaitAck())
		err |=  ACK_ERROR;
	I2C_WriteByte((uint8_t)(cmd&0xFF));
	if (false == I2C_WaitAck())
		err |=  ACK_ERROR;
	//I2C_Stop();
    return err;
}

int I2C_WriteBytesAfterCommand(uint8_t slaveAddr, const uint8_t *const pregValue, int len)
{
	int err = 0;
	int i;
	for (i = 0; i < len; i++)
	{
		I2C_WriteByte(*(pregValue + i));
		if (false == I2C_WaitAck())
			err |=  ACK_ERROR;
	}
	I2C_Stop();
    return err;
}

int I2C_ReadBytesAfterCommand(uint8_t slaveAddr, uint8_t *const pregValue, int len)
{
	int err = 0;
	int i;
	I2C_Start();
	I2C_WriteByte(slaveAddr | 0x01);
	if (false == I2C_WaitAck())
		err |=  ACK_ERROR;
	for (i = 0; i < len - 1; i++)
	{
		*(pregValue + i) = I2C_ReadByte();
		I2C_Ack();
	}
	*(pregValue + i) = I2C_ReadByte();
	I2C_NAck();
	I2C_Stop();
    return err;
}

//===========================================================================
int I2C_WriteRegisterBytes(uint8_t slaveAddr, uint8_t regAddr, const uint8_t *const pregValue, int len)
//===========================================================================
{
	int err = 0;
	int i;
	I2C_Start();
	I2C_WriteByte(slaveAddr & 0xFE);
	if (false == I2C_WaitAck())
		err |=  ACK_ERROR;
	I2C_WriteByte(regAddr);
	if (false == I2C_WaitAck())
		err |=  ACK_ERROR;
	for (i = 0; i < len; i++)
	{
		I2C_WriteByte(*(pregValue + i));
		if (false == I2C_WaitAck())
			err |=  ACK_ERROR;
	}
	I2C_Stop();
    return err;
}

//===========================================================================
int I2C_ReadRegisterBytes(uint8_t slaveAddr, uint8_t regAddr, uint8_t *const pregValue, int len)
//===========================================================================
{
	int err = 0;
	int i;
	I2C_Start();
	I2C_WriteByte(slaveAddr & 0xFE);
	if (false == I2C_WaitAck())
		err |=  ACK_ERROR;
	I2C_WriteByte(regAddr);
	if (false == I2C_WaitAck())
		err |=  ACK_ERROR;
	//I2C_Stop();
	I2C_Start();
	I2C_WriteByte(slaveAddr | 0x01);
	if (false == I2C_WaitAck())
		err |=  ACK_ERROR;
	for (i = 0; i < len - 1; i++)
	{
		*(pregValue + i) = I2C_ReadByte();
		I2C_Ack();
	}
	*(pregValue + i) = I2C_ReadByte();
	I2C_NAck();
	I2C_Stop();
    return err;
}


