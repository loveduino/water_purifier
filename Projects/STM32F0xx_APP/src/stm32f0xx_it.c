/**
******************************************************************************
* @file    STM32F0xx_IAP/binary_template/src/stm32f0xx_it.c 
* @author  MCD Application Team
* @version V1.0.0
* @date    29-May-2012
* @brief   Main Interrupt Service Routines.
*          This file provides template for all exceptions handler and 
*          peripherals interrupt service routine.
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
#include "stm32f0xx_it.h"
#include "main.h"

/** @addtogroup STM32F0xx_IAP
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint32_t TimingDelay;
extern __IO uint32_t LsiFreq;
extern __IO uint32_t TIM3Freq = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
* @brief  This function handles NMI exception.
* @param  None
* @retval None
*/
void NMI_Handler(void)
{
}

/**
* @brief  This function handles Hard Fault exception.
* @param  None
* @retval None
*/
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

//必须要移植的代码，放在了port.c里面实现。为了少改动启动文件，注释这里的实现。
///**
//  * @brief  This function handles SVCall exception.
//  * @param  None
//  * @retval None
//  */
//void SVC_Handler(void)
//{
//}

//必须要移植的代码，放在了port.c里面实现。为了少改动启动文件，注释这里的实现。
///**
//  * @brief  This function handles PendSV_Handler exception.
//  * @param  None
//  * @retval None
//  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//必须要移植的代码，放在了port.c里面实现。为了少改动启动文件，注释这里的实现。
///**
//  * @brief  This function handles SysTick Handler.
//  * @param  None
//  * @retval None
//  */
//void SysTick_Handler(void)
//{
//  xPortSysTickHandler(); 
//}

#ifdef LSI_TIM_MEASURE
uint16_t CaptureNumber = 0;
/**
* @brief  This function handles TIM14 global interrupt request.
* @param  None
* @retval None
*/
void TIM14_IRQHandler(void)
{
    static uint16_t IC1ReadValue1 = 0, IC1ReadValue2 = 0;
    uint32_t Capture = 0;

    if (TIM_GetITStatus(TIM14, TIM_IT_CC1) != RESET)
    {    
        if(CaptureNumber == 0)
        {
            /* Get the Input Capture value */
            IC1ReadValue1 = TIM_GetCapture1(TIM14);
        }
        else if(CaptureNumber == 1)
        {
            /* Get the Input Capture value */
            IC1ReadValue2 = TIM_GetCapture1(TIM14); 
            
            /* Capture computation */
            if (IC1ReadValue2 > IC1ReadValue1)
            {
                Capture = (IC1ReadValue2 - IC1ReadValue1); 
            }
            else
            {
                Capture = ((0xFFFF - IC1ReadValue1) + IC1ReadValue2); 
            }
            /* Frequency computation */ 
            LsiFreq = (uint32_t) SystemCoreClock / Capture;
            LsiFreq *= 8;
        }
        
        CaptureNumber++;
        
        /* Clear TIM14 Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(TIM14, TIM_IT_CC1);
    }
}
#endif /* LSI_TIM_MEASURE */

/**
  * @brief  This function handles TIM3 Capture Compare interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{ 
    static uint16_t IC1ReadValue1 = 0, IC1ReadValue2 = 0;
    static uint16_t CaptureNumber = 0;
    uint32_t Capture = 0;
    
    if(TIM_GetITStatus(TIM3, TIM_IT_CC1) == SET) 
    {
        /* Clear TIM3 Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
        if(CaptureNumber == 0)
        {
            /* Get the Input Capture value */
            IC1ReadValue1 = TIM_GetCapture1(TIM3);
            CaptureNumber = 1;
        }
        else if(CaptureNumber == 1)
        {
            /* Get the Input Capture value */
            IC1ReadValue2 = TIM_GetCapture1(TIM3); 
            
            /* Capture computation */
            if (IC1ReadValue2 > IC1ReadValue1)
            {
                Capture = (IC1ReadValue2 - IC1ReadValue1); 
            }
            else if (IC1ReadValue2 < IC1ReadValue1)
            {
                Capture = ((0xFFFF - IC1ReadValue1) + IC1ReadValue2); 
            }
            else
            {
                Capture = 0;
            }
            /* Frequency computation */ 
            TIM3Freq = (uint32_t) SystemCoreClock / Capture;
            CaptureNumber = 0;
            //printf("TIM3Freq = %d\r\n", TIM3Freq);
        }
    }
}

/*******************************************************************************
* Function Name  : EXTI1_IRQHandler
* Description    : This function handles External interrupt Line 1 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI4_15_IRQHandler(void)
{
    if(EXTI_GetITStatus(KEY_BUTTON_EXTI_LINE) != RESET)
    {
        if (STM_EVAL_PBGetState(BUTTON_KEY) == 0x00)
            key_fall_flag = 1;//生成按键按下标志
        /* Clear the EXTI line 15 pending bit */
        EXTI_ClearITPendingBit(KEY_BUTTON_EXTI_LINE);
    }
    
    if(EXTI_GetITStatus(EXTI_Line6) != RESET)//流量计
    {
        /* Toggle LED1 */
        //STM_EVAL_LEDToggle(LED1);
        
        /*
        0	流量模式
        1	时长模式
        */
        if (water.is_test_mode_switch_off == 1)
        {
            water.test_used_flow = 0;
            water.flowmeter_count++;//流量计累加，但是如果恢复出厂的时候，就需要清理0
            water.save.used_flow++;
            
            if (water.save.remain_flow > 0)
            {
                water.save.remain_flow--;
            }
            else
            {
                water.save.remain_flow = 0;
                //water.save.is_bind = 0;
                //water.device_status = 3;//欠费
            }
        }
        else
        {
            water.test_used_flow++;
        }
        
        /* Clear the EXTI line 8 pending bit */
        EXTI_ClearITPendingBit(EXTI_Line6);
    }
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                            */
/******************************************************************************/

/**
* @brief  This function handles PPP interrupt request.
* @param  None
* @retval None
*/
/*void PPP_IRQHandler(void)
{
}*/

/**
* @}
*/ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
