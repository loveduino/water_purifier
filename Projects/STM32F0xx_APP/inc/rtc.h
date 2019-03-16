#ifndef __RTC_H
#define __RTC_H

#include "stm32f0xx.h"


//调试宏开关
#define PCF8563_DBUG	1
#if PCF8563_DBUG
	#include "main.h"
	#define PCF8563_Debug(format,...)	printf(format,##__VA_ARGS__)
#else
	#define PCF8563_Debug(format,...)	
#endif	//PCF8563_DBUG


//PCF8563 iic总线地址
#define PCF8563_SLAVE_ADDR			    0xA2

//PCF8563 寄存器
typedef enum
{
	PCF8563_REG_CONTROL1			=	0x00,	//控制寄存器1
	PCF8563_REG_CONTROL2			=	0x01,	//控制寄存器2
	PCF8563_REG_SECONDS				=	0x02,	//秒 seconds 00 to 59 coded in BCD
	PCF8563_REG_MINUTES				=	0x03,	//分 minutes 00 to 59 coded in BCD
	PCF8563_REG_HOURS				=	0x04,	//小时 hours 00 to 23 coded in BCD
	PCF8563_REG_DAYS				=	0x05,	//日 days 01 to 31 coded in BCD
	PCF8563_REG_WEEK				=	0x06,	//星期 weekdays 0 to 6 in BCD
	PCF8563_REG_MONTHS				=	0x07,	//月份 months 01 to 12 coded in BCD
	PCF8563_REG_YEARS				=	0x08,	//年份 years 00 to 99 coded in BCD
	PCF8563_REG_ALARM_MINUTE		=	0x09,	//闹钟，分钟 minute alarm 00 to 59 coded in BCD
	PCF8563_REG_ALARM_HOUR			=	0x0A,	//闹钟，小时 hour alarm 00 to 23 coded in BCD
	PCF8563_REG_ALARM_DAY			=	0x0B,	//闹钟，日 day alarm 01 to 31 coded in BCD
	PCF8563_REG_ALARM_WEEK			=	0x0C,	//闹钟，星期 weekday alarm 0 to 6 in BCD
	PCF8563_REG_CLKOUT				=	0x0D,	//时钟输出设置
	PCF8563_REG_TIME_CONTROL		=	0x0E,	//定时器设置
	PCF8563_REG_TIME				=	0x0F,	//定时器倒计数值
}PCF8563_REG_TYPE;


void PCF8563_SetTime(time_t seconds);//根据UTC时间设置本地时间，如果要设置当地的北京时间，需要获取当前北京时间对应的UTC时间
//从公元1970年1月1日0时0分0 秒算起至今的UTC时间所经过的秒数，返回的是UTC时间，而不是北京时间
time_t PCF8563_ReadTime(void);

#endif//__RTC_H