#include <stdio.h>
#include <stdbool.h>

#include "main.h"
#include "gprs.h"
#include "serial.h"
#include "protocol.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"


static portTASK_FUNCTION_PROTO( vEdpRecvProcessTask, pvParameters );
static TaskHandle_t xEdpRecvProcessTaskHandle;

static  void  AppTaskEdpNetCheck        (void        *p_arg);
static TaskHandle_t xedpNetCheckHandlerTask = NULL;

static  void  AppTaskEdpNetPing         (void        *p_arg);
static TaskHandle_t xedpNetPingHandlerTask = NULL;


edp_t edp = {
    .is_tcpip_connect = false,
    .is_ping_resp = false,
    .is_ping_send = false
};

int send_ping_again = 0;

/* Handle to the com port used by both tasks. */
static xComPortHandle xPort = NULL;

void GPRS_PWRKEY_Init (void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(GPRS_PWRKEY_RCC, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPRS_PWRKEY_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPRS_PWRKEY_PORT, &GPIO_InitStructure);
}

//PWRKEY 引脚开机
//单片机输出高电平拉低PWRKEY引脚，PWRKEY引脚低电平有效
//将 PWRKEY 置为低电平，当STATUS 引脚输出高 电平之后，表示开机成功， PWRKEY 引脚可以释放，通过检测STATUS 引脚的电平来判别模块是否开机。
// >1s有效
void GPRS_PWRKEY_On (void)
{
    GPIO_SetBits(GPRS_PWRKEY_PORT, GPRS_PWRKEY_PIN);
    wait(2);
    GPIO_ResetBits(GPRS_PWRKEY_PORT, GPRS_PWRKEY_PIN);
}

//正常关机：控制 PWRKEY 引脚关机
//关机模式可以用STATUS 管脚来指示， 低电平指示模块已进入关机模式。
//关机过程中，模块需要注销 GSM 网络，注销时间与当前网络状态有关，经测定
//用时约 2s~12s，因此建议延长 12s 后再对模块进行断电或重启的操作，以确保在完
//全关机之前让软件保存好重要数据。
void GPRS_PWRKEY_Off (void)
{
    GPIO_SetBits(GPRS_PWRKEY_PORT, GPRS_PWRKEY_PIN);
    wait(12);
    GPIO_ResetBits(GPRS_PWRKEY_PORT, GPRS_PWRKEY_PIN);
}

bool Uart_SendATCmd(const char *const cmd, const char *const result, uint32_t wait_ms, uint8_t times)
{
    uint8_t _times = 1;
    while (1)
    {
        if(_times++ > times)
            return false;
        vSerialPutString( xPort, (int8_t*)cmd, strlen(cmd) );
        int sum=0;
        int len = strlen(result);
        for (;;)
        {
            int8_t c;
            if (pdTRUE == xSerialGetChar( xPort, &c, pdMS_TO_TICKS( wait_ms ) ))
            {
                sum = (c==result[sum]) ? sum+1 : 0;
                if(sum == len)
                    return true;
            }
            else
                
                break;
        }
    }
}

bool Uart_SendATCmdOut(const char *const cmd, const char *const result, const char *const out, uint32_t wait_ms, uint8_t times)
{
    uint8_t _times = 1;
    char *__out = (char *)out;
    
    while (1)
    {
        if(_times++ > times)
            return false;
        vSerialPutString( xPort, (int8_t*)cmd, strlen(cmd) );
        int sum=0;
        int len = strlen(result);
        for (;;)
        {
            int8_t c;
            if (pdTRUE == xSerialGetChar( xPort, &c, pdMS_TO_TICKS( wait_ms ) ))
            {
                *__out = c;
                __out++;
                sum = (c==result[sum]) ? sum+1 : 0;
                if(sum == len)
                    return true;
            }
            else
                break;
        }
    }
}


/*
AT+CGSN

869286031588317

OK
*/
void get_imei_form_str(const uint8_t *const out_buf, char *in_str)
{
    uint8_t *_out_buf = (uint8_t *)out_buf;
    char *p = strstr(in_str, "\r\r\n") + strlen("\r\r\n");
    uint64_t num = atoll(p);
#if 0
    _out_buf[9] = num / 1 % 100;
    _out_buf[8] = num / 100 % 100;
    _out_buf[7] = num / 10000 % 100;
    _out_buf[6] = num / 1000000 % 100;
    _out_buf[5] = num / 100000000 % 100;
    _out_buf[4] = num / 10000000000 % 100;
    _out_buf[3] = num / 1000000000000 % 100;
    _out_buf[2] = num / 100000000000000 % 100;
    _out_buf[1] = num / 10000000000000000 % 100;
    _out_buf[0] = num / 1000000000000000000 % 100;
#else
    _out_buf[9] = (num >>  0) & 0xFF;
    _out_buf[8] = (num >>  8) & 0xFF;
    _out_buf[7] = (num >> 16) & 0xFF;
    _out_buf[6] = (num >> 24) & 0xFF;
    _out_buf[5] = (num >> 32) & 0xFF;
    _out_buf[4] = (num >> 40) & 0xFF;
    _out_buf[3] = (num >> 48) & 0xFF;
    _out_buf[2] = (num >> 56) & 0xFF;
    _out_buf[1] = (num >> 64) & 0xFF;
    _out_buf[0] = (num >> 72) & 0xFF;
    //memcpy((uint8_t *)&_out_buf[0], (uint8_t *)&num, sizeof(uint64_t));
#endif
}

int findStrChrN(const char *s, char c, int n)
{
	int flag = 0;
	int index = 0;
	if (NULL == s)
	{
		return -1;
	}
	while (*s != '\0')
	{
		if (*s == c)
		{
			++flag;
			++index;
			++s;
			if (flag == n)
			{
				return index;
			}
		}
		++s;
		++index;
	}
	return index;
}

/*
AT

AT+CGSN

869286031588317

OK

OK
AT+CGATT=1

OK
AT+CIPMODE=1

OK
AT+CSTT="CMNET"

OK
AT+CIICR

OK
AT+CIFSR

10.200.140.198
AT+CIPSTART="TCP","water.51204433.com",9992

OK

CONNECT
*/

void NET_GPRS_Config(void)
{
    char at_result[100] = {0};
netRoot:
    GPRS_PWRKEY_Init();
    GPRS_PWRKEY_On();
    wait(2);

        //printf("AT\r\n");OK
        if (!Uart_SendATCmd("AT\r\n", "OK", 1000, 5))
            goto netRoot;
            
        memset(at_result, 0x00, sizeof(at_result));
        if (!Uart_SendATCmdOut("AT+CGSN\r\n", "OK", at_result, 1000, 5))
            ;
        //printf(at_result);
        //\r\nAT+CGSN\r\r\n866050034443331\r\n\r\nOK
        if (strstr(at_result, "OK") != NULL)//把IMEI给弄出来
        {
            uint8_t iccid[10] = {0};
            get_imei_form_str(iccid, at_result);
            if (0 != memcmp(g_data_format.iccid, iccid, sizeof(g_data_format.iccid)))//从e2prom中读出来的值应该是和2g模块返回的一样才对
            {
                memcpy(g_data_format.iccid, iccid, sizeof(g_data_format.iccid));
                I2C_EE_BufferWriteSafe(0+sizeof(save_data_default), (uint8_t *)&g_data_format.iccid[0], sizeof(g_data_format.iccid));
                NVIC_SystemReset();//重启
            }
        }
        vTaskDelay(5000);
        //printf("AT+CPIN?\r\n");OK
        if (!Uart_SendATCmd("AT+CPIN?\r\n", "OK", 1000, 5))
            ;//continue;
//        //基本信息{
//        //检查网络信号强度
//        printf("AT+CSQ\r\n");
//        if (!Uart_SendATCmd("AT+CSQ\r\n", "OK", 1000, 5))
//            ;//continue;
//        //查询当前运营商，该指令只有在连上网络后，才返回运营商，否则返回空
//        printf("AT+COPS?\r\n");
//        if (!Uart_SendATCmd("AT+COPS?\r\n", "OK", 1000, 5))
//            ;//continue;
//        //查询模块型号，如返回： SIMCOM_SIM800C
//        printf("AT+CGMM\r\n");
//        if (!Uart_SendATCmd("AT+CGMM\r\n", "OK", 1000, 5))
//            ;//continue;
//        //查询本机号码，必须在 SIM 卡在位的时候才可以查询
//        printf("AT+CNUM\r\n");
//        if (!Uart_SendATCmd("AT+CNUM\r\n", "OK", 2000, 5))
//            ;//continue;
//        //}
            
/*
AT+CSQ

+CSQ: 24,0

OK
*/
        uint8_t get_csq_count = 0;
re_get_csq:
        get_csq_count++;
        memset(at_result, 0x00, sizeof(at_result));
        if (!Uart_SendATCmdOut("AT+CSQ\r\n", "OK", at_result, 1000, 5))
            ;
        if (strstr(at_result, "+CSQ: ") != NULL)//把IMEI给弄出来
        {
            water.rssi = atoi(strstr(at_result, "+CSQ: ")+strlen("+CSQ: "));
        }
        if ((water.rssi == 0) && (get_csq_count < 3))
            goto re_get_csq;
        
        extern time_t time_dat;
        if ((time_dat < 1514764800) || (time_dat > 1925000000))//北京时间2018年1月1日8时0分0秒
        {
            //获取基站时间
            {
                if (!Uart_SendATCmd("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n", "OK", 2000, 2))
                    ;//continue;
                if (!Uart_SendATCmd("AT+SAPBR=3,1,\"APN\",\"3gnet\"\r\n", "OK", 2000, 2))
                    ;//continue;
                if (!Uart_SendATCmd("AT+SAPBR=1,1\r\n", "OK", 5000, 2))
                    ;//continue;
                //SIM800C定位信息解析
                //参数 1 ‘0’表示基站定位获取成功参数 2 经度参数 3 纬度参数 4 海拔参数 5 日期和时间，这里的时间是世界时间（UTC），转换成北京时间         （BTC）要在原来的时间上加 8 个小时，即现在是北京时间 18:05:46
                memset(at_result, 0x00, sizeof(at_result));
                if (!Uart_SendATCmdOut("AT+CLBS=4,1\r\n", "OK", at_result, 2000, 5))
                    ;
                //+CLBS: 0,106.505072,29.584707,550,18/12/16,14:42:23
                if (strstr(at_result, "+CLBS: 0") != NULL)
                {
                    /*
                    struct tm {
                    int tm_sec;   // 秒，正常范围从 0 到 59，但允许至 61
                    int tm_min;   // 分，范围从 0 到 59
                    int tm_hour;  // 小时，范围从 0 到 23
                    int tm_mday;  // 一月中的第几天，范围从 1 到 31
                    int tm_mon;   // 月，范围从 0 到 11
                    int tm_year;  // 自 1900 年起的年数
                    int tm_wday;  // 一周中的第几天，范围从 0 到 6，从星期日算起，星期日表示为 0
                    int tm_yday;  // 一年中的第几天，范围从 0 到 365，从 1 月 1 日算起
                    int tm_isdst; // 夏令时
                }
                    */
                    char *index = strstr(at_result, "+CLBS: 0");
                    struct tm read_time;
                    
                    read_time.tm_year = atoi(index + findStrChrN(index, ',', 4)) + 2000 - 1900;
                    read_time.tm_mon =  atoi(index + findStrChrN(index, '/', 1)) - 1;
                    //read_time.tm_wday = atoi(index + findStrChrN(index, '/', 2));
                    read_time.tm_mday = atoi(index + findStrChrN(index, '/', 2));
                    read_time.tm_hour = atoi(index + findStrChrN(index, ',', 5));
                    read_time.tm_min =  atoi(index + findStrChrN(index, ':', 2));
                    read_time.tm_sec =  atoi(index + findStrChrN(index, ':', 3));  
                    read_time.tm_isdst = -1;
                    //mktime()用来将参数timeptr所指的tm结构数据转换成从公元1970年1月1日0时0分0 秒算起至今的UTC时间所经过的秒数。
                    time_dat = mktime(&read_time);  //并将时间初始化给系统time库时间
                }
                
                if (!Uart_SendATCmd("AT+SAPBR=0,1\r\n", "OK", 2000, 2))
                    ;//continue;
            }
        }
        
        //获取基站的lac和cid
        {
            uint8_t get_lac_cid_count = 0;
        re_get_lac_cid:
            get_lac_cid_count++;
/*
AT+CGREG=2

OK
*/
        if (!Uart_SendATCmd("AT+CGREG=2\r\n", "OK", 1000, 5))
            ;//continue;
/*    
AT+CGREG?

+CGREG: 2,1,"3361","EB15"

OK
*/
        memset(at_result, 0x00, sizeof(at_result));
        if (!Uart_SendATCmdOut("AT+CGREG?\r\n", "OK", at_result, 1000, 5))//lac cid
            ;
        if (strstr(at_result, "+CGREG: 2") != NULL)
        {
            char *str;
            char *index = NULL;
            
            index = strstr(at_result, ",\"") + strlen(",\"");
            water.lac_value = (int)strtol(index, &str, 16);//十六进制  
            
            index = strstr(index, ",\"") + strlen(",\"");
            water.cid_value = (int)strtol(index, &str, 16);//十六进制  
        }
        if ((water.lac_value == 0) && (water.cid_value == 0) && (get_csq_count < 3))
            goto re_get_lac_cid;
        }
        
        //透传{
        //设置附着和分离 GPRS 业务
        //printf("AT+CGATT=1\r\n");
        
        if (!Uart_SendATCmd("AT+CGCLASS=\"B\"\r\n", "OK", 2000, 10));
        
        if (!Uart_SendATCmd("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n", "OK", 2000, 10));
        
        vTaskDelay(1000);
        
        if (!Uart_SendATCmd("AT+CGATT=1\r\n", "OK", 3000, 5))//容易出错，后面就不用再处理了，反正也会失败的
            ;//continue;
        //printf("AT+CIPMODE=1\r\n");
        if (!Uart_SendATCmd("AT+CIPMODE=1\r\n", "OK", 1000, 5))
            ;//continue;
        //开始任务，设置APN
        //printf("AT+CSTT=\"CMNET\"\r\n");
        if (!Uart_SendATCmd("AT+CSTT=\"CMNET\"\r\n", "OK", 2000, 5))
            ;//continue;

        vTaskDelay(1000);    
        
        //建立无线链路
        //printf("AT+CIICR\r\n");
        if (!Uart_SendATCmd("AT+CIICR\r\n", "OK", 2000, 5))
            ;//continue;
        //获得本地IP地址
        //printf("AT+CIFSR\r\n");10.177.167.229
        if (!Uart_SendATCmd("AT+CIFSR\r\n", ".", 2000, 5))
            ;//continue;
        //}
        
        //printf("AT+CIPSTART=\"TCP\",\"water.51204433.com\",9992\r\n");
        if (!Uart_SendATCmd("AT+CIPSTART=\"TCP\",\"lot.youheone.com\",\"9992\"\r\n", "CONNECT", 2000, 30))
        //if (!Uart_SendATCmd("AT+CIPSTART=\"TCP\",\"122.114.122.174\",\"42127\"\r\n", "CONNECT", 2000, 30))
        {
            //printf("connect failed\n");
            goto netRoot;
        }
        
        //联网成功后，蜂鸣器向两下
        for (int i = 0; i < 2; i++)
        {
            BEEP_ON;
            vTaskDelay(100);  
            BEEP_OFF;
            vTaskDelay(100);
        }
        
        uint8_t buf[100] = {0};
        int len = xSerialGetBytes( xPort, buf, ( TickType_t ) pdMS_TO_TICKS( 3000 ));
        //printf("GPRS Net Config OK!");
        
        vTaskDelay(100); 
        edp.is_tcpip_connect = true;
}
/*-----------------------------------------------------------*/

static portTASK_FUNCTION( vEdpRecvProcessTask, pvParameters )
{
	/* Just to stop compiler warnings. */
	( void ) pvParameters;
    
	for ( ;; )
	{
        //获取当前网络状态
        if (!edp.is_tcpip_connect)
        {
//            if (NULL != xedpNetPingHandlerTask)
//                vTaskDelete( &xedpNetPingHandlerTask );
            
            NET_GPRS_Config();
            edp.is_ping_send = false;
            EDP_SendPacketPing( );
            
            
            
            extern bool is_send_device_status;
            is_send_device_status = true;
            edp.is_ping_send = true;
            edp.is_ping_resp = false;
            send_ping_again = 0;
            //xTaskCreate( AppTaskEdpNetPing, "PING", configMINIMAL_STACK_SIZE, NULL, edpNET_PING_TASK_PRIORITY, ( TaskHandle_t * ) &xedpNetPingHandlerTask );

        }
        else
        {
            //uint8_t *buf = NULL;
            //buf = (uint8_t*)pvPortMalloc(1000 * sizeof(uint8_t));
            
            
            //if (NULL == buf)
            //    for(;;);
            
            uint8_t buf[1500] = {0};
            
            int len = xSerialGetBytes( xPort, buf, ( TickType_t ) pdMS_TO_TICKS( 100 ));
            
            if (len > 0)
            {
                    
#if 0
                printf("len = %d\r\n", len);
                for (int i = 0; i < len; i++)
                {
                    printf(" %02x", buf[i]);
                }
                printf("\n");
#endif

                if (strstr((char*)buf, "CLOSED") != NULL)
                    edp.is_tcpip_connect = false;
                else
                {
                    if (0 == memcmp((uint8_t *)buf, g_data_format.iccid, 10))
                        ProtocolProcess((uint8_t *)buf, len);
                }
            }
            //vPortFree(buf);
        }
# if (INCLUDE_uxTaskGetStackHighWaterMark)
        UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
        printf("vEdpRecvProcessTask space avaiable = %d\r\n", uxHighWaterMark);
#endif
	}
} /*lint !e715 !e818 pvParameters is required for a task function even if it is not referenced. */
/*-----------------------------------------------------------*/




static portTASK_FUNCTION( AppTaskEdpNetPing, pvParameters )
{
	/* Just to stop compiler warnings. */
	( void ) pvParameters;
    
    for (;;)
    {
        if (edp.is_tcpip_connect)
        {
            vTaskDelay( pdMS_TO_TICKS(2 * 1000) );
            //printf("发送ping\r\n");
            if (send_ping_again == 0)
            {
                edp.is_ping_send = false;
                EDP_SendPacketPing( );
                
                edp.is_ping_send = true;
                edp.is_ping_resp = false;
                send_ping_again = 0;
            }
            
            /* We want this task to execute exactly every 3 minutes. */
            vTaskDelay( pdMS_TO_TICKS(abs(water.save.ping_time-2) * 1000) );
        }
        else
        {
            vTaskDelay( pdMS_TO_TICKS(1000) );
        }
    }
}

int count__ = 0;
static portTASK_FUNCTION( AppTaskEdpNetCheck, pvParameters )
{
	/* Just to stop compiler warnings. */
	( void ) pvParameters;
    
    //printf("创建EDP ping信号检测任务\r\n");
    
    
    for (;;)
    {
        
        if (edp.is_ping_send == true)
        {
            if (edp.is_ping_resp == true)//发送了心跳，有返回，证明网络正常
            {
                edp.is_ping_send = false;
                edp.is_ping_resp = false;
                send_ping_again = 0;
            }
            if (count__++ >= 10)//等待30s还是没有网络返回，说明网络故障
            {
                count__ = 0;
                extern bool is_iap_status;
                if (!is_iap_status)//沒有升级的时候，判断超时没有回应时，以为网络故障。
                {
                    if (send_ping_again < 2)
                    {
                        send_ping_again++;
                        EDP_SendPacketPing( );
                        edp.is_ping_send = true;
                        edp.is_ping_resp = false;
                    }
                    else
                    {
                        edp.is_tcpip_connect = false;//网络异常，重启网络
                        edp.is_ping_send = false;
                        edp.is_ping_resp = false;
                    }
                }
            }
        }
        else
            count__ = 0;
        
        vTaskDelay ( pdMS_TO_TICKS(1000) );
        
    }
}

void AppEdpNetCheckGive(void)
{
    xTaskNotifyGive( xedpNetCheckHandlerTask );
}


void prvGprsTask( void *pvParameters )
{
	/* Just to stop compiler warnings. */
	( void ) pvParameters;
    GPRS_PWRKEY_Init();
    
	xTaskCreate( vEdpRecvProcessTask, "EDP RECV PROCESS", 1000, NULL, tskIDLE_PRIORITY + 3, ( TaskHandle_t * ) &xEdpRecvProcessTaskHandle );
    //创建Edp Net Check任务，检测edp的连接是否正常
    xTaskCreate( AppTaskEdpNetCheck, "EDP NET CHECK", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 4, &xedpNetCheckHandlerTask );
    
    xTaskCreate( AppTaskEdpNetPing, "PING", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 4, ( TaskHandle_t * ) &xedpNetPingHandlerTask );
    
    vTaskDelete( NULL );
}


