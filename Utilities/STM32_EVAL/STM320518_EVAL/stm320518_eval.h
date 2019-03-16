/**
  ******************************************************************************
  * @file    stm320518_eval.h
  * @author  MCD Application Team
  * @version V1.1.1
  * @date    16-January-2014
  * @brief   This file contains definitions for STM320518_EVAL's Leds, push-buttons
  *          and COM ports hardware resources.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
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
#ifndef __STM320518_EVAL_H
#define __STM320518_EVAL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
//#include "stm32_eval_legacy.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_EVAL
  * @{
  */

/** @addtogroup STM320518_EVAL
  * @{
  */
      
/** @addtogroup STM320518_EVAL_LOW_LEVEL
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/
typedef enum 
{
  LED1 = 0,
  LED2 = 1,
  BLUE = LED1,
  YELLOW = LED2,
  BUSH_LED = LED1,
  RUN_LED = LED2
} Led_TypeDef;

typedef enum 
{
  BUTTON_IO1 = 0,//高压开关             =0，制水，                    =1，制水完毕
  BUTTON_IO2 = 1,//低压开关             =0，自来水管故障水压异常，    =1自来水管水压正常
  BUTTON_KEY = 2
} Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

typedef enum 
{
  COM1 = 0
} COM_TypeDef;

/** @defgroup STM320518_EVAL_LOW_LEVEL_Exported_Constants
  * @{
  */ 

/** 
  * @brief  Define for STM320518_EVAL board  
  */ 
#if !defined (USE_STM320518_EVAL)
 #define USE_STM320518_EVAL
#endif

/**
 * @brief Select the revision A or B(default) of the EVAL board used.  
 */
#if !defined (STM320518_EVAL_REVB)
 #define STM320518_EVAL_REVB
#endif

#if !defined (STM320518_EVAL_REVA)
 /* #define STM320518_EVAL_REVA */
#endif


/** @addtogroup STM320518_EVAL_LOW_LEVEL_LED
  * @{
  */
#define LEDn                             2

#define LED1_PIN                         GPIO_Pin_0
#define LED1_GPIO_PORT                   GPIOF
#define LED1_GPIO_CLK                    RCC_AHBPeriph_GPIOF
  
#define LED2_PIN                         GPIO_Pin_1
#define LED2_GPIO_PORT                   GPIOF
#define LED2_GPIO_CLK                    RCC_AHBPeriph_GPIOF

/**
  * @}
  */ 

/** @addtogroup STM320518_EVAL_LOW_LEVEL_BUTTON
  * @{
  */  
#define BUTTONn                          3

/**
 * @brief IO1 input     高压开关，水满（水满-常开，水没有满-常闭）
 */
#define IO1_BUTTON_PIN                   GPIO_Pin_5
#define IO1_BUTTON_GPIO_PORT             GPIOA
#define IO1_BUTTON_GPIO_CLK              RCC_AHBPeriph_GPIOA
#define IO1_BUTTON_EXTI_LINE             EXTI_Line5
#define IO1_BUTTON_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOA
#define IO1_BUTTON_EXTI_PIN_SOURCE       EXTI_PinSource5
#define IO1_BUTTON_EXTI_IRQn             EXTI4_15_IRQn

/**
 * @brief IO2 input     低压开关，进水端检测是否故障（水压低-常开，水压正常-常闭）
 */
#define IO2_BUTTON_PIN                   GPIO_Pin_4
#define IO2_BUTTON_GPIO_PORT             GPIOA
#define IO2_BUTTON_GPIO_CLK              RCC_AHBPeriph_GPIOA
#define IO2_BUTTON_EXTI_LINE             EXTI_Line4
#define IO2_BUTTON_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOA
#define IO2_BUTTON_EXTI_PIN_SOURCE       EXTI_PinSource4
#define IO2_BUTTON_EXTI_IRQn             EXTI4_15_IRQn 

/**
 * @brief Key push-button
 */
#define KEY_BUTTON_PIN                   GPIO_Pin_15
#define KEY_BUTTON_GPIO_PORT             GPIOC
#define KEY_BUTTON_GPIO_CLK              RCC_AHBPeriph_GPIOC
#define KEY_BUTTON_EXTI_LINE             EXTI_Line15
#define KEY_BUTTON_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOC
#define KEY_BUTTON_EXTI_PIN_SOURCE       EXTI_PinSource15
#define KEY_BUTTON_EXTI_IRQn             EXTI4_15_IRQn

/**
  * @}
  */ 


/** @addtogroup STM320518_EVAL_LOW_LEVEL_COM
  * @{
  */
#define COMn                             1

/**
 * @brief Definition for COM port1, connected to USART1
 */ 
#define EVAL_COM1                        USART1
#define EVAL_COM1_CLK                    RCC_APB2Periph_USART1

#define EVAL_COM1_TX_PIN                 GPIO_Pin_9
#define EVAL_COM1_TX_GPIO_PORT           GPIOA
#define EVAL_COM1_TX_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define EVAL_COM1_TX_SOURCE              GPIO_PinSource9
#define EVAL_COM1_TX_AF                  GPIO_AF_1

#define EVAL_COM1_RX_PIN                 GPIO_Pin_10
#define EVAL_COM1_RX_GPIO_PORT           GPIOA
#define EVAL_COM1_RX_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define EVAL_COM1_RX_SOURCE              GPIO_PinSource10
#define EVAL_COM1_RX_AF                  GPIO_AF_1
   
#define EVAL_COM1_IRQn                   USART1_IRQn
#define EVAL_COM1_IRQHandler             USART1_IRQHandler
/**
  * @}
  */



/**
  * @}
  */



/**
  * @}
  */

/** @defgroup STM320518_EVAL_LOW_LEVEL_Exported_Functions
  * @{
  */
void STM_EVAL_LEDInit(Led_TypeDef Led);
void STM_EVAL_LEDOn(Led_TypeDef Led);
void STM_EVAL_LEDOff(Led_TypeDef Led);
void STM_EVAL_LEDToggle(Led_TypeDef Led);
void STM_EVAL_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint32_t STM_EVAL_PBGetState(Button_TypeDef Button);
void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct);

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __STM320518_EVAL_H */
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
