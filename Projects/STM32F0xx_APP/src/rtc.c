#include <time.h>
#include "rtc.h"
#include "i2c_soft.h"


unsigned char BCD2HEX(unsigned char bcd_data)    //BCD转为HEX(or十进制)子程序  ,0x12(BCD) -> 12(DEC)  转小
{   
    return (((bcd_data>>4)*10) + (bcd_data & 0x0F));
}

unsigned char HEX2BCD(unsigned char hex_data)    //HEX(or十进制)转为BCD子程序 ,12(DEC) -> 0x12(BCD)   转大
{   
    return (((hex_data/10)<<4) + (hex_data%10));   
}

void PCF8563_BufferWrite(uint8_t WriteAddr, const uint8_t* const pBuffer, int NumByteToWrite)
{
    I2C_WriteCommand8(PCF8563_SLAVE_ADDR, WriteAddr);
    I2C_WriteBytesAfterCommand(PCF8563_SLAVE_ADDR, pBuffer, NumByteToWrite);
}

void PCF8563_BufferRead(uint8_t ReadAddr, uint8_t* const pBuffer, uint16_t NumByteToRead)
{
    I2C_WriteCommand8(PCF8563_SLAVE_ADDR, ReadAddr);
    I2C_ReadBytesAfterCommand(PCF8563_SLAVE_ADDR, pBuffer, NumByteToRead);
}

/*
struct tm {
  int tm_sec;   // 秒，正常范围从 0 到 59，但允许至 61
  int tm_min;   // 分，范围从 0 到 59
  int tm_hour;  // 小时，范围从 0 到 23
  int tm_mday;  // 一月中的第几天，范围从 1 到 31
  int tm_mon;   // 月，范围从 0 到 11
  int tm_year;  // 自 1900 年起的年数
  int tm_wday;  // 一周中的第几天，范围从 0 到 6，从星期日算起，星期日表示为 0
  int tm_yday;  // 一年中的第几天，范围从 0 到 365，从 1 月 1 日算起
  int tm_isdst; // 夏令时
}

UTC的时间是从公元1970年1月1日0时0分0秒开始的。
time库的时间是从公元1900年1月1日0时0分0秒开始的。
两者相差2208988800s

GMT 0:00 格林尼治标准时间
UT +00:00 全球时间
UTC +00:00 校准的全球时间
*/
//从公元1970年1月1日0时0分0秒算起至今的UTC时间所经过的秒数，转换为日期时间写入到RTC中
void PCF8563_SetTime(time_t seconds)//根据UTC时间设置本地时间，如果要设置当地的北京时间，需要获取当前北京时间对应的UTC时间
{
    uint8_t time_value[7];
    time_t _seconds = seconds;
    struct tm *set_time = gmtime(&_seconds);
    
    time_value[6]/*00–99 */ = HEX2BCD((unsigned char)(set_time->tm_year + 1900 - 2000)) & 0xFF;
    time_value[5]/*01–12 */ = HEX2BCD((unsigned char)(set_time->tm_mon + 1)) & 0x1F;   
    time_value[4]/*00–06 */ = HEX2BCD((unsigned char)(set_time->tm_wday)) & 0x07; // 一周中的第几天，范围从 0 到 6，从星期日算起，星期日表示为 0
    time_value[3]/*01–31 */ = HEX2BCD((unsigned char)(set_time->tm_mday)) & 0x3F;
    time_value[2]/*00–23 */ = HEX2BCD((unsigned char)(set_time->tm_hour)) & 0x3F;  
    time_value[1]/*00–59 */ = HEX2BCD((unsigned char)(set_time->tm_min)) & 0x7F;   
    time_value[0]/*00–59 */ = HEX2BCD((unsigned char)(set_time->tm_sec)) & 0x7F;   
    PCF8563_BufferWrite(PCF8563_REG_SECONDS, time_value, 7);
}

//int year;
//从公元1970年1月1日0时0分0 秒算起至今的UTC时间所经过的秒数，返回的是UTC时间，而不是北京时间
time_t PCF8563_ReadTime(void)
{
    uint8_t time_value[7];
    struct tm read_time;
    
    PCF8563_BufferRead(PCF8563_REG_SECONDS, time_value, 7);
    //year = BCD2HEX(time_value[6]);
    read_time.tm_year = BCD2HEX(time_value[6]&0xFF) + 2000 - 1900;
    read_time.tm_mon =  BCD2HEX(time_value[5]&0x1F) - 1;
    read_time.tm_wday = BCD2HEX(time_value[4]&0x07);
    read_time.tm_mday = BCD2HEX(time_value[3]&0x3F);
    read_time.tm_hour = BCD2HEX(time_value[2]&0x3F);
    read_time.tm_min =  BCD2HEX(time_value[1]&0x7F);
    read_time.tm_sec =  BCD2HEX(time_value[0]&0x7F);
    
//    for (int i = 0; i < 7; i++)
//    {
//        printf("time_value[%d] = %02x\r\n", i, time_value[i]);
//    }
//    printf("\r\n\r\n");
//    
//    printf("%d %d %d  %d:%d:%d\r\n", read_time.tm_year, read_time.tm_mon, read_time.tm_mday, read_time.tm_hour, read_time.tm_min, read_time.tm_sec);
//    
    read_time.tm_isdst = -1;
    //mktime()用来将参数timeptr所指的tm结构数据转换成从公元1970年1月1日0时0分0 秒算起至今的UTC时间所经过的秒数。
    return mktime(&read_time);//并将时间初始化给系统time库时间
}


