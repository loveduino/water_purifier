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

#include "e2prom.h"
#include "protocol.h"


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


/* Private functions ---------------------------------------------------------*/
#include "stdio.h"

/**
  * @brief  Download a file via serial port
  * @param  None
  * @retval None
  */
#if 0
int32_t SerialDownload(void)
{
  uint8_t Number[10] = {0};
  int32_t Size = 0;

  SerialPutString("Waiting for the file to be sent ... (press 'a' to abort)\n\r");
  Size = Ymodem_Receive(&tab_1024[0]);
  vTaskDelay(pdMS_TO_TICKS(100));
  printf("\r\nSize = %d\r\n", Size);
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
  return Size;
}
#else
int32_t SerialDownload(void)
{
  //uint8_t Number[10] = {0};
  int32_t Size = 0;

  //SerialPutString("Waiting for the file to be sent ... (press 'a' to abort)\n\r");
  Size = Ymodem_Receive(&tab_1024[0]);
  vTaskDelay(pdMS_TO_TICKS(100));
  //printf("\r\nSize = %d\r\n", Size);
  if (Size > 0)
  {
    //SerialPutString("\n\n\r Programming Completed Successfully!\n\r--------------------------------\r\n Name: ");
    //SerialPutString(FileName);
    //Int2Str(Number, Size);
    //SerialPutString("\n\r Size: ");
    //SerialPutString(Number);
    //SerialPutString(" Bytes\r\n");
    //SerialPutString("-------------------\n");
  }
  else if (Size == -1)
  {
    //SerialPutString("\n\n\rThe image size is higher than the allowed space memory!\n\r");
  }
  else if (Size == -2)
  {
    //SerialPutString("\n\n\rVerification failed!\n\r");
  }
  else if (Size == -3)
  {
    //SerialPutString("\r\n\nAborted by user.\n\r");
  }
  else
  {
    //SerialPutString("\n\rFailed to receive the file!\n\r");
  }
  return Size;
}
#endif

/**
  * @brief  Upload a file via serial port.
  * @param  None
  * @retval None
  */
void SerialUpload(void)
{
  uint8_t status = 0 ; 

  SerialPutString("\n\n\rSelect Receive File\n\r");

  if (GetKey() == CRC16)
  {
    /* Transmit the flash image through ymodem protocol */
    status = Ymodem_Transmit((uint8_t*)APPLICATION_ADDRESS, (const uint8_t*)"UploadedFlashImage.bin", USER_FLASH_SIZE);

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
#if BOOTLOADER
  SerialPutString("\r\n=               BOOTLOADER                                           =");
#elif APP_A
  SerialPutString("\r\n=               APP_A                                                =");
#elif APP_B
  SerialPutString("\r\n=               APP_B                                                =");
#endif
  SerialPutString("\r\n=  STM32F0xx In-Application Programming Application  (Version 1.0.0) =");
  SerialPutString("\r\n=                                                                    =");
  SerialPutString("\r\n=                                   By MCD Application Team          =");
  SerialPutString("\r\n======================================================================");
  SerialPutString("\r\n\r\n");

  /* Test if any sector of Flash memory where user application will be loaded is write protected */
  if (FLASH_If_GetWriteProtectionStatus() != 0)
  {
    FlashProtection = 1;
  }
  else
  {
    FlashProtection = 0;
  }

  while (1)
  {
    SerialPutString("\r\n================== Main Menu ============================\r\n\n");
    SerialPutString("  Download Image To the STM32F0xx Internal Flash ------- 1\r\n\n");
    SerialPutString("  Upload Image From the STM32F0xx Internal Flash ------- 2\r\n\n");
    SerialPutString("  Execute The New Program ------------------------------ 3\r\n\n");

    if(FlashProtection != 0)
    {
      SerialPutString("  Disable the write protection ------------------------- 4\r\n\n");
    }

    SerialPutString("==========================================================\r\n\n");

    /* Receive key */
    key = GetKey();

    if (key == '1')
    {
      /* Download user application in the Flash */
      SerialDownload();
    }
    else if (key == '2')
    {
      /* Upload user application from the Flash */
      //SerialUpload();
        
#if APP_A
    const char app_run[10] = "app-b-on";
    I2C_EE_BufferWrite(0+sizeof(save_data_default)+sizeof(g_data_format.iccid), (uint8_t *)app_run, 10);
#elif APP_B | BOOTLOADER
    const char app_run[10] = "app-a-on";
    I2C_EE_BufferWrite(0+sizeof(save_data_default)+sizeof(g_data_format.iccid), (uint8_t *)app_run, 10);
#endif
        
    }
    else if (key == '3') /* execute the new program */
    {
        
        NVIC_SystemReset();
        
        //vPortEndScheduler();
        //vTaskSuspendAll();
        
        //USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);   //????????
        //USART_ITConfig(USART1, USART_IT_ORE, DISABLE);    //????????        portDISABLE_INTERRUPTS() ;
        
        // __disable_irq();
        
        //__set_PRIMASK(1);
        //__ASM("CPSID I");
        
	/* Stop and reset the SysTick. */
	//*(( ( volatile uint32_t * ) 0xe000e010 )) = 0UL;
//	*(( ( volatile uint32_t * ) 0xe000e018 )) = 0UL;
//        
//        taskDISABLE_INTERRUPTS();
        
      /* Jump to user application */
      JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
      Jump_To_Application = (pFunction) JumpAddress;
      
      /* Initialize user application's Stack Pointer */
      __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
      
      /* Jump to application */
      Jump_To_Application();
    }
    else if ((key == '4') && (FlashProtection == 1))
    {
      /* Disable the write protection */
      writeprotect = FLASH_If_DisableWriteProtection();

      switch (writeprotect)
      {
        case 0:
        {
          SerialPutString("Write Protection disabled...\r\n");
          SerialPutString("...and a System Reset will be generated to reload the new option bytes\r\n");
          
          /* Launch loading new option bytes */
          FLASH_OB_Launch();
          
          break;
        }
        case 1:
        {
          SerialPutString("Error: Flash write unprotection failed...\r\n");
          break;
        }
        case 2:
        {
          SerialPutString("Flash memory not write protected\r\n");
          break;
        }
        default:
        {
        }
      }
    }
    else
    {
      if (FlashProtection == 0)
      {
        SerialPutString("Invalid Number ! ==> The number should be either 1, 2 or 3\r");
      }
      else
      {
        SerialPutString("Invalid Number ! ==> The number should be either 1, 2, 3 or 4\r");
      }
    }
  }
}

/**
  * @brief  Initialize the IAP.
  * @param  None
  * @retval None
  */
void IAP_Init(void)
{
  /* Unlock the Flash Program Erase controller */
  FLASH_If_Init();

  /* USART resources configuration (Clock, GPIO pins and USART registers) ----*/
  /* USART configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  //xSerialPortInit( ser115200, 100, 1000 );
}


void prvIapTask( void *pvParameters )
{
	/* Just to stop compiler warnings. */
	( void ) pvParameters;
    
    /* If Key is pressed, execute the IAP driver in order to re-program the Flash */
    IAP_Init();
    
    /* Display main menu */
    Main_Menu ();
}

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
