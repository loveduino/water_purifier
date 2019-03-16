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

/*
	BASIC INTERRUPT DRIVEN SERIAL PORT DRIVER FOR UART0.
*/

/* Library includes. */
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

/* Demo application includes. */
#include "serial.h"

/* Hardware includes. */
#include "stm32f0xx.h"
#include "stm320518_eval.h"

/*-----------------------------------------------------------*/

/* Misc defines. */
#define serINVALID_QUEUE				( ( QueueHandle_t ) 0 )
#define serNO_BLOCK						( ( TickType_t ) 10 / portTICK_PERIOD_MS )
#define serTX_BLOCK_TIME				( 40 / portTICK_PERIOD_MS )
#define serRX_BLOCK_TIME				( 10 / portTICK_PERIOD_MS )//相邻两帧数据之间的间隔，根据波特率设定

/*-----------------------------------------------------------*/

/* The queue used to hold received characters. */
static QueueHandle_t xRxedChars;
static QueueHandle_t xCharsForTx;


/*-----------------------------------------------------------*/

/*
 * See the serial2.h header file.
 */
xComPortHandle xSerialPortInit( unsigned long ulWantedBaud, unsigned portBASE_TYPE uxTxQueueLength, unsigned portBASE_TYPE uxRxQueueLength )
{
    xComPortHandle xReturn;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

	/* Create the queues used to hold Rx/Tx characters. */
	xRxedChars = xQueueCreate( uxRxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed char ) );
	xCharsForTx = xQueueCreate( uxTxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed char ) );
    
	/* If the queue/semaphore was created correctly then setup the serial port
	hardware. */
	if ( ( xRxedChars != serINVALID_QUEUE ) && ( xCharsForTx != serINVALID_QUEUE ) )
	{
        /* Enable the USART Interrupt */
        NVIC_InitStructure.NVIC_IRQChannel = EVAL_COM1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPriority = 8;//因为串口中断会调用FreeRTOS API，所以这里的优先级要低于 configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);

        /* USARTx configured as follow:
        - BaudRate = baud_rate baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
        */
        USART_InitStructure.USART_BaudRate = ulWantedBaud;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
        
        USART_OverrunDetectionConfig(EVAL_COM1, USART_OVRDetection_Disable);     
        
        STM_EVAL_COMInit(COM1, &USART_InitStructure);
        //只要接收中断打开，即RXNEIE设置为1，那么ORE中断也自动打开了,只有当USART_IT_ERR中断使能时，才能读到ORE中断
        USART_ITConfig(EVAL_COM1, USART_IT_RXNE, ENABLE);   //开启串口接受中断
        USART_ClearFlag(EVAL_COM1, USART_FLAG_RXNE);        //防止配置完就进入中断
        //USART_ITConfig(EVAL_COM1, USART_IT_ORE, ENABLE);    //开启串口溢出中断
        
        xReturn = ( xComPortHandle ) 1;
    }
	else
	{
		xReturn = ( xComPortHandle ) 0;
	}

	/* This demo file only supports a single port but we have to return
	something to comply with the standard demo header file. */
	return xReturn;
}
/*-----------------------------------------------------------*/

signed portBASE_TYPE xSerialReadable( xComPortHandle pxPort )
{
	/* The port handle is not required as this driver only supports one port. */
	( void ) pxPort;

	/* Get the next character from the buffer.  Return false if no characters
	are available, or arrive before xBlockTime expires. */
	if( uxQueueMessagesWaiting( xRxedChars) )
	{
		return pdTRUE;
	}
	else
	{
		return pdFALSE;
	}
}
/*-----------------------------------------------------------*/
 
signed portBASE_TYPE xSerialGetChar( xComPortHandle pxPort, signed char *const pcRxedChar, TickType_t xBlockTime )
{
	/* The port handle is not required as this driver only supports one port. */
	( void ) pxPort;

	/* Get the next character from the buffer.  Return false if no characters
	are available, or arrive before xBlockTime expires. */
	if( xQueueReceive( xRxedChars, pcRxedChar, xBlockTime ) )
	{
		return pdTRUE;
	}
	else
	{
		return pdFALSE;
	}
}
/*-----------------------------------------------------------*/

size_t xSerialGetBytes( xComPortHandle pxPort, unsigned char *const pucRxedBytes, TickType_t xBlockTime )
{
	/* The port handle is not required as this driver only supports one port. */
	( void ) pxPort;
    
    size_t get_len = 0;
    unsigned char *p = pucRxedBytes;
    if (pdTRUE != xSerialGetChar( pxPort, (signed char *)p, xBlockTime ))
        return get_len;
    p++;
    get_len++;
    while (pdTRUE == xSerialGetChar( pxPort, (signed char *)p, serRX_BLOCK_TIME ))
    {
        p++;
        get_len++;
    }
    return get_len;
}
/*-----------------------------------------------------------*/

signed portBASE_TYPE xSerialPutChar( xComPortHandle pxPort, signed char cOutChar, TickType_t xBlockTime )
{
signed portBASE_TYPE xReturn;

	if( xQueueSend( xCharsForTx, &cOutChar, xBlockTime ) == pdPASS )
	{
		xReturn = pdPASS;
		USART_ITConfig( EVAL_COM1, USART_IT_TXE, ENABLE );
	}
	else
	{
		xReturn = pdFAIL;
	}
	return xReturn;
}
/*-----------------------------------------------------------*/

void vSerialPutBytes( xComPortHandle pxPort, const unsigned char * const puBytes, unsigned short usBytesLength )
{
unsigned char *pxNext;

	/* A couple of parameters that this port does not use. */
	( void ) pxPort;

	/* Send each character in the string, one at a time. */
	pxNext = ( unsigned char * ) puBytes;
	while ( usBytesLength-- )
	{
		xSerialPutChar( pxPort, *pxNext, serNO_BLOCK );
        pxNext++;
	}
}
/*-----------------------------------------------------------*/

void vSerialPutString( xComPortHandle pxPort, const signed char * const pcString, unsigned short usStringLength )
{
signed char *pxNext;

	/* A couple of parameters that this port does not use. */
	( void ) usStringLength;
	( void ) pxPort;

	/* Send each character in the string, one at a time. */
	pxNext = ( signed char * ) pcString;
	while( *pxNext )
	{
		xSerialPutChar( pxPort, *pxNext, serNO_BLOCK );
		pxNext++;
	}
}
/*-----------------------------------------------------------*/

void vSerialClose( xComPortHandle xPort )
{
	/* Not supported as not required by the demo application. */
}
/*-----------------------------------------------------------*/


void EVAL_COM1_IRQHandler( void )
{
static portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;//这个参数在调用 xxxFromISR()前被设置为 pdFALSE，如果在调用完成后被置为 pdTRUE，则需要进行一次上下文切换。
char cChar;

	if( USART_GetITStatus( EVAL_COM1, USART_IT_TXE ) == SET )//如果TX是空的，说明可以发送数据
	{
		/* The interrupt was caused by the THR becoming empty.  Are there any
		more characters to transmit? */
		if( xQueueReceiveFromISR( xCharsForTx, &cChar, &xHigherPriorityTaskWoken ) == pdTRUE )//从发送队列中取出数据
		{
			/* A character was retrieved from the queue so can be sent to the
			THR now. */
			USART_SendData( EVAL_COM1, cChar );
		}
		else
		{
			USART_ITConfig( EVAL_COM1, USART_IT_TXE, DISABLE );
		}
	}
	
	if( USART_GetITStatus( EVAL_COM1, USART_IT_RXNE ) == SET )//如果RX非空，说明接收到数据
	{
		cChar = USART_ReceiveData( EVAL_COM1 );
		xQueueSendFromISR( xRxedChars, &cChar, &xHigherPriorityTaskWoken );//向接收队列中写入数据
	}
    
    if(USART_GetITStatus(EVAL_COM1, USART_IT_ORE) != RESET)//注意！不能使用if(USART_GetITStatus(EVAL_COM1, USART_IT_RXNE) != RESET)来判断
    {
        USART_ClearITPendingBit(EVAL_COM1, USART_IT_ORE);
        USART_ReceiveData( EVAL_COM1 );  //这步一定要有的，相当于清空缓存区
    }
    
	//如果为 pdTRUE，则在中断退出前应当进行一次上下文切换。这样才能保证中断直接返回到就绪态任务中优先级最高的任务中。
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}
/*-----------------------------------------------------------*/

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
  #define GETCHAR_PROTOTYPE int __io_getchar(FILE *f)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
  #define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */
/*-----------------------------------------------------------*/

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
//    /* Place your implementation of fputc here */
//    /* e.g. write a character to the USART */
//    USART_SendData(EVAL_COM1, (uint8_t) ch);
//    
//    /* Loop until transmit data register is empty */
//    while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TXE) == RESET)
//    {}
    
    return ch;
}
/*-----------------------------------------------------------*/

GETCHAR_PROTOTYPE
{
    int ch = -1;
//    while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_RXNE) == RESET)
//    {}
//    ch = (int)USART_ReceiveData(EVAL_COM1);
    return ch;
}
/*-----------------------------------------------------------*/

	
