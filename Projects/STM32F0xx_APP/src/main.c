/*
 * FreeRTOS Kernel V10.0.0
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software. If you wish to use our Amazon
 * FreeRTOS name, please do so in a fair use way that does not cause confusion.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */


/* Standard includes. */
#include "string.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/* Demo application include. */
#include "main.h"
#include "adc.h"
#include "crypto.h"
#include "i2c_soft.h"
#include "rtc.h"
#include "e2prom.h"
#include "led.h"
#include "menu.h"

#include "protocol.h"
#include "serial.h"
#include "common.h"

/** @addtogroup STM32F0xx_IAP
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
    

/* Private variables ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if APP_A | APP_B
#if   (defined ( __CC_ARM ))
__IO uint32_t VectorTable[48] __attribute__((at(0x20000000)));
#elif (defined (__ICCARM__))
#pragma location = 0x20000000
__no_init __IO uint32_t VectorTable[48];
#elif defined   (  __GNUC__  )
__IO uint32_t VectorTable[48] __attribute__((section(".RAMVectorTable")));
#elif defined ( __TASKING__ )
__IO uint32_t VectorTable[48] __at(0x20000000);
#endif
#endif


uint32_t make_water_time_to_bush = 0;//用于记录制水时长：定时强冲
uint32_t make_water_time_to_check = 0;//用于记录制水时长：检修

KeyClick_t KeyClick;
bool key_fall_flag = false;

/* Private function prototypes -----------------------------------------------*/
static void Main_task(void * pvParameters);
static void prvLedTask(void * pvParameters);
static void prvDataTimeTask( void *pvParameters );
static void prvDeviceMakeStatusTask(void * pvParameters);
static void prvDeviceMakeWaterTask(void * pvParameters);
static void prvDeviceSendStatusTask(void * pvParameters);
static void IWDG_Config(void);
//static void NVIC_Config(void);
static void TIM_CaptureConfig(void);
static void RCC_Configuration(void);
void BEEP_Config(void);
static void prvSetupHardware( void );
void EN1_Config(void);
void EN2_Config(void);
static void EXTI4_15_Config(void);

void EDP_SendPacketStatus(void);//0x0C,//工作状态同步


/* Private functions ---------------------------------------------------------*/
/*
可以在项目设置中设置时间使用的变量类型计数范围
_DLIB_TIME_USES_LONG
_DLIB_TIME_USES_64
*/
clock_t clk_count = 0;//系统上电后运行时间计数，如果是32位，1ms周期下最大计时49.7天
time_t time_dat;//设置localtime相对于公元1970年1月1日0时0分0秒算起至今的UTC时间所经过的秒数

clock_t clock (void) {
    return (clk_count);
}
/*-----------------------------------------------------------*/

#if _DLIB_TIME_USES_64
time_t __time64 (time_t *p) {
    return time_dat;
}
#else
time_t __time32 (time_t *p) {
    return time_dat;
}
#endif
/*-----------------------------------------------------------*/

const char * __getzone(void)//没有生效
{
    return ": GMT+8:GMT+9:+0800";
}

delay_show_tds_t delay_show_tds;

void WorkMode(WorkMode_t work_mode)
{
    static WorkMode_t work_mode_last;
    
    if (work_mode_last != work_mode)
    {
        work_mode_last = work_mode;
        if (WORK_MAKE == work_mode)
        {
            delay_show_tds.is_make_water = 1;
            delay_show_tds.make_water_count = 0;
        }
        else
        {
            delay_show_tds.is_make_water = 0;
            delay_show_tds.make_water_count = 0;
        }
    }
    switch (work_mode)
    {
    case WORK_STOP:
        STM_EVAL_LEDOff(BUSH_LED);
        EN1_L;
        EN2_L;
        break;
    case WORK_MAKE:
        STM_EVAL_LEDOff(BUSH_LED);
        EN1_H;
        EN2_L;
        break;
    case WORK_BUSH:
        STM_EVAL_LEDOn(BUSH_LED);
        make_water_time_to_bush = 0;//如果冲洗过，就重新计算这个累加制水时间，清零
        EN1_H;
        EN2_H;
        break;
    default:STM_EVAL_LEDOff(BUSH_LED);break;
    }
}

#include <string.h>

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
    
#if APP_A
    /* Relocate by software the vector table to the internal SRAM at 0x20000000 ***/  
    /* Copy the vector table from the Flash (mapped at the base of the application
    load address 0x08010000) to the base address of the SRAM at 0x20000000. */
    for (uint32_t i = 0; i < 48; i++)
    {
        VectorTable[i] = *(__IO uint32_t*)(0x08010000 + (i<<2));
    }

    /* Enable the SYSCFG peripheral clock*/
    //RCC_APB2PeriphResetCmd(RCC_APB2Periph_SYSCFG, ENABLE);//在此处应该将 RCC_APB2PeriphResetCmd 函数替换为RCC_APB2PeriphClockCmd 函数。
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    /* Remap SRAM at 0x00000000 */
    SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
#elif APP_B
    /* Relocate by software the vector table to the internal SRAM at 0x20000000 ***/  
    /* Copy the vector table from the Flash (mapped at the base of the application
    load address 0x08010000) to the base address of the SRAM at 0x20000000. */
    for (uint32_t i = 0; i < 48; i++)
    {
        VectorTable[i] = *(__IO uint32_t*)(0x08020000 + (i<<2));
    }

    /* Enable the SYSCFG peripheral clock*/
    //RCC_APB2PeriphResetCmd(RCC_APB2Periph_SYSCFG, ENABLE);//在此处应该将 RCC_APB2PeriphResetCmd 函数替换为RCC_APB2PeriphClockCmd 函数。
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    /* Remap SRAM at 0x00000000 */
    SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
#endif
    
    RCC_Configuration();
    
#if BOOTLOADER
    
    STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_GPIO);
    if (STM_EVAL_PBGetState(BUTTON_KEY) != 0x00)
    {
        char app_run[10] = {0};
        
        I2C_Config();
        
        I2C_EE_BufferRead(0+sizeof(save_data_default)+sizeof(g_data_format.iccid)+APP_O_OFFSET_ADDR, (uint8_t *)&app_run[0], 10);
        
        //APP - A
        /* Test if user code is programmed starting from address "APPLICATION_ADDRESS" */
        //if ((((*(__IO uint32_t*)0x08010000) & 0x2FFE0000 ) != 0x20000000) && (NULL != strstr(app_run, "app_a_on")))
        if  (NULL != strstr(app_run, "app_a_on"))
        {
            extern pFunction Jump_To_Application;
            extern uint32_t JumpAddress;

            /* Jump to user application */
            JumpAddress = *(__IO uint32_t*) (0x08010000 + 4);
            Jump_To_Application = (pFunction) JumpAddress;
            
            /* Initialize user application's Stack Pointer */
            __set_MSP(*(__IO uint32_t*) 0x08010000);
            
            /* Jump to application */
            Jump_To_Application();
        }
        //APP - B
        /* Test if user code is programmed starting from address "APPLICATION_ADDRESS" */
        //else if ((((*(__IO uint32_t*)0x08020000) & 0x2FFE0000 ) == 0x20000000) && (NULL != strstr(app_run, "app_b_on")))
        else if  (NULL != strstr(app_run, "app_b_on"))
        {
            extern pFunction Jump_To_Application;
            extern uint32_t JumpAddress;

            /* Jump to user application */
            JumpAddress = *(__IO uint32_t*) (0x08020000 + 4);
            Jump_To_Application = (pFunction) JumpAddress;
            
            /* Initialize user application's Stack Pointer */
            __set_MSP(*(__IO uint32_t*) 0x08020000);
            
            /* Jump to application */
            Jump_To_Application();
        }
        else //bootloader
        {
            
        }
    }
#endif
    
    IWDG_Config();
    /* Reload IWDG counter */
    IWDG_ReloadCounter();
    
    /***************** Add your application code here   ***************************/
    
    /* Init task */
    if( pdPASS != xTaskCreate(Main_task, (char *) "Main", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 1, NULL) )
    {
        while (1);
    }
    else
    {
        /* Now all the tasks have been started - start the scheduler. */
        vTaskStartScheduler(  );
    }
    
    /* We should never get here as control is now taken by the scheduler */
    for( ;; );
}



bool is_beep_on = false;


/**
  * @brief  Main task
  * @param  pvParameters not used
  * @retval None
  */
void Main_task(void * pvParameters)
{
	/* Just to stop compiler warnings. */
	( void ) pvParameters;
    
    /*Initialize Leds */ 
    prvSetupHardware();
    
    xSerialPortInit( ser115200, 100, 1000 );
    
#if BOOTLOADER
    SerialPutString("\r\n= BOOTLOADER  =");
#elif APP_A
    SerialPutString("\r\n= APP_A       =");
#elif APP_B
    SerialPutString("\r\n= APP_B       =");
#endif
    
    //运行到这里的时候，清空下以后的app用哪个
#if BOOTLOADER
    const char app_run[10] = " ";
    I2C_EE_BufferWrite(0+sizeof(save_data_default)+sizeof(g_data_format.iccid)+APP_O_OFFSET_ADDR, (uint8_t *)app_run, 10);
#endif
    
    I2C_EE_BufferRead(0+sizeof(save_data_default), (uint8_t *)&g_data_format.iccid[0], sizeof(g_data_format.iccid));
    
    I2C_EE_BufferRead(0, (uint8_t *)&water.save, sizeof(save_data_default));
    if (save_data_default.e2prom_init != water.save.e2prom_init)
    {
        memcpy(&water.save, &save_data_default, sizeof(save_data_default));
        //set e2prom to default values.
        I2C_EE_BufferWriteSafe(0, (uint8_t *)&save_data_default, sizeof(save_data_default));
    }
    
    time_t seconds = PCF8563_ReadTime();
    if (seconds < 1514764800)//北京时间2018年1月1日8时0分0秒
    {
        //北京时间2018年1月1日8时0分0秒对应的UTC时间戳
        PCF8563_SetTime(1514764800);
        //seconds = PCF8563_ReadTime();
        seconds = 1514764800;
    }
    time_dat = seconds;//修改系统时间
    
    xTaskCreate( prvDataTimeTask, (char *) "DATA TIME", configMINIMAL_STACK_SIZE*2, NULL, tskIDLE_PRIORITY + 8, NULL );

    xTaskCreate( prvLedTask, (char *) "LED", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL );
    
    xTaskCreate( prvShowTask, (char *) "SHOW", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 7, NULL );
    
    xTaskCreate( prvDeviceMakeStatusTask, (char *) "Device Status", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 5, NULL );//制水状态检查
    
    xTaskCreate( prvDeviceMakeWaterTask, (char *) "Device Make", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 5, NULL );//是否需要制水检查
    
    xTaskCreate( prvAdcTask, (char *) "ADC", 200, NULL, tskIDLE_PRIORITY + 4, NULL );
    
    xTaskCreate( prvGprsTask, (char *) "GPRS", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL );
    
    xTaskCreate( prvDeviceSendStatusTask, (char *) "prvDeviceSendStatusTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL );
    
    for( ;; )
    {
        vTaskDelay(200);
        if (is_beep_on == true)
        {
            BEEP_ON;
            vTaskDelay(200);
            BEEP_OFF;
            is_beep_on = false;
        }
    }
}

TimerHandle_t xForceFlushTimerHandler = NULL;

//强冲洗按钮的使用，用户按下强冲按钮则进行18秒的强制冲洗
void vForceFlushTimerCallback( TimerHandle_t pxTimer )
{
    // Optionally do something if the pxTimer parameter is NULL.
    configASSERT( pxTimer );
    
    water.io_high_status = -1;//记录IO1状态,高压开关状态
    water.io_high_last_status = -1;
    water.io_low_status = -1;//记录IO2状态,低压开关状态
    water.io_low_last_status = -1;
    
    water.is_bushing_now = 0;
    is_beep_on = true;
}

TimerHandle_t xTestModeTimerHandler = NULL;

//测试状态为20分钟，20分钟之后自动回复到原状态。
void vTestModeTimerCallback( TimerHandle_t pxTimer )
{
    // Optionally do something if the pxTimer parameter is NULL.
    configASSERT( pxTimer );
    
    //printf("LONG\r\n");
    water.is_test_mode_switch_off = 1;
    is_beep_on = true;
}

static void prvLedTask(void * pvParameters)
{
    for( ;; )
    {
        IWDG_ReloadCounter();
        {
            static int count_30s = 0;
            if (water.is_overhaul_beep == 1)
            {
                
                static bool is_true = false;
                is_true = !is_true;
                if (is_true)
                    BEEP_ON;
                else
                    BEEP_OFF;
                
                if (count_30s++ >= 30)//報警15s蜂鸣器响
                {
                    count_30s = 0;
                    water.is_overhaul_beep = 0;
                    BEEP_OFF;
                }
            }
            else
                count_30s = 0;
        }
        
#if BOOTLOADER
        //STM_EVAL_LEDToggle(RUN_LED);
        vTaskDelay(500);
#elif APP_A
        //STM_EVAL_LEDOn(RUN_LED);
        vTaskDelay(100);
        //STM_EVAL_LEDOff(RUN_LED);
        vTaskDelay(400);
#elif APP_B
        //STM_EVAL_LEDOn(RUN_LED);
        vTaskDelay(400);
        //STM_EVAL_LEDOff(RUN_LED);
        vTaskDelay(100);
#endif
    }
}

bool is_send_device_status = false;

static void prvDeviceSendStatusTask(void * pvParameters)
{
	/* Just to stop compiler warnings. */
	( void ) pvParameters;
    
    for( ;; )
    {
        if (is_send_device_status == true)
        {
            
            vTaskDelay(2000);
            EDP_SendPacketStatus();
            is_send_device_status = false;
        }
        vTaskDelay(100);
    }
}


static void prvDeviceMakeStatusTask(void * pvParameters)
{
	/* Just to stop compiler warnings. */
	( void ) pvParameters;
    
    TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS( 1000 );
    /* The xLastWakeTime variable needs to be initialized with the current tick
    count. Note that this is the only time the variable is written to explicitly.
    After this assignment, xLastWakeTime is updated automatically internally within
    vTaskDelayUntil(). */
    xLastWakeTime = xTaskGetTickCount();
    
    for( ;; )
    {
        if (water.is_make_water_status == 1)//3	正常制水
        {
            make_water_time_to_check++;
            make_water_time_to_bush++;
            
            //累计制水2小时（默认值数据后台可以进行设置），中间无冲洗，则自动冲洗15秒
            if (make_water_time_to_bush >= water.save.period_flush_time) 
            {
                make_water_time_to_bush = 0;
                water.is_bushing_now = 1;
                if( xTimerStart( xForceFlushTimerHandler, pdMS_TO_TICKS(100) ) == pdPASS )
                {
                }
            }
            
            //连续8小时（默认值，后台可以进行设置）制水，中间无水满，说明净水机出现故障，进入检修程序，提示用户联系售后服务，蜂鸣器急促鸣叫30次后停止报警。
            if (make_water_time_to_check >= water.save.maintenance_time)
            {
                make_water_time_to_check = 0;
                water.is_overhaul_status = 1;
                water.is_overhaul_beep = 1;
                is_send_device_status = true;
            }
        }
        
        /* This task should execute every 50 milliseconds. Time is measured
        in ticks. The pdMS_TO_TICKS macro is used to convert milliseconds
        into ticks. xLastWakeTime is automatically updated within vTaskDelayUntil()
        so is not explicitly updated by the task. */
        vTaskDelayUntil( &xLastWakeTime, xPeriod );       
    }
}
    
static void prvDeviceMakeWaterTask(void * pvParameters)
{
    EN1_Config();
    EN2_Config();
    
    EN1_L;
    EN2_L;
    
    /* TIM Configuration */
    //TIM_CaptureConfig();
    //使用外部中断来检测脉冲个数
    EXTI4_15_Config();
    
    STM_EVAL_PBInit(BUTTON_IO1, BUTTON_MODE_GPIO);
    STM_EVAL_PBInit(BUTTON_IO2, BUTTON_MODE_GPIO);
    
    xForceFlushTimerHandler = xTimerCreate(    "Force Flush Timer",   // Just a text name, not used by the kernel.
                                 ( pdMS_TO_TICKS(water.save.force_flush_time*1000) ),   // The timer period in ticks.
                                 pdFALSE,        // The timers will auto-reload themselves when they expire.
                                 ( void * ) NULL, // Assign each timer a unique id equal to its array index.
                                 vForceFlushTimerCallback); // Each timer calls the same callback when it expires.
    
    xTestModeTimerHandler = xTimerCreate(    "Test Mode Timer",   // Just a text name, not used by the kernel.
                                 ( pdMS_TO_TICKS(20*60*1000) ),   // The timer period in ticks.
                                 pdFALSE,        // The timers will auto-reload themselves when they expire.
                                 ( void * ) NULL, // Assign each timer a unique id equal to its array index.
                                 vTestModeTimerCallback); // Each timer calls the same callback when it expires.
    
    if (water.save.is_bind == 1 )
    {
        if( xTimerStart( xForceFlushTimerHandler, pdMS_TO_TICKS(100) ) == pdPASS )
        {
            //强制冲洗
            water.is_bushing_now = 1;
        }
    }
    
#if BOOTLOADER
    BEEP_ON;vTaskDelay(100);
    BEEP_OFF;vTaskDelay(100);
    BEEP_ON;vTaskDelay(100);
    BEEP_OFF;vTaskDelay(100);
    BEEP_ON;vTaskDelay(100);
    BEEP_OFF;vTaskDelay(100);
    BEEP_ON;vTaskDelay(100);
    BEEP_OFF;vTaskDelay(100);
    BEEP_ON;vTaskDelay(100);
    BEEP_OFF;vTaskDelay(100);
#elif APP_A
    BEEP_ON;vTaskDelay(300);
    BEEP_OFF;vTaskDelay(700);
#elif APP_B
    BEEP_ON;vTaskDelay(300);
    BEEP_OFF;vTaskDelay(200);
    BEEP_ON;vTaskDelay(300);
    BEEP_OFF;vTaskDelay(200);
#endif
    
    for (;;)
    {
        if ((water.is_overhaul_status != 1) && (water.is_drop_water_status != 1) && (water.is_power_on_status != 0) && (water.is_shortage_water_status != 1))
        {
            if (!water.is_bushing_now)
            {
                STM_EVAL_LEDOff(BUSH_LED);
                if (((1 == water.save.is_bind) && ((water.device_status == 0) || (water.device_status == 4)) ) || (0 == water.is_test_mode_switch_off))//已绑定或者咋测试模式下
                {
                    if (water.is_make_water_status == 1)
                    {
                        WorkMode(WORK_MAKE);
                    }
                    else
                    {
                        WorkMode(WORK_STOP);
                    }
                }
                else
                {
                    water.is_make_water_status = 0;
                    WorkMode(WORK_STOP);
                }
            }
            else
            {
                //強冲
                water.is_make_water_status = 0;
                WorkMode(WORK_BUSH);
            }
        }
        else
        {
            water.is_bushing_now = 0;
            water.is_make_water_status = 0;
            WorkMode(WORK_STOP);
            //关闭软件定时器，如果在运行的话
            if( xTimerIsTimerActive( xForceFlushTimerHandler ) != pdFALSE )
            {
                xTimerStop( xForceFlushTimerHandler, 100 );
            }
        }
        
        if (water.is_drop_water_status != 1)
        {
            water.io_low_status = STM_EVAL_PBGetState(BUTTON_IO2);////低压开关             =1，自来水管故障水压异常，    =0自来水管水压正常
            
            static bool shortage_water_exit = false;
            
            if (water.io_low_last_status != water.io_low_status)
            {
                water.io_low_last_status = water.io_low_status;
                if (water.io_low_status == 1)//BUTTON_IO2闭合，=1，自来水管故障水压异常
                {
                    //自来水管故障水压异常
                    water.is_shortage_water_status = 1;
                    water.is_make_water_status = 0;//未制水
                    is_send_device_status = true;
                    shortage_water_exit = true;
                }
                else
                {
                    water.is_shortage_water_status = 0;
                    is_send_device_status = true;
                    if (shortage_water_exit == true)
                    {
                        shortage_water_exit = false;
                        extern TimerHandle_t xForceFlushTimerHandler;
                        //從缺水模式退出后冲刷一次
                        if( xTimerStart( xForceFlushTimerHandler, pdMS_TO_TICKS(100) ) == pdPASS )
                        {
                            water.is_bushing_now = 1;
                        }
                    }
                }
            }
            
            if (water.is_shortage_water_status == 0)
            {
                //=1自来水管水压正常
                water.io_high_status = (bool)STM_EVAL_PBGetState(BUTTON_IO1);
                if (water.io_high_last_status != water.io_high_status)
                {
                    water.io_high_last_status = water.io_high_status;
                    if (water.io_high_status == 0)
                    {
                        //需要制水
                        water.is_make_water_status = 1;//正常制水（）
                        water.is_full_water_status = 0;//未水满
                        is_send_device_status = true;
                    }
                    else//水压高，已经制满水，开路
                    {
                        //制水完毕
                        water.is_make_water_status = 0;//未制水
                        water.is_full_water_status = 1;//水满
                        is_send_device_status = true;
                        make_water_time_to_check = 0;
                    }
                }
            }
            else
            {
                water.io_high_last_status = water.io_high_status = -1;
            }
        }
        else
        {
            water.io_high_status = -1;//记录IO1状态,高压开关状态
            water.io_high_last_status = -1;
            water.io_low_status = -1;//记录IO2状态,低压开关状态
            water.io_low_last_status = -1;
        }
        
        static uint32_t count_5s = 0;
        static uint32_t flowmeter_count = 0;
        static bool is_change = false;//正在改变中
        
        //如果正在流量计正在运转，标记将要记录数据，同时复位5s计数变量，重新开始计数。
        //当流量计通知转动后，5s后将会上传本次水量信息
        if (water.flowmeter_count != flowmeter_count)
        {
            flowmeter_count = water.flowmeter_count;
            is_change = true;
            count_5s = 0;
        }
        
        if (is_change == true)
        {
            count_5s++;
            if (count_5s >= 50)//流量计停止转动5s后做存储和上传动作
            {
                is_change = false;
                I2C_EE_BufferWriteSafe(0, (uint8_t *)&water.save, sizeof(save_data_default));//累计记录流量计转动的次数
                
                static uint32_t __flowmeter_count = 0; 
                
                void EDP_SendPacketWaterSync(uint32_t water);
                EDP_SendPacketWaterSync((uint32_t)((water.flowmeter_count - __flowmeter_count) * 0.917 + 0.5));//发送本次的流量数据
                __flowmeter_count = water.flowmeter_count;
                I2C_EE_BufferWriteSafe(0, (uint8_t *)&water.save, sizeof(save_data_default));
                extern bool is_send_device_status;
                is_send_device_status = true;
            }
        }
        vTaskDelay( pdMS_TO_TICKS(100) );
    }
}




static void prvDataTimeTask( void *pvParameters )
{
	/* Just to stop compiler warnings. */
	( void ) pvParameters;
    
    TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS( 1000 );
    /* The xLastWakeTime variable needs to be initialized with the current tick
    count. Note that this is the only time the variable is written to explicitly.
    After this assignment, xLastWakeTime is updated automatically internally within
    vTaskDelayUntil(). */
    xLastWakeTime = xTaskGetTickCount();
    
	for( ;; )
	{
        
        if (delay_show_tds.is_make_water == 1)
        {
            delay_show_tds.make_water_count++;
            if (delay_show_tds.make_water_count >= 60)
            {
                delay_show_tds.make_water_count = 0;
                delay_show_tds.is_make_water = 0;
            }
        }
        
        
//        static int i = 0;
//        if (i++ >= 3600)//沒小時从时钟芯片同步一次系统时钟
//        {
//            i = 0;
//            /* Perform the periodic actions here. */
//            time_t rtc = PCF8563_ReadTime();
//            time_dat = rtc;//修改系统时间
//            //printf("当前RTC本地时间为: %s\r\n", asctime(localtime(&rtc)));
//        }
        
        time_t now;
        now = time(NULL);
        //printf("当前系统本地时间为: %s\r\n", asctime(localtime(&now)));
        
        if (now > water.save.used_days_timestamp)
            water.used_days = (now - water.save.used_days_timestamp) / (3600 * 24);
        else
            water.used_days = 0;
        
        if (now < water.save.remain_days_timestamp)
            water.remain_days = (water.save.remain_days_timestamp - now) / (3600 * 24);
        else
            water.remain_days = 0;
            
        //使用天数 = （现在的时间搓-绑定的时间搓）/ (每天的一共时间搓)
        uint16_t filter_used_value_1 = (now - water.save.filter_used_timestamp_1 ) / (3600 * 24);
        if (water.save.filter_max_value_1 > filter_used_value_1)
            water.filter_remain_value_1 = water.save.filter_max_value_1 - filter_used_value_1;
        else
            water.filter_remain_value_1 = 0;
        
        uint16_t filter_used_value_2 = (now - water.save.filter_used_timestamp_2 ) / (3600 * 24);
        if (water.save.filter_max_value_2 > filter_used_value_2)
            water.filter_remain_value_2 = water.save.filter_max_value_2 - filter_used_value_2;
        else
            water.filter_remain_value_2 = 0;
        
        uint16_t filter_used_value_3 = (now - water.save.filter_used_timestamp_3 ) / (3600 * 24);
        if (water.save.filter_max_value_3 > filter_used_value_3)
            water.filter_remain_value_3 = water.save.filter_max_value_3 - filter_used_value_3;
        else
            water.filter_remain_value_3 = 0;
        
        uint16_t filter_used_value_4 = (now - water.save.filter_used_timestamp_4 ) / (3600 * 24);
        if (water.save.filter_max_value_4 > filter_used_value_4)
            water.filter_remain_value_4 = water.save.filter_max_value_4 - filter_used_value_4;
        else
            water.filter_remain_value_4 = 0;
        
        uint16_t filter_used_value_5 = (now - water.save.filter_used_timestamp_5 ) / (3600 * 24);
        if (water.save.filter_max_value_5 > filter_used_value_5)
            water.filter_remain_value_5 = water.save.filter_max_value_5 - filter_used_value_5;
        else
            water.filter_remain_value_5 = 0;
        
        
        //======================================================================
        
        
        //充值完后就好改变相应的设备状态了
        if (water.save.is_bind == 1)
        {
            if (water.is_test_mode_switch_off == 1)
            {
                if ( (water.remain_days > 0) && (water.save.remain_flow > 0) )
                {
                    if ( (water.filter_remain_value_1 > 0) && (water.filter_remain_value_2 > 0) && (water.filter_remain_value_3 > 0) && (water.filter_remain_value_4 > 0) && (water.filter_remain_value_5 > 0) )
                    {
                        water.device_status = 0;//正常，不欠費了
                    }
                }
                else
                    water.device_status = 3;//欠費了
                
                if ( (water.filter_remain_value_1 > 0) && (water.filter_remain_value_2 > 0) && (water.filter_remain_value_3 > 0) && (water.filter_remain_value_4 > 0) && (water.filter_remain_value_5 > 0) )
                {
                    if ( (water.remain_days > 0) && (water.save.remain_flow > 0) )
                        water.device_status = 0;//正常，不欠費了
                }
                else
                    water.device_status = 4;
            }
            else
                water.device_status = 5;//硬件调试
        }
        else
            water.device_status = 1;//待激活
        
            static uint8_t device_status_last;
            
            if (device_status_last != water.device_status)
            {
                device_status_last= water.device_status;
                water.io_high_status = -1;//记录IO1状态,高压开关状态
                water.io_high_last_status = -1;
                water.io_low_status = -1;//记录IO2状态,低压开关状态
                water.io_low_last_status = -1;
            }

        
        struct tm *gm_time = gmtime(&now);
        if ((gm_time->tm_hour == 3) && (gm_time->tm_min == 3) && (gm_time->tm_sec == 3))
        {
            void EDP_SendFilter(void);
            EDP_SendFilter();
        }
        
        /* This task should execute every 50 milliseconds. Time is measured
        in ticks. The pdMS_TO_TICKS macro is used to convert milliseconds
        into ticks. xLastWakeTime is automatically updated within vTaskDelayUntil()
        so is not explicitly updated by the task. */
        vTaskDelayUntil( &xLastWakeTime, xPeriod );
	}
}


/**
  * @brief  Configure PA6 in interrupt mode
  * @param  None
  * @retval None
  */
static void EXTI4_15_Config(void)
{
    EXTI_InitTypeDef   EXTI_InitStructure;
    GPIO_InitTypeDef   GPIO_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;

  /* Enable GPIOA clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* Configure PA6 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Connect EXTI6 Line to PA6 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource6);
  
  /* Configure EXTI6 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line6;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI4_15 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/* Prepare the hardware. */
static void prvSetupHardware( void )
{
    /* Initialize Leds mounted on STM320518-EVAL board */
    STM_EVAL_LEDInit(BUSH_LED);

    //STM_EVAL_LEDOff(RUN_LED);
    
    STM_EVAL_LEDOff(BUSH_LED);
    
    STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_EXTI);//按鍵開關
    
    BEEP_Config();
    BEEP_OFF;
    
    I2C_Config();
#ifndef TDA_AB_TOGGLE
    I2C_EE_Config();
    E2WP_DISABLE;//keyi xie 
#endif
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

//延时nus
//nus为要延时的us数.                                             
void wait_us(int us)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;//LOAD的值
    
    ticks = us * (SystemCoreClock / 1000000);//需要的节拍数             
    tcnt = 0;
    told = SysTick->VAL;//刚进入时的计数器值
    for (;;)
    {
        tnow = SysTick->VAL;  
        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow;//这里注意一下SYSTICK是一个递减的计数器就可以了.
            else 
                tcnt += reload - tnow + told;
            told = tnow;
            if (tcnt >= ticks)
                break;//时间超过/等于要延迟的时间,则退出.
        }
    }
}

void wait_ms(int ms)
{
    //wait_us(1000 * ms);
    vTaskDelay( pdMS_TO_TICKS(ms) );
}

void wait(float s)
{
    wait_ms((int)(1000.0 * s));
}

__IO uint32_t LsiFreq = 40000;
static void IWDG_Config(void)
{
    /* Check if the system has resumed from IWDG reset */
    if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
    {
        /* IWDGRST flag set */
        /* Turn on LED1 */
        //STM_EVAL_LEDOn(LED1);
        
        /* Clear reset flags */
        RCC_ClearFlag();
    }
    else
    {
        /* IWDGRST flag is not set */
        /* Turn off LED1 */
        //STM_EVAL_LEDOff(LED1);
    }
    
#ifdef LSI_TIM_MEASURE
    extern __IO uint16_t CaptureNumber;
    void TIM14_ConfigForLSI(void);

    /* TIM Configuration -------------------------------------------------------*/
    TIM14_ConfigForLSI();
    
    /* Wait until the TIM14 get 2 LSI edges */
    while(CaptureNumber != 2)
    {
    }
    
    /* Disable TIM14 CC1 Interrupt Request */
    TIM_ITConfig(TIM14, TIM_IT_CC1, DISABLE);
#endif /* LSI_TIM_MEASURE */
    
    /* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency
    dispersion) */
    /* Enable write access to IWDG_PR and IWDG_RLR registers */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    
    /* IWDG counter clock: LSI/32 */
    IWDG_SetPrescaler(IWDG_Prescaler_32);
    
    /* Set counter reload value to obtain 250ms IWDG TimeOut.
    Counter Reload Value = 250ms/IWDG counter clock period
    = 250ms / (LSI/32)
    = 0.25s / (LsiFreq/32)
    = LsiFreq/(32 * 4)
    = LsiFreq/128
    */
    IWDG_SetReload(LsiFreq/16);//1s
    
    /* Reload IWDG counter */
    IWDG_ReloadCounter();
    
    /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    IWDG_Enable();
}

#ifdef LSI_TIM_MEASURE
/**
* @brief  Configures TIM14 to measure the LSI oscillator frequency.
* @param  None
* @retval None
*/
static void TIM14_ConfigForLSI(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_ICInitTypeDef  TIM_ICInitStructure;
    
    /* Enable peripheral clocks ------------------------------------------------*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    
    /* Allow access to the RTC */
    PWR_BackupAccessCmd(ENABLE);
    
    /* Reset RTC Domain */
    RCC_BackupResetCmd(ENABLE);
    RCC_BackupResetCmd(DISABLE);
    
    /*!< LSI Enable */
    RCC_LSICmd(ENABLE);
    
    /*!< Wait till LSI is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
    {}
    
    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
    
    /* Enable the RTC Clock */
    RCC_RTCCLKCmd(ENABLE);
    
    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();
    
    /* Enable TIM14 clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);  
    
    /* Enable the TIM14 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* Configure TIM14 prescaler */
    TIM_PrescalerConfig(TIM14, 0, TIM_PSCReloadMode_Immediate);
    
    /* Connect internally the TM14_CH1 Input Capture to the LSI clock output */
    TIM_RemapConfig(TIM14, TIM14_RTC_CLK);
    
    /* TIM14 configuration: Input Capture mode ---------------------
    The LSI oscillator is connected to TIM14 CH1
    The Rising edge is used as active edge,
    The TIM14 CCR1 is used to compute the frequency value 
    ------------------------------------------------------------ */
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV8;
    TIM_ICInitStructure.TIM_ICFilter = 0;
    TIM_ICInit(TIM14, &TIM_ICInitStructure);
    
    /* TIM14 Counter Enable */
    TIM_Cmd(TIM14, ENABLE);
    
    /* Reset the flags */
    TIM14->SR = 0;
    
    /* Enable the CC1 Interrupt Request */  
    TIM_ITConfig(TIM14, TIM_IT_CC1, ENABLE);  
}
#endif /* LSI_TIM_MEASURE */

/**
  * @brief  Configure the TIM1 Pins.
  * @param  None
  * @retval None
  */
static void TIM_CaptureConfig(void)
{
  TIM_ICInitTypeDef  TIM_ICInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* GPIOA clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
  /* TIM3 channel 1 pin (PA6) configuration */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Connect TIM pins to AF1 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);
  
  /* TIM3 configuration: Input Capture mode ---------------------
     The external signal is connected to TIM3 CH1 pin (PA.06)  
     The Rising edge is used as active edge,
     The TIM3 CCR1 is used to compute the frequency value 
  ------------------------------------------------------------ */

  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;//通道选择
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//上升沿触发
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//管脚与寄存器对应关系
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//输入预分频，意思是控制在多少个输入周期做一个捕获，如果输入的信号频率没有变，测得的周期也不会变，比如选择四分频，则没四个输入周期做一次捕获，这昂再输入信号不频繁的情况下，可以减少软件被不断中断的次数
  TIM_ICInitStructure.TIM_ICFilter = 0x0;//滤波设置，经历几个周期跳变认定波形稳定0x0-0xf;

  TIM_ICInit(TIM3, &TIM_ICInitStructure);
  
//    //注（一）：若想改变测量的PWM频率范围，可将TIM时钟频率做分频处理 
//    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;     //周期0～FFFF 
//    TIM_TimeBaseStructure.TIM_Prescaler = 5;       //时钟分频，分频数为5+1即6分频 
//    TIM_TimeBaseStructure.TIM_ClockDivision = 0;   //时钟分割 
//    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//模式 
//    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);//基本初始化 
  
  /* TIM enable counter */
  TIM_Cmd(TIM3, ENABLE);

  /* Enable the CC1 Interrupt Request */
  TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
  
  /* Enable the TIM3 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*
* buffer按十六进制输出
*/
void hexdump(const unsigned char *buf, uint32_t num)
{
    for (uint32_t i = 0; i < num; i++) 
    {
        printf("%02X ", buf[i]);
        if ((i+1)%8 == 0)
            printf("\n");
    }
    printf("\n");
}

void EN1_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_EN1_PORT, ENABLE);
    GPIO_InitStructure.GPIO_Pin 	= EN1_PIN ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(EN1_PORT, &GPIO_InitStructure);
}

void EN2_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_EN2_PORT, ENABLE);
    GPIO_InitStructure.GPIO_Pin 	= EN2_PIN ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
    GPIO_Init(EN2_PORT, &GPIO_InitStructure);
}

void BEEP_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_BEEP_PORT, ENABLE);
    GPIO_InitStructure.GPIO_Pin 	= BEEP_PIN ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; 
    GPIO_Init(BEEP_PORT, &GPIO_InitStructure);
}

#if ( configGENERATE_RUN_TIME_STATS == 1 )

void TIM3_Int_Init(uint16_t arr, uint16_t psc);

//FreeRTOS时间统计所用的节拍计数器
volatile unsigned long long FreeRTOSRunTimeTicks;

//初始化TIM3使其为FreeRTOS的时间统计提供时基
void ConfigureTimeForRunTimeStats(void)
{
/* -----------------------------------------------------------------------
    TIM3 Configuration: Output Compare Timing Mode:
    
    In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1), 
    since APB1 prescaler is different from 1.   
      TIM3CLK = 2 * PCLK1  
      PCLK1 = HCLK / 4 
      => TIM3CLK = HCLK / 2 = SystemCoreClock /2
          
    To get TIM3 counter clock at 1 MHz, the prescaler is computed as follows:
       Prescaler = (TIM3CLK / TIM3 counter clock) - 1
       Prescaler = ((SystemCoreClock /2) /1 MHz) - 1
*/
	//定时器3初始化，定时器时钟为84M，分频系数为84-1，所以定时器3的频率
	//为84M/84=1M，自动重装载为50-1，那么定时器周期就是50us
	FreeRTOSRunTimeTicks = 0;
    /* Compute the prescaler value */
    uint16_t rescalerValue = (uint16_t) ((SystemCoreClock / 2) / 1000000) - 1;
	TIM3_Int_Init(50-1, rescalerValue);	//初始化TIM3
}
/*-----------------------------------------------------------*/

//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(uint16_t arr,uint16_t psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

    //中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx
}
/*-----------------------------------------------------------*/

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update)==SET) //溢出中断
	{
		FreeRTOSRunTimeTicks++;
	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除中断标志位
}

#endif

void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
    //如果不想修改空任务，还可以通过空任务的钩子函数插入实现低功耗的代码。
    //在大多数嵌入式操作系统中可以在系统滴答中断函数中退出低功耗模式。 
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	/* This function will be called by each tick interrupt if
	configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
	added here, but the tick hook is called from an interrupt context, so
	code must not attempt to block, and only the interrupt safe FreeRTOS API
	functions can be used (those that end in FromISR()). */
    
    static portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;//这个参数在调用 xxxFromISR()前被设置为 pdFALSE，如果在调用完成后被置为 pdTRUE，则需要进行一次上下文切换。
    
    //time.h
    extern clock_t clk_count;
    extern time_t time_dat;
    clk_count++;
    if ((clk_count % 1000) == 0) {
        time_dat++;
    }
    
    {//按键处理
        static bool _keyUpFlag = false;
        static int _key_up_last_count = 0;
        extern bool key_fall_flag;
        
        if (true == key_fall_flag)//发生按键按下事件
        {
            static int _key_holdon_count = 0;//记录每一次按下的时间长度
            if (STM_EVAL_PBGetState(BUTTON_KEY) == 0x00) //按键 KEY//按键持续按下
            {
                if (_key_holdon_count <= 5000)
                {
                    _key_holdon_count++;
                }
                else //按键按下到2000ms就判断长按时间成立，生成长按标志 
                {
                    key_fall_flag = false;//清按键按下标志 
                    _key_holdon_count = 0; 
                    //printf("LONG\r\n");
                    KeyClick = LONG;
                    
                    if ((water.is_overhaul_status != 1) && (water.is_drop_water_status != 1) && (water.is_power_on_status != 0) && (water.is_shortage_water_status != 1))
                    {
                        is_beep_on = true;
                        static bool is_enter_test_mode = false;
                        is_enter_test_mode = !is_enter_test_mode;
                        if (true == is_enter_test_mode)
                        {
                            if( xTimerStartFromISR( xTestModeTimerHandler, &xHigherPriorityTaskWoken ) == pdPASS )
                            {
                                // 一直按强制冲洗按钮5秒，则进入测试模式
                                water.is_test_mode_switch_off = 0;
                            }
                        }
                        else
                        {
                            if( xTimerStopFromISR( xTestModeTimerHandler, &xHigherPriorityTaskWoken ) == pdPASS )
                            {
                                //也可以再按住强冲按钮5秒不放，恢复到原状态
                                water.is_test_mode_switch_off = 1;
                            }
                        }
                        
                        water.is_bushing_now = 0;
                        if( xTimerIsTimerActive( xForceFlushTimerHandler ) != pdFALSE )
                        {
                            if( xTimerStopFromISR( xForceFlushTimerHandler, &xHigherPriorityTaskWoken ) == pdPASS )
                            {
                                ;
                                ;
                            }
                        }
                    }
                }
            }
            else //按键抬起
            {
                if (_key_holdon_count > 50)//按下时间大于50ms，生成单击标志
                {
                    key_fall_flag  = false;//清按键按下标志
                    _keyUpFlag = true;//单击抬起按键后，生成按键抬起标志 
                    _key_holdon_count = 0;
                    //距离上次单击时间在100~500ms之间，则认为发生连击事件
                    if (_key_up_last_count > 100 && _key_up_last_count < 500)
                    {
                        _key_up_last_count = 0;//如果是连击事件了，就不会再是单击事件
                        _keyUpFlag = false;//
                        //printf("DOUBLE\r\n");
                        KeyClick = DOUBLE;
                    }
                }
                else  //按键持续时间小于50ms，忽略
                {
                    key_fall_flag  = false;//清按键按下标志
                    _key_holdon_count = 0;
                    
                    KeyClick = NONE;//覆盖上一次按键消息
                }
            }
        }// if(key_fall_flag==1)//发生按键按下事件
        
        if (_keyUpFlag)//单击抬起后，启动计数，计数到500ms  
            _key_up_last_count++;
        if (_key_up_last_count > 500)//判断本次按下和上次按下的时间，是否大于500ms，如果大于说明是单击。单击事件必须会有500ms的时间用于判断是否会有下一次的单击出现，用于判断是否是双击
        {
            _key_up_last_count = 0;
            _keyUpFlag = false;
            //printf("SHORT\r\n");
            KeyClick = SHORT;
            if (((water.save.is_bind == 1) || (water.is_test_mode_switch_off == 0) ) && (water.is_overhaul_status != 1) && (water.is_drop_water_status != 1) && (water.is_power_on_status != 0) && (water.is_shortage_water_status != 1))
            {
                
                //如果在运行的话
                
                if( xTimerIsTimerActive( xForceFlushTimerHandler ) == pdFALSE )
                {
                    if( xTimerStartFromISR( xForceFlushTimerHandler, &xHigherPriorityTaskWoken ) == pdPASS )
                    {
                        //用户按下强冲按钮则进行18秒的强制冲洗
                        water.is_bushing_now = 1;
                        is_beep_on = true;
                    }
                }
            }
        }
    }
    
	//如果为 pdTRUE，则在中断退出前应当进行一次上下文切换。这样才能保证中断直接返回到就绪态任务中优先级最高的任务中。
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
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
