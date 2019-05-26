/**
  ******************************************************************************
  * @file    STM32F0xx_IAP/inc/flash_if.h
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
#ifndef __FLASH_IF_H
#define __FLASH_IF_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

/*
256KB = 0x40000
0~0x0ffff   bootloader(64K)
0x10000~0x1ffff   app_a(64K)
0x20000~0x2ffff   app_b(64K)
0x30000~0x3ffff   blank(64K)
*/


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define FLASH_PAGE_SIZE               0x800      /* 2 Kbytes */

/* define the address from where user application will be loaded,
   the application address should be a start sector address */
#if BOOTLOADER | APP_B      //更新APP_A的固件
#define USER_FLASH_LAST_PAGE_ADDRESS  0x0801F800
#define USER_FLASH_END_ADDRESS        0x0801FFFF
#define APPLICATION_ADDRESS     (uint32_t)0x08010000
#elif APP_A                 //更新APP_B的固件
#define USER_FLASH_LAST_PAGE_ADDRESS  0x0802F800
#define USER_FLASH_END_ADDRESS        0x0802FFFF
#define APPLICATION_ADDRESS     (uint32_t)0x08020000
#endif

/* Get the number of Sector from where the user program will be loaded */
#define  FLASH_PAGE_NUMBER      (uint32_t)((APPLICATION_ADDRESS - 0x08000000) >> 12)

/* Compute the mask to test if the Flash memory, where the user program will be
   loaded, is write protected */
#define  FLASH_PROTECTED_PAGES   ((uint32_t)~((1 << FLASH_PAGE_NUMBER) - 1))

/* define the user application size */
#define USER_FLASH_SIZE   (USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS + 1)




/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void FLASH_If_Init(void);
uint32_t FLASH_If_Erase(uint32_t StartSector);
uint32_t FLASH_If_Write(__IO uint32_t* FlashAddress, uint32_t* Data, uint16_t DataLength);
uint32_t FLASH_If_DisableWriteProtection(void);
uint32_t FLASH_If_GetWriteProtectionStatus(void);

#endif  /* __FLASH_IF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

