#ifndef __LED_H
#define __LED_H

#include "stm32f0xx.h"

#define N1_PIN 		                GPIO_Pin_6
#define N1_PORT 		            GPIOC
#define N1_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOC
#define N2_PIN 		                GPIO_Pin_5
#define N2_PORT 		            GPIOC
#define N2_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOC
#define N3_PIN 		                GPIO_Pin_4
#define N3_PORT 		            GPIOC
#define N3_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOC
#define N4_PIN 		                GPIO_Pin_3
#define N4_PORT 		            GPIOC
#define N4_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOC
#define N5_PIN 		                GPIO_Pin_2
#define N5_PORT 		            GPIOC
#define N5_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOC
#define N6_PIN 		                GPIO_Pin_1
#define N6_PORT 		            GPIOC
#define N6_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOC
#define N7_PIN 		                GPIO_Pin_0
#define N7_PORT 		            GPIOC
#define N7_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOC

#define D8_PIN 		                GPIO_Pin_13
#define D8_PORT 		            GPIOC
#define D8_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOC
#define D9_PIN 		                GPIO_Pin_9
#define D9_PORT 		            GPIOB
#define D9_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOB
#define D10_PIN 		            GPIO_Pin_8
#define D10_PORT 		            GPIOB
#define D10_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOB
#define D11_PIN 		            GPIO_Pin_13
#define D11_PORT 		            GPIOB
#define D11_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOB
#define D12_PIN 		            GPIO_Pin_12
#define D12_PORT 		            GPIOB
#define D12_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOB
#define D13_PIN 		            GPIO_Pin_0
#define D13_PORT 		            GPIOB
#define D13_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOB
#define D14_PIN 		            GPIO_Pin_1
#define D14_PORT 		            GPIOB
#define D14_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOB
#define D15_PIN 		            GPIO_Pin_2
#define D15_PORT 		            GPIOB
#define D15_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOB
#define D16_PIN 		            GPIO_Pin_10
#define D16_PORT 		            GPIOB
#define D16_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOB
#define D17_PIN 		            GPIO_Pin_11//空
#define D17_PORT 		            GPIOB
#define D17_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOB
#define D18_PIN 		            GPIO_Pin_7//空，跳线到D27
#define D18_PORT 		            GPIOB
#define D18_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOB
#define D19_PIN 		            GPIO_Pin_15
#define D19_PORT 		            GPIOB
#define D19_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOB
#define D20_PIN 		            GPIO_Pin_7
#define D20_PORT 		            GPIOC
#define D20_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOC
#define D21_PIN 		            GPIO_Pin_8
#define D21_PORT 		            GPIOC
#define D21_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOC
#define D22_PIN 		            GPIO_Pin_4
#define D22_PORT 		            GPIOB
#define D22_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOB
#define D23_PIN 		            GPIO_Pin_5
#define D23_PORT 		            GPIOB
#define D23_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOB
#define D24_PIN 		            GPIO_Pin_6
#define D24_PORT 		            GPIOB
#define D24_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOB
#define D25_PIN 		            GPIO_Pin_8
#define D25_PORT 		            GPIOA
#define D25_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOA
#define D26_PIN 		            GPIO_Pin_9
#define D26_PORT 		            GPIOC
#define D26_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOC
#define D27_PIN 		            GPIO_Pin_7//硬件没有连接，用D18的来连接跳线
#define D27_PORT 		            GPIOB
#define D27_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOB
#define D28_PIN 		            GPIO_Pin_11
#define D28_PORT 		            GPIOC
#define D28_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOC
#define D29_PIN 		            GPIO_Pin_12
#define D29_PORT 		            GPIOC
#define D29_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOC
#define D30_PIN 		            GPIO_Pin_2
#define D30_PORT 		            GPIOD
#define D30_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOD
#define D31_PIN 		            GPIO_Pin_3
#define D31_PORT 		            GPIOB
#define D31_RCC_AHBPeriph_PORT 	    RCC_AHBPeriph_GPIOB

#define N1_H         do{GPIO_SetBits  (N1_PORT, N1_PIN);}while(__LINE__==-1)
#define N1_L         do{GPIO_ResetBits(N1_PORT, N1_PIN);}while(__LINE__==-1)
#define N2_H         do{GPIO_SetBits  (N2_PORT, N2_PIN);}while(__LINE__==-1)
#define N2_L         do{GPIO_ResetBits(N2_PORT, N2_PIN);}while(__LINE__==-1)
#define N3_H         do{GPIO_SetBits  (N3_PORT, N3_PIN);}while(__LINE__==-1)
#define N3_L         do{GPIO_ResetBits(N3_PORT, N3_PIN);}while(__LINE__==-1)
#define N4_H         do{GPIO_SetBits  (N4_PORT, N4_PIN);}while(__LINE__==-1)
#define N4_L         do{GPIO_ResetBits(N4_PORT, N4_PIN);}while(__LINE__==-1)
#define N5_H         do{GPIO_SetBits  (N5_PORT, N5_PIN);}while(__LINE__==-1)
#define N5_L         do{GPIO_ResetBits(N5_PORT, N5_PIN);}while(__LINE__==-1)
#define N6_H         do{GPIO_SetBits  (N6_PORT, N6_PIN);}while(__LINE__==-1)
#define N6_L         do{GPIO_ResetBits(N6_PORT, N6_PIN);}while(__LINE__==-1)
#define N7_H         do{GPIO_SetBits  (N7_PORT, N7_PIN);}while(__LINE__==-1)
#define N7_L         do{GPIO_ResetBits(N7_PORT, N7_PIN);}while(__LINE__==-1)

#define D8_H         do{GPIO_SetBits  (D8_PORT, D8_PIN);}while(__LINE__==-1)
#define D8_L         do{GPIO_ResetBits(D8_PORT, D8_PIN);}while(__LINE__==-1)
#define D9_H         do{GPIO_SetBits  (D9_PORT, D9_PIN);}while(__LINE__==-1)
#define D9_L         do{GPIO_ResetBits(D9_PORT, D9_PIN);}while(__LINE__==-1)
#define D10_H        do{GPIO_SetBits  (D10_PORT, D10_PIN);}while(__LINE__==-1)
#define D10_L        do{GPIO_ResetBits(D10_PORT, D10_PIN);}while(__LINE__==-1)
#define D11_H        do{GPIO_SetBits  (D11_PORT, D11_PIN);}while(__LINE__==-1)
#define D11_L        do{GPIO_ResetBits(D11_PORT, D11_PIN);}while(__LINE__==-1)
#define D12_H        do{GPIO_SetBits  (D12_PORT, D12_PIN);}while(__LINE__==-1)
#define D12_L        do{GPIO_ResetBits(D12_PORT, D12_PIN);}while(__LINE__==-1)
#define D13_H        do{GPIO_SetBits  (D13_PORT, D13_PIN);}while(__LINE__==-1)
#define D13_L        do{GPIO_ResetBits(D13_PORT, D13_PIN);}while(__LINE__==-1)
#define D14_H        do{GPIO_SetBits  (D14_PORT, D14_PIN);}while(__LINE__==-1)
#define D14_L        do{GPIO_ResetBits(D14_PORT, D14_PIN);}while(__LINE__==-1)
#define D15_H        do{GPIO_SetBits  (D15_PORT, D15_PIN);}while(__LINE__==-1)
#define D15_L        do{GPIO_ResetBits(D15_PORT, D15_PIN);}while(__LINE__==-1)
#define D16_H        do{GPIO_SetBits  (D16_PORT, D16_PIN);}while(__LINE__==-1)
#define D16_L        do{GPIO_ResetBits(D16_PORT, D16_PIN);}while(__LINE__==-1)
#define D17_H        do{GPIO_SetBits  (D17_PORT, D17_PIN);}while(__LINE__==-1)
#define D17_L        do{GPIO_ResetBits(D17_PORT, D17_PIN);}while(__LINE__==-1)
#define D18_H        do{GPIO_SetBits  (D18_PORT, D18_PIN);}while(__LINE__==-1)
#define D18_L        do{GPIO_ResetBits(D18_PORT, D18_PIN);}while(__LINE__==-1)
#define D19_H        do{GPIO_SetBits  (D19_PORT, D19_PIN);}while(__LINE__==-1)
#define D19_L        do{GPIO_ResetBits(D19_PORT, D19_PIN);}while(__LINE__==-1)
#define D20_H        do{GPIO_SetBits  (D20_PORT, D20_PIN);}while(__LINE__==-1)
#define D20_L        do{GPIO_ResetBits(D20_PORT, D20_PIN);}while(__LINE__==-1)
#define D21_H        do{GPIO_SetBits  (D21_PORT, D21_PIN);}while(__LINE__==-1)
#define D21_L        do{GPIO_ResetBits(D21_PORT, D21_PIN);}while(__LINE__==-1)
#define D22_H        do{GPIO_SetBits  (D22_PORT, D22_PIN);}while(__LINE__==-1)
#define D22_L        do{GPIO_ResetBits(D22_PORT, D22_PIN);}while(__LINE__==-1)
#define D23_H        do{GPIO_SetBits  (D23_PORT, D23_PIN);}while(__LINE__==-1)
#define D23_L        do{GPIO_ResetBits(D23_PORT, D23_PIN);}while(__LINE__==-1)
#define D24_H        do{GPIO_SetBits  (D24_PORT, D24_PIN);}while(__LINE__==-1)
#define D24_L        do{GPIO_ResetBits(D24_PORT, D24_PIN);}while(__LINE__==-1)
#define D25_H        do{GPIO_SetBits  (D25_PORT, D25_PIN);}while(__LINE__==-1)
#define D25_L        do{GPIO_ResetBits(D25_PORT, D25_PIN);}while(__LINE__==-1)
#define D26_H        do{GPIO_SetBits  (D26_PORT, D26_PIN);}while(__LINE__==-1)
#define D26_L        do{GPIO_ResetBits(D26_PORT, D26_PIN);}while(__LINE__==-1)
#define D27_H        do{GPIO_SetBits  (D27_PORT, D27_PIN);}while(__LINE__==-1)
#define D27_L        do{GPIO_ResetBits(D27_PORT, D27_PIN);}while(__LINE__==-1)
#define D28_H        do{GPIO_SetBits  (D28_PORT, D28_PIN);}while(__LINE__==-1)
#define D28_L        do{GPIO_ResetBits(D28_PORT, D28_PIN);}while(__LINE__==-1)
#define D29_H        do{GPIO_SetBits  (D29_PORT, D29_PIN);}while(__LINE__==-1)
#define D29_L        do{GPIO_ResetBits(D29_PORT, D29_PIN);}while(__LINE__==-1)
#define D30_H        do{GPIO_SetBits  (D30_PORT, D30_PIN);}while(__LINE__==-1)
#define D30_L        do{GPIO_ResetBits(D30_PORT, D30_PIN);}while(__LINE__==-1)
#define D31_H        do{GPIO_SetBits  (D31_PORT, D31_PIN);}while(__LINE__==-1)
#define D31_L        do{GPIO_ResetBits(D31_PORT, D31_PIN);}while(__LINE__==-1)      

#define seg_a       0
#define seg_b       1
#define seg_c       2
#define seg_d       3
#define seg_e       4
#define seg_f       5
#define seg_g       6
#define seg_h       7

void prvShowTask( void *pvParameters );

#endif


