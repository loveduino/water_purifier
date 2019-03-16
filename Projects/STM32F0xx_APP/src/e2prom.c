#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "i2c_soft.h"
#include "e2prom.h"

#define I2C_PageSize           8			/* AT24C02每页有8个字节 */


/*
 * 函数名：I2C_EE_WaitEepromStandbyState
 * 描述  ：Wait for EEPROM Standby state 
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 * 调用  ： 
 */
/*
EEPROM在接收完数据后，启动内部周期写入数据的时间不会对主机的请求作出应答的特性。
所以利用这个函数循环发送起始信号，若检测到EEPROM的应答，则说明EEPROM已经完成上一步的数据写入，进入standby状态，可以进行下一步的操作。
*/
void I2C_EE_WaitEepromStandbyState(void)
{
    wait_ms(5);
}

/*
* 函数名：I2C_EE_BufferWrite
* 描述  ：将缓冲区中的数据写到I2C EEPROM中
* 输入  ：-pBuffer 缓冲区指针
*         -WriteAddr 接收数据的EEPROM的地址
*         -NumByteToWrite 要写入EEPROM的字节数
* 输出  ：无
* 返回  ：无
* 调用  ：外部调用
*/
void I2C_EE_BufferWrite(uint8_t WriteAddr, const uint8_t* const pBuffer, uint16_t NumByteToWrite)
{
    uint8_t *_pBuffer = (uint8_t *)pBuffer;
    uint8_t NumOfFirstPageAvailable = I2C_PageSize - WriteAddr % I2C_PageSize;
    //页是绝对的，按整页大小排列，不是从开始写入的地址开始算。
    //页写与字节写的区别就是，页写可以一次写多个数据，而字节写只能一次写一个数据。
    //但由于AT24C02A的一页才8个字节，所以页写也最多写8个数据，而且只能在该页内写，不会发生一次页写同时写两页的情况。
    if (NumByteToWrite <= NumOfFirstPageAvailable)
    {
        I2C_WriteRegisterBytes(E2PROM_SLAVE_ADDR, WriteAddr, _pBuffer, NumByteToWrite);
        I2C_EE_WaitEepromStandbyState();
    }
    else
    {
        I2C_WriteRegisterBytes(E2PROM_SLAVE_ADDR, WriteAddr, _pBuffer, NumOfFirstPageAvailable);
        I2C_EE_WaitEepromStandbyState();
        
        uint16_t NumOfRemainPages = (NumByteToWrite - NumOfFirstPageAvailable) / I2C_PageSize;
        uint8_t NumOfRemainBytes = (NumByteToWrite - NumOfFirstPageAvailable) % I2C_PageSize;

        WriteAddr +=  NumOfFirstPageAvailable;
        _pBuffer += NumOfFirstPageAvailable;
        I2C_EE_WaitEepromStandbyState();
        while(NumOfRemainPages--)
        {
            I2C_WriteRegisterBytes(E2PROM_SLAVE_ADDR, WriteAddr, _pBuffer, I2C_PageSize);//一次可以写入8个字节的数据
            I2C_EE_WaitEepromStandbyState();
            WriteAddr +=  I2C_PageSize;
            _pBuffer += I2C_PageSize;
        }
        if(NumOfRemainBytes != 0)
        {
            I2C_WriteRegisterBytes(E2PROM_SLAVE_ADDR, WriteAddr, _pBuffer, NumOfRemainBytes);
            I2C_EE_WaitEepromStandbyState();
        }
    }
}

/*
* 函数名：I2C_EE_BufferRead
* 描述  ：从EEPROM里面读取一块数据。 
* 输入  ：-pBuffer 存放从EEPROM读取的数据的缓冲区指针。
*         -WriteAddr 接收数据的EEPROM的地址。 
*         -NumByteToWrite 要从EEPROM读取的字节数。
* 输出  ：无
* 返回  ：无
* 调用  ：外部调用
*/
void I2C_EE_BufferRead(uint8_t ReadAddr, uint8_t* const pBuffer, uint16_t NumByteToRead)
{
    uint8_t *_pBuffer = (uint8_t *)pBuffer;
    //读取数据可以一直读到最后一个地址.序列读没有一页8个字节的限制
    //读没有页的问题，可以从任意地址开始读取任意大小数据，只是超过整个存储器容量时地址才回卷。
    I2C_ReadRegisterBytes(E2PROM_SLAVE_ADDR, ReadAddr, _pBuffer, NumByteToRead);
}

int I2C_EE_BufferWriteSafe(uint8_t WriteAddr, const uint8_t* const pBuffer, uint16_t NumByteToWrite)
{
    uint8_t* pReadBuffer = (uint8_t*)malloc(sizeof(uint8_t) * NumByteToWrite);
    if (NULL == pReadBuffer)
        return -2;
    uint8_t count = 1;
    do {
        I2C_EE_BufferWrite(WriteAddr, pBuffer, NumByteToWrite);
        I2C_EE_BufferRead(WriteAddr, pReadBuffer, NumByteToWrite);
        if (0 == memcmp(pReadBuffer, pBuffer, NumByteToWrite))
        {
            break;
        }
    }while (count++ < 3);
    free(pReadBuffer);
    if (count >= 3)
        return -1;
    else
        return count;
}

void I2C_EE_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_APB2Periph_E2WP_PORT, ENABLE);
    GPIO_InitStructure.GPIO_Pin 	= E2WP_PIN ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
    GPIO_Init(E2WP_PORT, &GPIO_InitStructure);
}

