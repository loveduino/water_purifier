#ifndef _E2PROM
#define _E2PROM

#include "stm32f0xx.h"

#define E2WP_PIN 		            GPIO_Pin_14
#define E2WP_PORT 		            GPIOC
#define RCC_APB2Periph_E2WP_PORT 	RCC_AHBPeriph_GPIOC

//When the Write Protect pin is connected to VCC, the write protection feature is enabled
#define E2WP_ENABLE                 do{GPIO_SetBits(E2WP_PORT, E2WP_PIN);}while(__LINE__==-1)
// The Write Protect pin allows normal Read/Write operations when connected to ground (GND). 
#define E2WP_DISABLE                do{GPIO_ResetBits(E2WP_PORT, E2WP_PIN);}while(__LINE__==-1)

#define E2PROM_SLAVE_ADDR	        0xA0	// sensor I2C address + write bit

void I2C_EE_BufferWrite(uint8_t WriteAddr, const uint8_t* const pBuffer, uint16_t NumByteToWrite);
void I2C_EE_BufferRead(uint8_t ReadAddr, uint8_t* const pBuffer, uint16_t NumByteToRead);
int I2C_EE_BufferWriteSafe(uint8_t WriteAddr, const uint8_t* const pBuffer, uint16_t NumByteToWrite);



#endif
