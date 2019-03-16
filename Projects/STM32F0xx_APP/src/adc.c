/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "adc.h"
#include "main.h"
#include <stdio.h>

__IO uint16_t  ADC1ConvertedValue = 0;
__IO uint16_t RegularConvData_Tab[5];

/**
  * @brief  ADC1 channel configuration
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  ADC_InitTypeDef     ADC_InitStructure;
  GPIO_InitTypeDef    GPIO_InitStructure;
  /* ADC1 DeInit */  
  ADC_DeInit(ADC1);
  
  /* GPIOA Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
   /* ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  /* Configure ADC1 Channel0, Channel1 and channel2 as analog input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Initialize ADC structure */
  ADC_StructInit(&ADC_InitStructure);
  
  /* Configure the ADC1 in continuous mode withe a resolution equal to 12 bits  */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;/*配置ADC分辨率为12位*/
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; /*开启连续转换*/
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;/*禁止触发检测，使用软件触发*/
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;/*ADC采集数据右对齐*/
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;/*向上扫描*/
  ADC_Init(ADC1, &ADC_InitStructure); 

  /* Convert the ADC1 Channel0, Channel1 and channel2 with 55.5 Cycles as sampling time */ 
  ADC_ChannelConfig(ADC1, ADC_Channel_0 , ADC_SampleTime_55_5Cycles); 
  ADC_ChannelConfig(ADC1, ADC_Channel_1 , ADC_SampleTime_55_5Cycles); 
  ADC_ChannelConfig(ADC1, ADC_Channel_2 , ADC_SampleTime_55_5Cycles); 
  
  /* Convert the ADC1 temperature sensor  with 55.5 Cycles as sampling time */ 
  ADC_ChannelConfig(ADC1, ADC_Channel_TempSensor , ADC_SampleTime_55_5Cycles);  
  ADC_TempSensorCmd(ENABLE);
  
  /* Convert the ADC1 Vref  with 55.5 Cycles as sampling time */ 
  ADC_ChannelConfig(ADC1, ADC_Channel_Vrefint , ADC_SampleTime_55_5Cycles); 
  ADC_VrefintCmd(ENABLE);
  
  /* ADC Calibration */
  ADC_GetCalibrationFactor(ADC1);
  
  /* ADC DMA request in circular mode */
  ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
  
  /* Enable ADC_DMA */
  ADC_DMACmd(ADC1, ENABLE);  
  
  /* Enable the ADC peripheral */
  ADC_Cmd(ADC1, ENABLE);     
  
  /* Wait the ADRDY flag */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)); 
  
  /* ADC1 regular Software Start Conv */ 
  ADC_StartOfConversion(ADC1);
}

/**
  * @brief  DMA channel1 configuration
  * @param  None
  * @retval None
  */
static void DMA_Config(void)
{
  DMA_InitTypeDef   DMA_InitStructure;
  /* DMA1 clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
  
  /* DMA1 Channel1 Config */
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RegularConvData_Tab;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 5;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  /* DMA1 Channel1 enable */
  DMA_Cmd(DMA1_Channel1, ENABLE);
  
}


/**
  * @brief  Display ADC converted value on LCD
  * @param  None
  * @retval None
  */
void Display(void)
{
  float v=0;
  uint8_t text[50],index;
  
  for(index=0;index<5;index++)
  {

    v=((RegularConvData_Tab[index]* 3300) / 0xFFF) / 1000.0;
    
    if (index == 0)
    {
      sprintf((char*)text," Pot (PA0)   = %f V   ",v);
    }
    else if (index == 1)
    {
      sprintf((char*)text," Pot (PA1)   = %f V   ",v);
    }
    else if (index == 2)
    {
      sprintf((char*)text," Pot (PA2)   = %f V   ",v);
    }
    else if (index == 3)
    {
      sprintf((char*)text," V(Temp)     = %f V   ",v);
      
/* Temperature sensor calibration value address */
#define TEMP30_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FFFF7B8))
#define VDD_CALIB ((uint32_t) (3300))
#define VDD_APPLI ((uint32_t) (3000))
#define AVG_SLOPE ((uint32_t) (5336)) //AVG_SLOPE in ADC conversion step (@3.3V)/°C multiplied by 1000 for precision on the division
      
      int32_t temperature; /* will contain the temperature in degrees Celsius */
      temperature = ((uint32_t) *TEMP30_CAL_ADDR - ((uint32_t) (RegularConvData_Tab[index]) * VDD_APPLI / VDD_CALIB)) * 1000;
      temperature = (temperature / AVG_SLOPE) + 30;
      printf("temp = %d\r\n", temperature);
    }
    else if (index == 4)
    {
      sprintf((char*)text," V(ref int)  = %f V   ",v);
    }
    printf((char*)text);
    printf("\r\n");
  } 
}


uint8_t g_PorbeAoutputLevel = 0;

static void TIM_Config(void)
{
    uint16_t PrescalerValue = 0;
    
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* Enable the TIM3 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
    /* -----------------------------------------------------------------------
    TIM3 Configuration: Output Compare Timing Mode:
    
    In this example TIM3 input clock (TIM3CLK) is set to APB1 clock (PCLK1),  
      => TIM3CLK = PCLK1 = SystemCoreClock = 48 MHz
          
    To get TIM3 counter clock at 6 MHz, the prescaler is computed as follows:
       Prescaler = (TIM3CLK / TIM3 counter clock) - 1
       Prescaler = (PCLK1 /6 MHz) - 1
                                              
    CC1 update rate = TIM3 counter clock / CCR1_Val = 146.48 Hz
    ==> Toggling frequency = 73.24 Hz
    
    C2 update rate = TIM3 counter clock / CCR2_Val = 219.7 Hz
    ==> Toggling frequency = 109.8 Hz
    
    CC3 update rate = TIM3 counter clock / CCR3_Val = 439.4 Hz
    ==> Toggling frequency = 219.7 Hz
    
    CC4 update rate = TIM3 counter clock / CCR4_Val = 878.9 Hz
    ==> Toggling frequency = 439.4 Hz

    Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f0xx.c file.
     Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
     function to update SystemCoreClock variable value. Otherwise, any configuration
     based on this variable will be incorrect.    
  ----------------------------------------------------------------------- */   


  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) (SystemCoreClock  / 1000000) - 1;

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 680;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* Prescaler configuration */
  TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);

  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  
  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);
}

void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        if (g_PorbeAoutputLevel == 1)
        {
            g_PorbeAoutputLevel = 0;
            TDS_A_L;
            TDS_B_H;
        }
        else
        {
            g_PorbeAoutputLevel = 1;
            TDS_A_H;
            TDS_B_L;
        }
    }
}


void prvAdcTask( void *pvParameters )
{
	/* Just to stop compiler warnings. */
	( void ) pvParameters;
    
    /* ADC1 configuration */
    ADC_Config();
    
    /* DMA configuration */
    DMA_Config();
    
    I2C_EE_Config();
    g_PorbeAoutputLevel = 0;
    TDS_A_L;
    TDS_B_H;
    
    TIM_Config();
            
	for( ;; )
    {
        static int count = 0;
        
        if (count++ == 2)
        {
            count  = 0;
            
            while(g_PorbeAoutputLevel == 0);                /* wait to output hige level */
            wait_us(5);
            
            /* Test DMA1 TC flag */
            while((DMA_GetFlagStatus(DMA1_FLAG_TC1)) == RESET );
            /* Clear DMA TC flag */
            DMA_ClearFlag(DMA1_FLAG_TC1);
            
            water.raw_water_ppm = (uint16_t)((float)RegularConvData_Tab[1]   * 3300 / 4095/* * 0.4 - 10.2*/);
//            if (water.raw_water_ppm > 999)
//                water.raw_water_ppm = 0;
            
            water.clean_water_ppm = (uint16_t)((float)RegularConvData_Tab[0]   * 3300 / 4095/* * 0.4 - 10.2*/);
//            if (water.clean_water_ppm > 999)
//                water.clean_water_ppm = 0;
        }
        
        /* Test DMA1 TC flag */
        while((DMA_GetFlagStatus(DMA1_FLAG_TC1)) == RESET );
        /* Clear DMA TC flag */
        DMA_ClearFlag(DMA1_FLAG_TC1);
        
        if (RegularConvData_Tab[2] > (uint16_t)(1.5/3.3*4095))
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
        
        /* Display converted data on the LCD */
        //Display();
        vTaskDelay( pdMS_TO_TICKS(1000) );
    }
}
