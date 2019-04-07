/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "adc.h"
#include "main.h"
#include <stdio.h>

/**
  * @brief  ADC1 channel configuration
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  ADC_InitTypeDef     ADC_InitStructure;
  GPIO_InitTypeDef    GPIO_InitStructure;
  
  /* GPIOA Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
   /* ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  /* Configure ADC1 Channel0, Channel1 and channel2 as analog input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* ADC1 DeInit */  
  ADC_DeInit(ADC1);
  
  /* Initialize ADC structure */
  ADC_StructInit(&ADC_InitStructure);
  
  /* Configure the ADC1 in continuous mode withe a resolution equal to 12 bits  */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;/*配置ADC分辨率为12位*/
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; /*开启连续转换*/
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;/*禁止触发检测，使用软件触发*/
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;/*ADC采集数据右对齐*/
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;/*向上扫描*/
  ADC_Init(ADC1, &ADC_InitStructure); 

  /* Convert the ADC1 Channel0, Channel1 and channel2 with 55.5 Cycles as sampling time */ 
  ADC_ChannelConfig(ADC1, ADC_Channel_0 , ADC_SampleTime_55_5Cycles); 
  ADC_ChannelConfig(ADC1, ADC_Channel_1 , ADC_SampleTime_55_5Cycles); 
  ADC_ChannelConfig(ADC1, ADC_Channel_2 , ADC_SampleTime_55_5Cycles); 
  
  /* ADC Calibration */
  ADC_GetCalibrationFactor(ADC1);
  
  /* Enable the ADC peripheral */
  ADC_Cmd(ADC1, ENABLE);     
  
  /* Wait the ADRDY flag */
  //while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)); 
  
  /* ADC1 regular Software Start Conv */ 
  //ADC_StartOfConversion(ADC1);
}

uint16_t ADC_Get_Result(uint32_t ch, uint16_t count)
{
    uint32_t sum = 0;
    ADC_ChannelConfig(ADC1, ch, ADC_SampleTime_55_5Cycles);
    ADC1->CHSELR = ch;
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)){};
    for (uint16_t i = 0; i < count; i++)
    {
        ADC_StartOfConversion(ADC1);
        while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET){};
        sum += ADC_GetConversionValue(ADC1)&0x00000fff;
    }
    return sum / count;
}

float raw_water_voltage = 0.0;
float clean_water_voltage = 0.0;

void prvAdcTask( void *pvParameters )
{
	/* Just to stop compiler warnings. */
	( void ) pvParameters;
    
    /* ADC1 configuration */
    ADC_Config();

    STM_EVAL_LEDInit(RUN_LED);
    STM_EVAL_LEDOff(RUN_LED);
    TDS_A_L;
            
	for( ;; )
    {
        float adc_value = 0;
        static int count = 0;
        
        if (count++ == 2)
        {
            count  = 0;
            TDS_A_L;
            //wait_us(500);
            vTaskDelay( pdMS_TO_TICKS(1) );
            
            extern delay_show_tds_t delay_show_tds;
            
            //在制水前10s是不会更新tds值的。
            if (delay_show_tds.is_make_water == 0)
            {
                raw_water_voltage = adc_value = (uint16_t)((float)ADC_Get_Result(ADC_Channel_1, 10)   * 3300 / 4095/* * 0.4 - 10.2*/);
                if (adc_value < 6)
                    water.raw_water_ppm = 0;
                else
                    water.raw_water_ppm = (uint16_t)(adc_value*adc_value*0.0007+adc_value*0.2505+1.7221);
                
                clean_water_voltage = adc_value = (uint16_t)((float)ADC_Get_Result(ADC_Channel_0, 10)   * 3300 / 4095/* * 0.4 - 10.2*/);
                if (adc_value < 6)
                    water.clean_water_ppm = 0;
                else
                    water.clean_water_ppm = (uint16_t)(adc_value*adc_value*0.0007+adc_value*0.2505+1.7221);
            }
            
            //wait_us(500);
            vTaskDelay( pdMS_TO_TICKS(1) );
            
            TDS_A_H;
            vTaskDelay( pdMS_TO_TICKS(1) );
        }
        
        if (ADC_Get_Result(ADC_Channel_2, 10) > (uint16_t)(1.5/3.3*4095))
        {
            water.is_drop_water_status = 1;
        }
        else
        {
            water.is_drop_water_status = 0;
        }
        extern bool is_send_device_status;
        static uint8_t is_drop_water_status = false;
        if (is_drop_water_status != water.is_drop_water_status)
        {
            is_drop_water_status = water.is_drop_water_status;
            is_send_device_status = true;
        }
        
        vTaskDelay( pdMS_TO_TICKS(1000) );
    }
}
