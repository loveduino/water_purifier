#ifndef __GPRS_H
#define __GPRS_H

#include "stm32f0xx.h"
#include <stdbool.h>

#define  GPRS_PWRKEY_PIN                        GPIO_Pin_10
#define  GPRS_PWRKEY_PORT                       GPIOC
#define  GPRS_PWRKEY_RCC                        RCC_AHBPeriph_GPIOC

typedef struct{
    bool is_tcpip_connect;
    bool is_ping_resp;
    bool is_ping_send;
}edp_t;
extern edp_t edp;

void sendATTest(void);
void prvGprsTask( void *pvParameters );
void AppTaskEdpNetCheck();
#endif
