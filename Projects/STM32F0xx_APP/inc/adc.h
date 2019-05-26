#ifndef __ADC_H
#define __ADC_H

#include "stm32f0xx.h"
#include "stm320518_eval.h"

#include "e2prom.h"

#define ADC1_DR_Address    0x40012440

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint16_t  ADC1ConvertedValue;

void prvAdcTask( void *pvParameters );


//PF1
#define TDS_A_H     do{STM_EVAL_LEDOff(RUN_LED);}while(0)
#define TDS_A_L     do{STM_EVAL_LEDOn(RUN_LED);}while(0)

//PC14
#define TDS_B_H     do{E2WP_ENABLE;}while(0)
#define TDS_B_L     do{E2WP_DISABLE;}while(0)


#endif