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

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE. 
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

/* Ensure stdint is only used by the compiler, and not the assembler. */
//针对不同的编译器调用不同的stdint.h文件
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
	#include <stdint.h>
	extern uint32_t SystemCoreClock;
#endif


/******************************************************************************/
/*                FreeRTOS基础配置配置选项                                    */
/******************************************************************************/
#define configUSE_PREEMPTION			        1//为1时RTOS使用抢占式调度器，为0时RTOS使用协作式调度器（时间片）
#define configUSE_TIME_SLICING					1//1使能时间片调度(默认式使能的)，多个任务可以共用一个优先级，数量不限。
#define configUSE_PORT_OPTIMISED_TASK_SELECTION	0//1启用特殊方法来选择下一个要运行的任务。如果设置1，configMAX_PRIORITIES不能超过32！其他情况下，configMAX_PRIORITIES可以为任意值！
                                                 //一般是硬件计算前导零指令，如果所使用的
                                                 //MCU没有这些硬件指令的话此宏应该设置为0！
#define configUSE_QUEUE_SETS					1//为1时启用队列
#define configCPU_CLOCK_HZ				        ( SystemCoreClock )//入实际的CPU内核时钟频率，也就是CPU指令执行频率，通常称为Fcclk
#define configTICK_RATE_HZ				        ( ( TickType_t ) 1000 )//RTOS 系统节拍中断的频率
#define configMAX_PRIORITIES			        ( 32 )//可使用的最大优先级，用户实际可以使用的优先级范围是0到configMAX_PRIORITIES– 1;用户配置任务的优先级数值越小，那么此任务的优先级越低，空闲任务的优先级是0。
#define configMINIMAL_STACK_SIZE		        ( ( unsigned short ) 128 )//空闲任务使用的堆栈大小，The number of words (not bytes!) to allocate for use as the task's stack. 
#define configMAX_TASK_NAME_LEN			        ( 16 )//任务名字字符串长度
#define configUSE_16_BIT_TICKS					0//系统节拍计数器变量数据类型，
                                                 //1表示为16位无符号整形，0表示为32位无符号整形
#define configIDLE_SHOULD_YIELD					1//为1时空闲任务放弃CPU使用权给其他同优先级的用户任务
#define configUSE_TASK_NOTIFICATIONS            1//为1时开启任务通知功能，默认开启
#define configUSE_MUTEXES						1//为1时使用互斥信号量
#define configQUEUE_REGISTRY_SIZE				8//不为0时表示启用队列记录，具体的值是可以记录的队列和信号量最大数目。
#define configCHECK_FOR_STACK_OVERFLOW			2//大于0时启用堆栈溢出检测功能，如果使用此功能
                                                 //用户必须提供一个栈溢出钩子函数，如果使用的话
                                                 //此值可以为1或者2，因为有两种栈溢出检测方法。1：不断的检测任务堆栈指针是否指向有效空间，如果指向了无效空间，就会调用钩子函数。优点快，缺点不能检测所有的堆栈溢出！
                                                 //2: 创建任务的时候会向任务堆栈填充一个已知的标记值，会一直检测堆栈后面的几个bytes（标记值）是否被改写。如果被改写就调用钩子函数。优点机会能检测所有的堆栈溢出，缺点相比1慢一些！
                                                 //堆栈溢出检测会增加上下文切换的开销，建议在调试的时候使用！
#define configUSE_RECURSIVE_MUTEXES				1//为1时使用递归互斥信号量
#define configUSE_MALLOC_FAILED_HOOK			1//1使用内存申请失败钩子函数
#define configUSE_APPLICATION_TASK_TAG			0
#define configUSE_COUNTING_SEMAPHORES			1//为1时使用计数信号量

/* The full demo always has tasks to run so the tick will never be turned off.
The blinky demo will use the default tickless idle implementation to turn the
tick off. */
#define configUSE_TICKLESS_IDLE					0//1启用低功耗tickless模式


/******************************************************************************/
/*             FreeRTOS与内存申请有关配置选项                                 */
/******************************************************************************/
#define configSUPPORT_DYNAMIC_ALLOCATION        1//支持动态内存申请
#define configSUPPORT_STATIC_ALLOCATION			0//支持静态内存，在创建一些内核对象的时候需要用户指定RAM
#define configTOTAL_HEAP_SIZE					( ( size_t ) ( 20 * 1024 ) )//系统所有总的堆大小
                                                 //RTOS内核总计可用的有效的RAM大小。
                                                 //仅在你使用官方下载包中附带的内存分配策略时，才有可能用到此值。
                                                 //每当创建任务、队列、互斥量、软件定时器或信号量时，RTOS内核会为此分配RAM，这里的RAM都属于configTOTAL_HEAP_SIZE指定的内存区。


/******************************************************************************/
/*             FreeRTOS与钩子函数有关的配置选项                               */
/******************************************************************************/
#define configUSE_IDLE_HOOK						1//1，使用空闲钩子；0，不使用
#define configUSE_TICK_HOOK						1//1，使用时间片钩子；0，不使用


/* Run time stats gathering definitions. */
/******************************************************************************/
/*             FreeRTOS与运行时间和任务状态收集有关的配置选项                 */
/******************************************************************************/
#define configGENERATE_RUN_TIME_STATS	        0//为1时启用运行时间统计功能
/*
If configGENERATE_RUN_TIME_STATS is defined then portCONFIGURE_TIMER_FOR_RUN_TIME_STATS must also be defined.  
portCONFIGURE_TIMER_FOR_RUN_TIME_STATS should call a port layer function to setup a peripheral timer/counter that can then be used as the run time counter time base.
*/
//初始化一个外设，作为时钟统计的基准时钟
#if ( configGENERATE_RUN_TIME_STATS == 1 )
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
extern volatile unsigned long long FreeRTOSRunTimeTicks;
void ConfigureTimeForRunTimeStats(void);
#endif
    #define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() ConfigureTimeForRunTimeStats()//定时器3提供时间统计的时基，频率为10K，即周期为100us
#endif
/*
If configGENERATE_RUN_TIME_STATS is defined then either portGET_RUN_TIME_COUNTER_VALUE or portALT_GET_RUN_TIME_COUNTER_VALUE must also be defined.  
See the examples provided and the FreeRTOS web site for more information.
*/
//返回当前基准时钟的时钟值
#define portGET_RUN_TIME_COUNTER_VALUE()		FreeRTOSRunTimeTicks//获取时间统计时间值

#define configUSE_TRACE_FACILITY		        1//设置成1表示启动可视化跟踪调试，会激活一些附加的结构体成员和函数。
/* This demo makes use of one or more example stats formatting functions.  These
format the raw data provided by the uxTaskGetSystemState() function in to human
readable ASCII form.  See the notes in the implementation of vTaskList() within
FreeRTOS/Source/tasks.c for limitations. */
#define configUSE_STATS_FORMATTING_FUNCTIONS	1//与宏configUSE_TRACE_FACILITY同时为1时会编译下面3个函数
                                                 //prvWriteNameToBuffer(),vTaskList(),
                                                 //vTaskGetRunTimeStats()


/* Co-routine definitions. */
/******************************************************************************/
/*            FreeRTOS与协程有关的配置选项                                    */
/******************************************************************************/
#define configUSE_CO_ROUTINES 			        0                       //为1时启用协程，启用协程以后必须添加文件croutine.c。功能有限，建议关闭！
#define configMAX_CO_ROUTINE_PRIORITIES         ( 2 )                   //协程的有效优先级数目


/* Software timer definitions. */
/******************************************************************************/
/*            FreeRTOS与软件定时器有关的配置选项                              */
/******************************************************************************/
#define configUSE_TIMERS				        1                               //为1时启用软件定时器
#define configTIMER_TASK_PRIORITY		        (configMAX_PRIORITIES-1)        //软件定时器优先级
#define configTIMER_QUEUE_LENGTH		        10                              //软件定时器队列长度
#define configTIMER_TASK_STACK_DEPTH	        (configMINIMAL_STACK_SIZE*2)    //软件定时器任务堆栈大小


/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
/******************************************************************************/
/*           FreeRTOS可选函数配置选项                                         */
/******************************************************************************/
#define INCLUDE_xTaskGetSchedulerState          1 
#define INCLUDE_vTaskPrioritySet		        1
#define INCLUDE_uxTaskPriorityGet		        1
#define INCLUDE_vTaskDelete				        1
#define INCLUDE_vTaskCleanUpResources	        1
#define INCLUDE_vTaskSuspend			        1
#define INCLUDE_vTaskDelayUntil			        1
#define INCLUDE_vTaskDelay				        1
#define INCLUDE_eTaskGetState			        1
#define INCLUDE_xTimerPendFunctionCall	        1
#define	INCLUDE_xTaskGetHandle			        0
#define INCLUDE_uxTaskGetStackHighWaterMark     0//query how close a task has come to overflowing the stack space allocated to it.
                                                 //returns the minimum amount of remaining stack space that has been available since the task started executing.
                                                 //The closer the high water mark is to zero, the closer the task has come to overflowing its stack.
                                                 //调试阶段使用，发布的时候最好不要使用，因为此函数会消耗时间。

/* Cortex-M0 specific definitions. */
/******************************************************************************/
/*          FreeRTOS与中断有关的配置选项                                      */
/******************************************************************************/
/*!< STM32F0 uses 2 Bits for the Priority Levels    */
//STM32只有4级优先级，在设计芯片的时候会裁掉表达式优先级的几个低端有效位，以减少优先级数。
//不管采用多少位来表达优先级，都是MSB对齐的。
/*
| bit7    | bit6     | bit5 | bit4 | bit3 | bit2 | bit1 | bit0 |
| <-用于表达优先级-> | <-        没有实现，读回零           -> |
*/
#ifdef __NVIC_PRIO_BITS
	/* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
	#define configPRIO_BITS       		                __NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		                2   /* 4 priority levels */
#endif

/*
如果有用户中断函数调用到freertos提供的系统函数，一定要使用带FromISR的freertos提供的系统函数，
并且这个用户中断的优先级一定要在configKERNEL_INTERRUPT_PRIORITY和configMAX_SYSCALL_INTERRUPT_PRIORITY的优先级之间，
一般configKERNEL_INTERRUPT_PRIORITY设置为单片机最低优先级，configMAX_SYSCALL_INTERRUPT_PRIORITY是FreeRTOS系统能够屏蔽的最高优先级，
规定中断优先级比configMAX_SYSCALL_INTERRUPT_PRIORITY高不能调用FreeRTOS API（如此高的中断优先级，已经不在FreeRTOS系统控制范围内）。
*/

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
//设置最低优先级，STM32F0优先级数使用了2位（configPRIO_BITS），而且STM32配置使用组4，也就是4位都是抢占式优先级。
//因此优先级数就是4个，最低优先级数那就是3.CORTEX内核的优先级是数值越小优先级越高，即0是最高优先级。
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			3//中断最低优先级

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
//操作系统可管理的最大中断优先级，高于5的优先级（优先级数小于2，0~1）不归FreeRTOS管理。
//不调用任何FreeRTOS API的中断可以使用所有的中断优先级，并且它们可以中断嵌套
//优先级0~1中断不会被FreeRTOS禁止，不会因为执行FreeRTOS内核而延时，中断不可调用FreeRTOS的API函数
//优先级2~3中断可以调用以“FromISR”结尾的API函数，并且他们可以中断嵌套
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	2//FreeRTOS为了满足某些应用对中断实时性要求高的需求，使得中断优先级高于某个值之后，就不能调用操作系统的内核函数来提高实时性。

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
//设置内核中断优先级，STM32使用了4位作为优先级，而这4位是高4位，因此要左移4位才是真正的优先级。
//将freertos系统内核中断（configKERNEL_INTERRUPT_PRIORITY）的优先级设置成最低
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )//0x0f -> 0xF0
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
//低于此优先级的中断可以安全的调用FreeRTOS的API函数；高于此优先级的中断，FreeRTOS是不能禁止的，中断服务函数也不能调用FreeRTOS的API函数。
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )//0x50

/* This is the value being used as per the ST library which permits 4
priority values, 0 to 3.  This must correspond to the
configKERNEL_INTERRUPT_PRIORITY setting.  Here 15 corresponds to the lowest
NVIC value of 255. */
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY	3


/******************************************************************************/
/*                 FreeRTOS与断言有关的配置选项                               */
/******************************************************************************/
//STM32库使用 USE_FULL_ASSERT 定义断言检测
/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
#ifdef USE_RTOS_ASSERT//调试的时候可以使用断言，当调试完成后尽量去掉断言，防止增加内销!
/* Ensure stdio is only used by the compiler, and not the assembler. */
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
    #include <stdio.h>
#endif
#define vAssertCalled( char, int ) printf( "Error:%s, %d\r\n", char, int )//vAssertCalled函数需要用户自行去定义
#define configASSERT( x ) if( ( x ) == 0 ) vAssertCalled(__FILE__,__LINE__)
//#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }
#else
#define configASSERT( x )   ((void)0)
#endif /* USE_RTOS_ASSERT */


/******************************************************************************/
/*                FreeRTOS与中断服务函数有关的配置选项                        */
/******************************************************************************/
/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names - or at least those used in the unmodified vector table. */
/* 移植要实现的三个中断 , 为了尽量少改动启动文件，其余的中断函数名称不改变，屏蔽掉原始库中的中断函数定义*/
#define vPortSVCHandler                         SVC_Handler
#define xPortPendSVHandler                      PendSV_Handler
#define xPortSysTickHandler                     SysTick_Handler
 
#endif /* FREERTOS_CONFIG_H */
