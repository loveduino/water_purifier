/**
  ******************************************************************************
  * @file    STM32F0xx_IAP/binary_template/main.h 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-May-2012
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "stm320518_eval.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "gprs.h"

#include "crypto.h"
#include "i2c_soft.h"
#include "rtc.h"
#include "e2prom.h"
#include "led.h"
#include "menu.h"

#include "protocol.h"
#include "serial.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
//制水泵和进水电磁阀
#define EN1_PIN 		            GPIO_Pin_3
#define EN1_PORT 		            GPIOA
#define RCC_AHBPeriph_EN1_PORT 	    RCC_AHBPeriph_GPIOA
//组合电磁阀（冲洗废水用）
#define EN2_PIN 		            GPIO_Pin_7
#define EN2_PORT 		            GPIOA
#define RCC_AHBPeriph_EN2_PORT 	    RCC_AHBPeriph_GPIOA

#define EN1_H                       do{GPIO_SetBits(EN1_PORT, EN1_PIN);}while(__LINE__==-1)
#define EN1_L                       do{GPIO_ResetBits(EN1_PORT, EN1_PIN);}while(__LINE__==-1)

#define EN2_H                       do{GPIO_SetBits(EN2_PORT, EN2_PIN);}while(__LINE__==-1)
#define EN2_L                       do{GPIO_ResetBits(EN2_PORT, EN2_PIN);}while(__LINE__==-1)

#define BEEP_PIN 		            GPIO_Pin_15
#define BEEP_PORT 		            GPIOA
#define RCC_AHBPeriph_BEEP_PORT 	RCC_AHBPeriph_GPIOA

#define BEEP_ON                     do{GPIO_SetBits(BEEP_PORT, BEEP_PIN);}while(__LINE__==-1)
#define BEEP_OFF                    do{GPIO_ResetBits(BEEP_PORT, BEEP_PIN);}while(__LINE__==-1)

/* Exported functions ------------------------------------------------------- */
void wait_us(int us);
void wait_ms(int ms);
void wait(float s);

typedef enum{
    NONE = 0,
    SHORT,
    DOUBLE,
    LONG
}KeyClick_t;

extern KeyClick_t KeyClick;
extern bool key_fall_flag;


typedef enum{
    WORK_STOP = 0,//停止
    WORK_MAKE,//製水
    WORK_BUSH//沖洗
}WorkMode_t;

void WorkMode(WorkMode_t work_mode);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
