/**
  ******************************************************************************
  * @file    STM32F0xx_IAP/src/menu.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-May-2012
  * @brief   Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "flash_if.h"
#include "menu.h"
#include "ymodem.h"

/** @addtogroup STM32F0xx_IAP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
pFunction Jump_To_Application;
uint32_t JumpAddress;
extern uint32_t UserMemoryMask;
uint32_t FlashProtection = 0;
uint8_t tab_1024[1024] =
  {
    0
  };
uint8_t FileName[FILE_NAME_LENGTH];

/* Private function prototypes -----------------------------------------------*/
void SerialDownload(uint32_t application_address);
void SerialUpload(uint32_t application_address);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Download a file via serial port
  * @param  None
  * @retval None
  */
void SerialDownload(uint32_t application_address)
{
  uint8_t Number[10] = {0};
  int32_t Size = 0;

  SerialPutString("Waiting for the file to be sent ... (press 'a' to abort)\n\r");
  Size = Ymodem_Receive(&tab_1024[0], application_address);
  if (Size > 0)
  {
    SerialPutString("\n\n\r Programming Completed Successfully!\n\r--------------------------------\r\n Name: ");
    SerialPutString(FileName);
    Int2Str(Number, Size);
    SerialPutString("\n\r Size: ");
    SerialPutString(Number);
    SerialPutString(" Bytes\r\n");
    SerialPutString("-------------------\n");
  }
  else if (Size == -1)
  {
    SerialPutString("\n\n\rThe image size is higher than the allowed space memory!\n\r");
  }
  else if (Size == -2)
  {
    SerialPutString("\n\n\rVerification failed!\n\r");
  }
  else if (Size == -3)
  {
    SerialPutString("\r\n\nAborted by user.\n\r");
  }
  else
  {
    SerialPutString("\n\rFailed to receive the file!\n\r");
  }
}

/**
  * @brief  Upload a file via serial port.
  * @param  None
  * @retval None
  */
void SerialUpload(uint32_t application_address)
{
  uint8_t status = 0 ; 

  SerialPutString("\n\n\rSelect Receive File\n\r");

  if (GetKey() == CRC16)
  {
    uint32_t user_flash_size = 0;
    /* Transmit the flash image through ymodem protocol */
    if (APPLICATION_ADDRESS_A == application_address)
    {
        user_flash_size = USER_FLASH_SIZE_A;
    }
    else if (APPLICATION_ADDRESS_B == application_address)
    {
        user_flash_size = USER_FLASH_SIZE_B;
    }
    status = Ymodem_Transmit((uint8_t*)application_address, (const uint8_t*)"UploadedFlashImage.bin", user_flash_size, application_address);

    if (status != 0) 
    {
      SerialPutString("\n\rError Occurred while Transmitting File\n\r");
    }
    else
    {
      SerialPutString("\n\rFile uploaded successfully \n\r");
    }
  }
}

/**
  * @brief  Display the Main Menu on HyperTerminal
  * @param  None
  * @retval None
  */
void Main_Menu(void)
{
  uint8_t key = 0;
  uint8_t writeprotect = 0;

  SerialPutString("\r\n======================================================================");
  SerialPutString("\r\n=              (C) COPYRIGHT 2012 STMicroelectronics                 =");
  SerialPutString("\r\n=                                                                    =");
  SerialPutString("\r\n=  STM32F0xx In-Application Programming Application  (Version 1.0.0) =");
  SerialPutString("\r\n=                                                                    =");
  SerialPutString("\r\n=                                   By MCD Application Team          =");
  SerialPutString("\r\n======================================================================");
  SerialPutString("\r\n\r\n");

//  /* Test if any sector of Flash memory where user application will be loaded is write protected */
//  if (FLASH_If_GetWriteProtectionStatus() != 0)  
//  {
//    FlashProtection = 1;
//  }
//  else
//  {
//    FlashProtection = 0;
//  }

  while (1)
  {
    SerialPutString("\r\n================== Main Menu ============================\r\n\n");
    SerialPutString("  Download Image To the STM32F0xx Internal Flash a------- 1\r\n\n");
    SerialPutString("  Upload Image From the STM32F0xx Internal Flash a------- 2\r\n\n");
    SerialPutString("  Execute The New Program a------------------------------ 3\r\n\n");
    SerialPutString("  Download Image To the STM32F0xx Internal Flash b------- 4\r\n\n");
    SerialPutString("  Upload Image From the STM32F0xx Internal Flash b------- 5\r\n\n");
    SerialPutString("  Execute The New Program b------------------------------ 6\r\n\n");
    
    if(FlashProtection != 0)
    {
      SerialPutString("  Disable the write protection ------------------------- 6\r\n\n");
    }

    SerialPutString("==========================================================\r\n\n");

    /* Receive key */
    key = GetKey();

    if (key == '1')
    {
      /* Download user application in the Flash */
      SerialDownload(APPLICATION_ADDRESS_A);
    }
    else if (key == '2')
    {
      /* Upload user application from the Flash */
      SerialUpload(APPLICATION_ADDRESS_A);
    }
    else if (key == '3') /* execute the new program */
    {
      /* Jump to user application */
      JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS_A + 4);
      Jump_To_Application = (pFunction) JumpAddress;

      /* Initialize user application's Stack Pointer */
      __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS_A);
      
      /* Jump to application */
      Jump_To_Application();
    }
    else if (key == '4')
    {
      /* Download user application in the Flash */
      SerialDownload(APPLICATION_ADDRESS_B);
    }
    else if (key == '5')
    {
      /* Upload user application from the Flash */
      SerialUpload(APPLICATION_ADDRESS_B);
    }
    else if (key == '6') /* execute the new program */
    {
      /* Jump to user application */
      JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS_B + 4);
      Jump_To_Application = (pFunction) JumpAddress;

      /* Initialize user application's Stack Pointer */
      __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS_B);
      
      /* Jump to application */
      Jump_To_Application();
    }
//    else if ((key == '7') && (FlashProtection == 1))
//    {
//      /* Disable the write protection */
//      writeprotect = FLASH_If_DisableWriteProtection();
//
//      switch (writeprotect)
//      {
//        case 0:
//        {
//          SerialPutString("Write Protection disabled...\r\n");
//          SerialPutString("...and a System Reset will be generated to reload the new option bytes\r\n");
//          
//          /* Launch loading new option bytes */
//          FLASH_OB_Launch();
//          
//          break;
//        }
//        case 1:
//        {
//          SerialPutString("Error: Flash write unprotection failed...\r\n");
//          break;
//        }
//        case 2:
//        {
//          SerialPutString("Flash memory not write protected\r\n");
//          break;
//        }
//        default:
//        {
//        }
//      }
//    }
    else
    {
      if (FlashProtection == 0)
      {
        SerialPutString("Invalid Number !\r");
      }
      else
      {
        SerialPutString("Invalid Number !\r");
      }
    }
  }
}


/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
