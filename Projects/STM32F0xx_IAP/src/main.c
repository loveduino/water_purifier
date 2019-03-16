/**
  ******************************************************************************
  * @file    STM32F0xx_IAP/src/main.c 
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
#include "main.h"

/** @addtogroup STM32F0xx_IAP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
  #define GETCHAR_PROTOTYPE int __io_getchar(FILE *f)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
  #define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern pFunction Jump_To_Application;
extern uint32_t JumpAddress;

/* Private function prototypes -----------------------------------------------*/
static void RCC_Configuration(void);
static void IAP_Init(void);
static void AutoBauRate_StartBitMethod(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */ 
  RCC_Configuration();
  
  /* Initialize Key Button mounted on STM320518-EVAL board */       
  STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_GPIO);

  /* Test if Key push-button on STM320518-EVAL Board is pressed */
  if (STM_EVAL_PBGetState(BUTTON_KEY) != 0x00)
  { 
    /* If Key is pressed, execute the IAP driver in order to re-program the Flash */
    IAP_Init();
    
    /* AutoBaudRate for USART by Start bit Method */
    //AutoBauRate_StartBitMethod();
    
    /* Display main menu */
    Main_Menu ();
  }  
  else
  {/* Keep the user application running */

    /* Test if user code is programmed starting from address "APPLICATION_ADDRESS" */
    if (((*(__IO uint32_t*)APPLICATION_ADDRESS_A) & 0x2FFE0000 ) == 0x20000000)
    {
      /* Jump to user application */
      JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS_A + 4);
      Jump_To_Application = (pFunction) JumpAddress;
      
      /* Initialize user application's Stack Pointer */
      __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS_A);
      
      /* Jump to application */
      Jump_To_Application();
    }
  }

  /* Infinite loop */
  while (1)
  {
  }
}

static void RCC_Configuration(void)
{
    RCC_DeInit();
	/* enable HSI */
	RCC_HSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
	{}
	
	/* Enable Prefetch Buffer */
	FLASH_PrefetchBufferCmd(ENABLE);
	/* set Flash Latency, 0-24M: FLASH_Latency_0, 24M < sysclk <48M: FLASH_Latency_1*/
	FLASH_SetLatency(FLASH_Latency_1);
	
	/* Configures the AHB clock (HCLK)  */
	RCC_HCLKConfig(RCC_CFGR_HPRE_DIV1);
	
	/* Configures the APB clock (PCLK) */
	RCC_PCLKConfig(RCC_CFGR_PPRE_DIV1);
	
	/* Configures PLL source, (8M/2) * 12 = 48M */
	RCC_PLLConfig(RCC_CFGR_PLLSRC_HSI_DIV2, RCC_CFGR_PLLMULL12); 
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
	{}
	
	/* Configures system clock source */
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
    {}
}


/**
  * @brief  Initialize the IAP.
  * @param  None
  * @retval None
  */
void IAP_Init(void)
{
 USART_InitTypeDef USART_InitStructure;

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
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  STM_EVAL_COMInit(COM1, &USART_InitStructure);
}

/**
  * @brief  Start Bit Method to USART AutoBaudRate.
  * @param  None
  * @retval None
  */
static void AutoBauRate_StartBitMethod(void)
{ 
  /* USART enable */
  USART_Cmd(EVAL_COM1, ENABLE);
  
  /* Configure the AutoBaudRate method */
  USART_AutoBaudRateConfig(EVAL_COM1, USART_AutoBaudRate_StartBit);
  
  /* Enable AutoBaudRate feature */
  USART_AutoBaudRateCmd(EVAL_COM1, ENABLE);
  
  /* Wait until Receive enable acknowledge flag is set */
  while(USART_GetFlagStatus(EVAL_COM1, USART_FLAG_REACK) == RESET)
  {}  
  
  /* Wait until Transmit enable acknowledge flag is set */  
  while(USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TEACK) == RESET)
  {}  
  
  /* Loop until the end of Autobaudrate phase */
  while(USART_GetFlagStatus(EVAL_COM1, USART_FLAG_ABRF) == RESET)
  {}  
  
  /* If AutoBaudBate error occurred */
  if (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_ABRE) != RESET)
  {
    /* Turn on LED3 */
    STM_EVAL_LEDOn(LED1);
  }
  else
  {
    /* Turn on LED2 */
    STM_EVAL_LEDOn(LED2);
    
    /* Wait until RXNE flag is set */
    while(USART_GetFlagStatus(EVAL_COM1, USART_FLAG_RXNE) == RESET)
    {}
    
    /* Wait until TXE flag is set */    
    while(USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TXE) == RESET)
    {}
    
    /* Send received character */
    USART_SendData(EVAL_COM1, USART_ReceiveData(EVAL_COM1)); 
    
    /* clear the TE bit (if a transmission is on going or a data is in the TDR, it will be sent before
    efectivelly disabling the transmission) */
    USART_DirectionModeCmd(EVAL_COM1, USART_Mode_Tx, DISABLE);
    
    /* Check the Transfer Complete Flag */
    while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
    {}
  }
  
  /* USART Disable */
  USART_Cmd(EVAL_COM1, DISABLE);
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(EVAL_COM1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

GETCHAR_PROTOTYPE
{
    while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_RXNE) == RESET)
    {}
    return (int)USART_ReceiveData(EVAL_COM1);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
