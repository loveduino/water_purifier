#include "protocol.h"
#include "crc_soft.h"
#include "serial.h"
#include "rtc.h"
#include "e2prom.h"
#include "gprs.h"

#include <string.h>
#include <stdio.h>
#include <time.h>

#include "menu.h"
#include "common.h"

//说明：固件版本号V1_1.0.3中V0表示bootloader，V1为设备A区运行固件，版本号V2为设备B区运行固件
#if BOOTLOADER
#define HW_VERSION         "V0_0.3.4"          //V0表示bootloader
#elif APP_A
#define HW_VERSION         "V1_0.3.4"          //V1为设备A区运行固件
#elif APP_B
#define HW_VERSION         "V2_0.3.4"          //V2为设备B区运行固件
#endif

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/*
typedef struct{
    uint8_t iccid[10];
    uint8_t cmd;
    uint16_t length;
    uint8_t data[100];//变长，根据具体功能决定
    uint8_t crc[2];
}data_format_t;
*/

data_format_t g_data_format = {
    .iccid = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09}
};



//上电的初始化值
struct water_s water =
{
    .device_status = 0,//备用
    .is_screen_status_off = 0,//屏幕打开（上电默认值）
    .used_days = 0,
    .remain_days = 0,
    .filter_remain_value_1 = 0,//第一级滤芯剩余值	HEX（2字节）
    .filter_remain_value_2 = 0,//第二级滤芯剩余值	HEX（2字节）
    .filter_remain_value_3 = 0,//第三级滤芯剩余值	HEX（2字节）
    .filter_remain_value_4 = 0,//第四级滤芯剩余值	HEX（2字节）
    .filter_remain_value_5 = 0,//第五级滤芯剩余值	HEX（2字节）
    .rssi = 0,//信号强度值
    .lac_value = 0,
    .cid_value = 0,
    .date_time = 0,
    .is_make_water_status = 0,
    .is_full_water_status = 0,
    .is_overhaul_status = 0,
    .is_drop_water_status = 0,
    .is_power_on_status = 1,
    .is_shortage_water_status = 0,
    .is_bushing_now = 0,
    .is_test_mode_switch_off = 1,//关闭调试模式
    .test_used_days = 0,
    .test_used_flow = 0,
    .flowmeter_count = 0,//流量计
    .raw_water_ppm = 0,////实际检测到的ppm值
    .clean_water_ppm = 0,////实际检测到的ppm值
    .overhaul_status_switch = 0x00,//0x00解除检修  //0x01进入检修
    .io_high_status = -1,//记录IO1状态,高压开关状态
    .io_high_last_status = -1,
    .io_low_status = -1,//记录IO2状态,低压开关状态
    .io_low_last_status = -1
};

//上电的初始化值，如果要恢复出厂就需要
struct water_s water_default =
{
    .device_status = 0,//备用
    .is_screen_status_off = 0,//屏幕打开（上电默认值）
    .used_days = 0,
    .remain_days = 0,
    .filter_remain_value_1 = 0,//第一级滤芯剩余值	HEX（2字节）
    .filter_remain_value_2 = 0,//第二级滤芯剩余值	HEX（2字节）
    .filter_remain_value_3 = 0,//第三级滤芯剩余值	HEX（2字节）
    .filter_remain_value_4 = 0,//第四级滤芯剩余值	HEX（2字节）
    .filter_remain_value_5 = 0,//第五级滤芯剩余值	HEX（2字节）
    .rssi = 0,//信号强度值
    .lac_value = 0,
    .cid_value = 0,
    .date_time = 0,
    .is_make_water_status = 0,
    .is_full_water_status = 0,
    .is_overhaul_status = 0,
    .is_drop_water_status = 0,
    .is_power_on_status = 1,
    .is_shortage_water_status = 0,
    .is_bushing_now = 0,
    .is_test_mode_switch_off = 1,//关闭调试模式
    .test_used_days = 0,
    .test_used_flow = 0,
    .flowmeter_count = 0,//流量计
    .raw_water_ppm = 0,////实际检测到的ppm值
    .clean_water_ppm = 0,////实际检测到的ppm值
    .overhaul_status_switch = 0x00,//0x00解除检修  //0x01进入检修
    .io_high_status = -1,//记录IO1状态,高压开关状态
    .io_high_last_status = -1,
    .io_low_status = -1,//记录IO2状态,低压开关状态
    .io_low_last_status = -1
};

//保存在e2prom里面的默认值
const struct save_data save_data_default =
{
    .e2prom_init = 123456,
    .is_bind = 0,
    .bind_timestamp = 0,//记录绑定那天的时间戳
    .work_mode = 0,//0	流量模式    1	时长模式
    .used_flow = 0,
    .remain_flow = 0,
    .used_days_timestamp = 0,//已用天数时间戳
    .remain_days_timestamp = 0,//剩余天数时间戳
    .clean_water_ppm = 0,
    .clean_water_tds_switch = 1,//0显示默认值，1显示实时值
    .raw_water_ppm = 0,
    .raw_water_tds_switch = 1,//0显示默认值，1显示实时值
    .filter_used_timestamp_1 = 0,//滤芯的开始使用时间戳
    .filter_used_timestamp_2 = 0,//滤芯的开始使用时间戳
    .filter_used_timestamp_3 = 0,//滤芯的开始使用时间戳
    .filter_used_timestamp_4 = 0,//滤芯的开始使用时间戳
    .filter_used_timestamp_5 = 0,//滤芯的开始使用时间戳
    .filter_max_value_1 = 0,
    .filter_max_value_2 = 0,
    .filter_max_value_3 = 0,
    .filter_max_value_4 = 0,
    .filter_max_value_5 = 0,
    .force_flush_time = 18,//18秒的强制冲洗
    .period_flush_time = 3600*2,//累计制水2小时进行强制冲洗
    .maintenance_time = 8*3600,//连续制水8小时进入检修状态
    .ping_time = 180,//设置心跳间隔时长180秒
    .reconnect_time = 0,
    .screen_mode_switch.day = 0x00,//已用天数
    .screen_mode_switch.flow = 0x02//已用流量
};

//int buf_to_struct(data_format_t *data_format, uint8_t *buf, int buf_len);
//int struct_to_buf(uint8_t *buf, int *buf_len, data_format_t data_format);


const cmd_process_t cmd_process[] = {
    {CMD_HEARTBEAT, cmd_heartbeat_process},//0x01,//心跳帧
    {CMD_BINDING_PACKAGE, cmd_binding_package_process},//0x02,//绑定套餐
    {CMD_CLOSE_THE_SCREEN, cmd_close_the_screen_process},//0x03,//关闭屏幕
    {CMD_OPEN_THE_SCREEN, cmd_open_the_screen_process},//0x04,//打开屏幕
    {CMD_POWER_OFF, cmd_power_off_process},//0x05,//关机
    {CMD_POWER_ON, cmd_power_on_process},//0x06,//开机
    {CMD_STRONG_PUNCH, cmd_strong_push_process},//0x07,//强冲
    {CMD_POSITIVE_VALUE, cmd_positive_value_process},//0x08,//充正值

    {CMD_FILTER_SEND, cmd_filter_send_process},//用户设备主动上报滤芯状态，每天上报一次的方式
    {CMD_FILTER_REQUIRE, cmd_filter_require_process},//主动获取滤芯状态

    {CMD_SYNCHRONIZE_WITH_WATER, cmd_synchronize_with_water_process},//0x0a,//用水同步
    //{CMD_TIME_SYNCHRONIZATION, cmd_time_synchronization_process},//0x0b,//用时同步
    {CMD_TIME_SYNCHRONIZATION_USED, cmd_time_synchronization_used_process},//用时同步 0xBA已用天数
    {CMD_TIME_SYNCHRONIZATION_REMAIN, cmd_time_synchronization_remain_process},//用时同步 0xBB剩余天数
    {CMD_WORK_STATUS_SYNCHRONIZATION, cmd_work_status_synchronization_process},//0x0c,//工作状态同步
    {CMD_QUERY_DEVICE_OPERATION_INFORMATION, cmd_query_device_operation_information_process},//0x0d,//查询设备运行信息
    {CMD_FILTER_RESET_AND_MODIFICATION, cmd_filter_reset_and_modification_process},//0x0e,//滤芯复位与修改
    {CMD_MODE_SWITCHING, cmd_mode_switching_process},//0x0f,//模式切换
    {CMD_RESET, cmd_reset_process},//0x10,//恢复出厂设置
    //修改域名和端口号
    {CMD_PARAMETER_MODIFICATION, cmd_parameter_modification_process},//0x11,//参数修改
    {CMD_TIMING_FLUSH_PARAMETER_MODIFICATION, cmd_timing_flush_parameter_modification_process},//0x12,//定时冲洗参数修改
    {CMD_MAINTENANCE_PARAMETER_MODIFICATION, cmd_maintenance_parameter_modification_process},//0x13,//检修参数修改
    {CMD_CONTROL_PARAMETER_MODIFICATION_1, cmd_control_parameter_modification_1_process},//0x14,//控制参数修改1
    {CMD_CONTROL_PARAMETER_MODIFICATION_2, cmd_control_parameter_modification_2_process},//0x15,//控制参数修改2
    {CMD_TEST_MODE_SWITCH, cmd_test_mode_switch_process},//0x16,//开启关闭测试模式
    {CMD_COMPUTER_BOARD_TIME_SYNCHRONIZATION_1, cmd_computer_board_time_synchronization_1},//0x17,// 电脑板时间同步1
    {CMD_COMPUTER_BOARD_TIME_SYNCHRONIZATION_2, cmd_computer_board_time_synchronization_2},//0x18,// 电脑板时间同步2
    {CMD_SYNCHRONIZATION_OF_WATER_CONSUMPTION_USED, cmd_synchronization_of_water_consumption_used},//0xAA,// 用水量同步 0xAA已用流量
    {CMD_SYNCHRONIZATION_OF_WATER_CONSUMPTION_REMAIN, cmd_synchronization_of_water_consumption_remain},//0xAB,// 用水量同步 0xAB剩余流量

    {CMD_REMOTE_UPGRADE_VERSION, cmd_remote_upgrade_version},//服务端获取固件当前版本号
    {CMD_REMOTE_UPGRADE_DATA0, cmd_remote_upgrade_data0},//远程升级文件名
    {CMD_REMOTE_UPGRADE_DATA1, cmd_remote_upgrade_data1},//远程升级文件
    {CMD_REMOTE_UPGRADE_DATA2, cmd_remote_upgrade_data2},//远程升级结束
    {CMD_REMOTE_UPGRADE_REBOOT, cmd_remote_upgrade_reboot},//远程重启
    {CMD_REMOTE_UPGRADE_NEW, cmd_remote_upgrade_new},//下次启动设备时是否运行新固件
    {CMD_REMOTE_UPGRADE_SWITCH, cmd_remote_upgrade_switch},//运行固件切换

    {CMD_SCREEN_MODE_SWITCH, cmd_screen_mode_switch},//屏幕显示模式切换0x5A
    {CMD_OVERHAUL_STATUS_SWITCH, cmd_overhaul_status_switch},//检修状态切换0x6A
    {CMD_GET_DEVICE_WORK_STATUS, cmd_get_device_work_status},//获取设备工作状态0x1c
};

const uint8_t cmd_process_cnt = sizeof( cmd_process ) / sizeof( cmd_process[0] );

//电脑板主动上传无需应答
int cmd_heartbeat_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len) //0x01,//心跳帧
{
    int ret = 0;

    //void AppEdpNetCheckGive(void);
    //AppEdpNetCheckGive();
    edp.is_ping_resp = true;

    return ret;
}

int cmd_binding_package_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len) //0x02,//绑定套餐
{
    int ret = 0;

    if ((0x00 == in_buf[0]) || (0x01 == in_buf[0]))
    {
        water.save.is_bind = 1;
        water.save.work_mode = in_buf[0];//00：流量模式   01：时间模式
        water.save.filter_max_value_1 = (in_buf[1] << 8) + (in_buf[2] << 0);
        water.save.filter_max_value_2 = (in_buf[3] << 8) + (in_buf[4] << 0);
        water.save.filter_max_value_3 = (in_buf[5] << 8) + (in_buf[6] << 0);
        water.save.filter_max_value_4 = (in_buf[7] << 8) + (in_buf[8] << 0);
        water.save.filter_max_value_5 = (in_buf[9] << 8) + (in_buf[10] << 0);

        //从绑定开始存储时间戳
        time_t now = time(NULL);
        water.save.bind_timestamp = now;
        water.save.filter_used_timestamp_1 = now;
        water.save.filter_used_timestamp_2 = now;
        water.save.filter_used_timestamp_3 = now;
        water.save.filter_used_timestamp_4 = now;
        water.save.filter_used_timestamp_5 = now;

        water.save.used_days_timestamp = now;

        *out_len = in_len;
        memcpy(out_buf, in_buf, *out_len);
        ret = 1;
    }
    return ret;
}

int cmd_close_the_screen_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len) //0x03,//关闭屏幕
{
    int ret = 0;

    //if (0x00 == in_buf[0])
    {
        water.is_screen_status_off = 1;//关闭屏幕

        *out_len = in_len;
        memcpy(out_buf, in_buf, *out_len);
        ret = 1;
    }
    return ret;
}

int cmd_open_the_screen_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len) //0x04,//打开屏幕
{
    int ret = 0;

    //if (0x00 == in_buf[0])
    {

        water.is_screen_status_off = 0;//打开屏幕

        *out_len = in_len;
        memcpy(out_buf, in_buf, *out_len);
        ret = 1;
    }
    return ret;
}

int cmd_power_off_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len) //0x05,//关机
{
    int ret = 0;

    //if (0x00 == in_buf[0])
    {
        if ((1 == water.save.is_bind) ||  (water.is_test_mode_switch_off == 1))//用于关闭，并关闭制水功能。必须绑定套餐后操作，否则无响应。
        {
            //6	关机
            //关闭软件定时器，如果在运行的话
            water.is_bushing_now = 0;
            extern TimerHandle_t xForceFlushTimerHandler;
            if( xTimerIsTimerActive( xForceFlushTimerHandler ) != pdFALSE )
            {
                xTimerStop( xForceFlushTimerHandler, 100 );
            }

            water.is_power_on_status = 0;
            *out_len = in_len;
            memcpy(out_buf, in_buf, *out_len);
            ret = 1;
        }
    }
    return ret;
}

int cmd_power_on_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len) //0x06,//开机
{
    int ret = 0;

    //if (0x00 == in_buf[0])
    {
        if ((1 == water.save.is_bind) ||  (water.is_test_mode_switch_off == 1))//用于开机，并开启制水功能。必须绑定套餐后操作，否则无响应。
        {
            water.is_power_on_status = 1;
            water.io_high_status = -1;//记录IO1状态,高压开关状态
            water.io_high_last_status = -1;
            water.io_low_status = -1;//记录IO2状态,低压开关状态
            water.io_low_last_status = -1;

            *out_len = in_len;
            memcpy(out_buf, in_buf, *out_len);
            ret = 1;
        }
    }
    return ret;
}

int cmd_strong_push_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len) //0x07,//强冲
{
    int ret = 0;

    if (0x00 == in_buf[0])
    {
        if ((1 == water.save.is_bind) ||  (water.is_test_mode_switch_off == 1))//用于强制冲洗。必须绑定套餐后操作，否则无响应。
        {
            //TODO
            //用于强制冲洗
            if ((water.is_overhaul_status != 1) && (water.is_drop_water_status != 1) && (water.is_power_on_status != 0) && (water.is_shortage_water_status != 1))
            {
                extern TimerHandle_t xForceFlushTimerHandler;
                if( xTimerStart( xForceFlushTimerHandler, pdMS_TO_TICKS(100) ) == pdPASS )
                {
                    water.is_bushing_now = 1;
                }
            }

            *out_len = in_len;
            memcpy(out_buf, in_buf, *out_len);
            ret = 1;
        }
    }
    return ret;
}

/*
用于同步天数及流量。必须绑定套餐后操作，否则无响应。
1、如果在时长模式，则可显示已用天数、剩余天数、已用流量
2、如果在流量模式，则可显示已用天数、剩余天数、已用流量、剩余流量
3、如果切换套餐，例如时长模式切换为流量模式，平台需要单独下发套餐及对应的已用天数、剩余天数、已用流量、剩余流量给设备。如果切换流量模式为时长模式，平台也需要单独下发套餐及对应的已用天数、剩余天数、已用流量
*/
int cmd_positive_value_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len) //0x08,//充确定值
{
    int ret = 0;

    if (1 == water.save.is_bind)//用于同步天数及流量。必须绑定套餐后操作，否则无响应。
    {
        /*
        0	流量模式
        1	时长模式
        */
        {
            water.used_days = (in_buf[0] << 8) + in_buf[1];
            water.remain_days = (in_buf[2] << 8) + in_buf[3] + 1;

            water.save.used_flow = (uint32_t)((((uint32_t)in_buf[4] << 24) + ((uint32_t)in_buf[5] << 16) + ((uint32_t)in_buf[6] << 8) + ((uint32_t)in_buf[7] << 0)) / 0.917 + 0.5);
            if (water.save.work_mode == 0)//0	流量模式
            {
                water.save.remain_flow = (uint32_t)((((uint32_t)in_buf[8] << 24) + ((uint32_t)in_buf[9] << 16) + ((uint32_t)in_buf[10] << 8) + ((uint32_t)in_buf[11] << 0)) / 0.917 + 0.5);
            }
            else
            {
                water.save.remain_flow = 0xFFFFFFFF;
            }

            time_t now = time(NULL);
            water.save.used_days_timestamp = now - water.used_days * 3600 * 24;
            water.save.remain_days_timestamp = now + water.remain_days * 3600 * 24;

            *out_len = in_len;
            memcpy(out_buf, in_buf, *out_len);
            ret = 1;
        }
    }
    return ret;
}

int cmd_filter_send_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len) //用户设备主动上报滤芯状态，每天上报一次的方式
{
    int ret = 0;
    return ret;
}

int cmd_filter_require_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len) //主动获取滤芯状态
{
    int ret = 0;
    //滤芯状态上传
    void EDP_SendFilter(void);
    EDP_SendFilter();
    return ret;
}

//用水量上传
//每次水龙头停止出水5秒后上传本次用水量，单位/10ml。
//电脑板主动上传无需应答
int cmd_synchronize_with_water_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len) //0x0a,//用水同步
{
    int ret = 0;

    return ret;
}

/*
int cmd_time_synchronization_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len) //0x0b,//用时同步
{
    int ret = 0;

    if (1 == water.save.bind)//必须绑定套餐后操作，否则无响应。
    {
        water.used_days = (in_buf[0] << 8) + in_buf[1];

        *out_len = in_len;
        memcpy(out_buf, in_buf, *out_len);
        ret = 1;
    }
    return ret;
}
*/

int cmd_time_synchronization_used_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len)//用时同步 0xBA已用天数
{
    int ret = 0;

    if (1 == water.save.is_bind)//必须绑定套餐后操作，否则无响应。
    {
        uint16_t used_days = (in_buf[0] << 8) + in_buf[1];
        time_t now = time(NULL);
        //修改已经用了多少天的时间搓，向过去方向走
        water.save.used_days_timestamp = now - used_days * 3600 * 24;

        *out_len = in_len;
        memcpy(out_buf, in_buf, *out_len);
        ret = 1;
    }
    return ret;
}

int cmd_time_synchronization_remain_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len)//用时同步 0xBB剩余天数
{
    int ret = 0;

    if (1 == water.save.is_bind)//必须绑定套餐后操作，否则无响应。
    {
        time_t now = time(NULL);
        water.remain_days = (in_buf[0] << 8) + in_buf[1] + 1;
        water.save.remain_days_timestamp = now + water.remain_days*3600*24,

        *out_len = in_len;
        memcpy(out_buf, in_buf, *out_len);
        ret = 1;
    }
    return ret;
}

/*
0x01制水
0x02水满
0x03缺水
0x04检修
*/
//设备运行状态更新时，主动上传此消息,电脑板主动上传无需应答
int cmd_work_status_synchronization_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len) //0x0c,//工作状态同步
{
    int ret = 0;

    extern bool SendPacketStatusSuccess;
    SendPacketStatusSuccess = true;//説明接受到返回的数据了，就不需要再次上传设备状态了

    return ret;
}

//立刻返回心跳信息到服务器
int cmd_query_device_operation_information_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len) //0x0d,//查询设备运行信息
{
    int ret = 0;

    void EDP_SendPacketPing(void); //0x01,//心跳帧
    EDP_SendPacketPing();
    return ret;
}

int cmd_filter_reset_and_modification_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len) //0x0e,//滤芯复位与修改
{
    int ret = 0;

    if (1 == water.save.is_bind)//必须绑定套餐后操作，否则无响应。
    {
        uint16_t remain_value = (in_buf[1] << 8) + in_buf[2];
        time_t now = time(NULL);

        switch (in_buf[0])
        {
        case 0x00: break;
        case 0x01: water.save.filter_used_timestamp_1 = now; break;//修改已经使用的天数记录的时间挫，那么最大天数没有变，剩余天数就是最大的天数。已经使用的天数就是0
        case 0x02: water.save.filter_used_timestamp_2 = now; break;
        case 0x03: water.save.filter_used_timestamp_3 = now; break;
        case 0x04: water.save.filter_used_timestamp_4 = now; break;
        case 0x05: water.save.filter_used_timestamp_5 = now; break;
        case 0x06:
            water.save.filter_used_timestamp_1 = now;
            water.save.filter_used_timestamp_2 = now;
            water.save.filter_used_timestamp_3 = now;
            water.save.filter_used_timestamp_4 = now;
            water.save.filter_used_timestamp_5 = now;
            break;
        case 0x07:
            if (remain_value > water.save.filter_max_value_1)
                water.save.filter_max_value_1 = remain_value;
            //修改了滤芯的剩余值，然后滤芯使用的天数就是 最大值-剩余值。然后根据已用的天数，获得这个已用的时间搓相对于现在提前了多久，从而间隔求得第一次使用额时间搓。后面计算的时候，就用现在天数时间搓-第一次使用的时间搓，就是滤芯已经使用的天数了。剩余值就是最大值-剩余值
            water.save.filter_used_timestamp_1 = now - (water.save.filter_max_value_1 - remain_value)*3600*24; //当前的时间搓 - 已经的时间搓，然后保存记下来，相当于修改了滤芯开始使用的时间搓
            break;
        case 0x08:
            if (remain_value > water.save.filter_max_value_2)
                water.save.filter_max_value_2 = remain_value;
            water.save.filter_used_timestamp_2 = now - (water.save.filter_max_value_2 - remain_value)*3600*24; //当前的时间搓 - 已经的时间搓，然后保存记下来，相当于修改了滤芯开始使用的时间搓
            break;
        case 0x09:
            if (remain_value > water.save.filter_max_value_3)
                water.save.filter_max_value_3 = remain_value;
            water.save.filter_used_timestamp_3 = now - (water.save.filter_max_value_3 - remain_value)*3600*24; //当前的时间搓 - 已经的时间搓，然后保存记下来，相当于修改了滤芯开始使用的时间搓
            break;
        case 0x0a:
            if (remain_value > water.save.filter_max_value_4)
                water.save.filter_max_value_4 = remain_value;
            water.save.filter_used_timestamp_4 = now - (water.save.filter_max_value_4 - remain_value)*3600*24; //当前的时间搓 - 已经的时间搓，然后保存记下来，相当于修改了滤芯开始使用的时间搓
            break;
        case 0x0b:
            if (remain_value > water.save.filter_max_value_5)
                water.save.filter_max_value_5 = remain_value;
            water.save.filter_used_timestamp_5 = now - (water.save.filter_max_value_5 - remain_value)*3600*24; //当前的时间搓 - 已经的时间搓，然后保存记下来，相当于修改了滤芯开始使用的时间搓
            break;
        default:break;
        }

        *out_len = in_len;
        memcpy(out_buf, in_buf, *out_len);
        ret = 1;
    }
    return ret;
}

//模式切换
int cmd_mode_switching_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len) //0x0f,//模式切换
{
    int ret = 0;
    /*
    0	流量模式
    1	时长模式
    */
    if ((0x00 == in_buf[0]) || (0x01 == in_buf[0]))
    {
        water.save.work_mode = in_buf[0];

        *out_len = in_len;
        memcpy(out_buf, in_buf, *out_len);
        ret = 1;
    }
    return ret;
}

//用于电脑板参数恢复出厂设置状态
int cmd_reset_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len) //0x10,//恢复出厂设置
{
    int ret = 0;

    if (0x00 == in_buf[0])
    {
        //恢复出厂就解除了綁定
        memcpy(&water, &water_default, sizeof(water_default));
        memcpy(&water.save, &save_data_default, sizeof(save_data_default));
        //clear e2prom
        I2C_EE_BufferWriteSafe(0, (uint8_t *)&save_data_default, sizeof(save_data_default));
        *out_len = in_len;
        memcpy(out_buf, in_buf, *out_len);
        ret = 1;
    }
    return ret;
}


//修改域名和端口号


//修改强制冲洗时间系统参数修改
int cmd_parameter_modification_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len) //0x11,//参数修改
{
    int ret = 0;

    water.save.force_flush_time = in_buf[0];

    extern TimerHandle_t xForceFlushTimerHandler;
    void vForceFlushTimerCallback( TimerHandle_t pxTimer );

    //删除定时器，并重新创建定时器
    water.is_bushing_now = 0;
    xTimerDelete( xForceFlushTimerHandler, pdMS_TO_TICKS(100) );
    xForceFlushTimerHandler = xTimerCreate(    "Force Flush Timer",   // Just a text name, not used by the kernel.
                                 ( pdMS_TO_TICKS(water.save.force_flush_time*1000) ),   // The timer period in ticks.
                                 pdFALSE,        // The timers will auto-reload themselves when they expire.
                                 ( void * ) NULL, // Assign each timer a unique id equal to its array index.
                                 vForceFlushTimerCallback); // Each timer calls the same callback when it expires.

    *out_len = in_len;
    memcpy(out_buf, in_buf, *out_len);
    ret = 1;
    return ret;
}

//定时冲洗参数修改
int cmd_timing_flush_parameter_modification_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len) //0x12,//定时冲洗参数修改
{
    int ret = 0;

    water.save.period_flush_time = ((uint32_t)in_buf[0] << 16) + ((uint32_t)in_buf[1] << 8) + ((uint32_t)in_buf[2] << 0);

    *out_len = in_len;
    memcpy(out_buf, in_buf, *out_len);
    ret = 1;
    return ret;
}

//修改进入检修状态时长
int cmd_maintenance_parameter_modification_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len) //0x13,//检修参数修改
{
    int ret = 0;

    water.save.maintenance_time = ((uint32_t)in_buf[0] << 16) + ((uint32_t)in_buf[1] << 8) + ((uint32_t)in_buf[2] << 0);
    *out_len = in_len;
    memcpy(out_buf, in_buf, *out_len);
    ret = 1;
    return ret;
}

//TDS值设置
int cmd_control_parameter_modification_1_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len) //0x14,//控制参数修改1
{
    int ret = 0;

    if (0x01 == in_buf[0])//0x01纯水
    {
        if ((0x00 == in_buf[1]) || (0x01 == in_buf[1]))//TDS检测开关00关，01开
        {
            water.save.clean_water_tds_switch = in_buf[1];//0显示默认值，1显示实时值
            water.save.clean_water_ppm = (in_buf[2] << 8) + in_buf[3];

            *out_len = in_len;
            memcpy(out_buf, in_buf, *out_len);
            ret = 1;
        }
    }
    else if (0x02 == in_buf[0])//0x02原水
    {
        if ((0x00 == in_buf[1]) || (0x01 == in_buf[1]))//TDS检测开关00关，01开
        {
            water.save.raw_water_tds_switch = in_buf[1];//0显示默认值，1显示实时值
            water.save.raw_water_ppm = (in_buf[2] << 8) + in_buf[3];

            *out_len = in_len;
            memcpy(out_buf, in_buf, *out_len);
            ret = 1;
        }
    }
    return ret;
}

//联网参数修改
int cmd_control_parameter_modification_2_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len) //0x15,//控制参数修改2
{
    int ret = 0;
    if (0x01 == in_buf[0])//0x01心跳间隔
    {
        water.save.ping_time = (in_buf[1] << 8) + in_buf[2];
    }
    else if (0x02 == in_buf[0])//0x02断网重连
    {
        water.save.reconnect_time = (in_buf[1] << 8) + in_buf[2];//未使用
    }

    *out_len = in_len;
    memcpy(out_buf, in_buf, *out_len);
    ret = 1;
    return ret;
}

int cmd_test_mode_switch_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len)//0x16,//开启关闭测试模式
{
    int ret = 0;
    /*
    0x00  开启
    0x01  关闭
    */
    if (0x00 == in_buf[0])//开启调试模式
    {
        water.is_test_mode_switch_off = 0;

        *out_len = in_len;
        memcpy(out_buf, in_buf, *out_len);
        ret = 1;
    }
    else if (0x01 == in_buf[0])//关闭调试模式
    {
        water.is_test_mode_switch_off = 1;

        *out_len = in_len;
        memcpy(out_buf, in_buf, *out_len);
        ret = 1;
    }
    return ret;
}

//设备向服务器请求时间
//向服务器发送当前的设备时间，并且服务器也会向设备发送当前的服务器时间
//用于电脑版和服务器端的时间同步，设备主动上传，设备每次开机启动，或者在固定的时间点（例如每周星期一凌晨03:00:00进行时间同步），向服务器请求时间同步。
int cmd_computer_board_time_synchronization_1(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len)//0x17,// 电脑板时间同步1
{
    int ret = 0;

    time_t seconds = ((uint32_t)in_buf[1] << 24) + ((uint32_t)in_buf[2] << 16) + ((uint32_t)in_buf[3] << 8) + ((uint32_t)in_buf[4] << 0);
    PCF8563_SetTime(seconds);
    //seconds = PCF8563_ReadTime();

    extern time_t time_dat;
    time_dat = seconds;//修改系统时间
    return ret;
}


time_t seconds_aaaa;

//服务器强制更新设备时间
//用于电脑版和服务器端的时间同步，服务端下发设备时间同步指令进行时间同步。
int cmd_computer_board_time_synchronization_2(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len)//0x18,// 电脑板时间同步2
{
    int ret = 0;

    time_t seconds = ((uint32_t)in_buf[1] << 24) + ((uint32_t)in_buf[2] << 16) + ((uint32_t)in_buf[3] << 8) + ((uint32_t)in_buf[4] << 0);

    PCF8563_SetTime(seconds);
    //seconds_aaaa = seconds = PCF8563_ReadTime();

    extern time_t time_dat;
    time_dat = seconds;//修改系统时间
    return ret;
}

//用水量同步:已用流量
int cmd_synchronization_of_water_consumption_used(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len)//0xAA,// 用水量同步 0xAA已用流量
{
    int ret = 0;

    if (1 == water.save.is_bind)//必须绑定套餐后操作，否则无响应。
    {
        water.save.used_flow = (uint32_t)((((uint32_t)in_buf[0] << 24) + ((uint32_t)in_buf[1] << 16) + ((uint32_t)in_buf[2] << 8) + ((uint32_t)in_buf[3] << 0)) / 0.917 + 0.5);

        *out_len = in_len;
        memcpy(out_buf, in_buf, *out_len);
        ret = 1;
    }
    return ret;
}

//用水量同步:剩余流量
int cmd_synchronization_of_water_consumption_remain(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len)//0xAB,// 用水量同步 0xAB剩余流量
{
    int ret = 0;

    if (1 == water.save.is_bind)//必须绑定套餐后操作，否则无响应。
    {
        water.save.remain_flow = (uint32_t)((((uint32_t)in_buf[0] << 24) + ((uint32_t)in_buf[1] << 16) + ((uint32_t)in_buf[2] << 8) + ((uint32_t)in_buf[3] << 0)) / 0.917 + 0.5);

        *out_len = in_len;
        memcpy(out_buf, in_buf, *out_len);
        ret = 1;
    }
    return ret;
}


bool is_iap_status = false;
int32_t packets_received = 0;
uint32_t flashdestination, ramsource;

int cmd_remote_upgrade_version(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len)//0x1A,//服务端获取固件当前版本号
{
    int ret = 0;
    uint16_t i = 0;

    out_buf[i++] = HW_VERSION[0];
    out_buf[i++] = HW_VERSION[1];
    out_buf[i++] = HW_VERSION[2];
    out_buf[i++] = HW_VERSION[3];
    out_buf[i++] = HW_VERSION[4];
    out_buf[i++] = HW_VERSION[5];
    out_buf[i++] = HW_VERSION[6];
    out_buf[i++] = HW_VERSION[7];
    *out_len = i;
    ret = 1;

    return ret;
}

int cmd_remote_upgrade_data0(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len)//0x2A,//远程升级文件名
{
    int32_t i, size = 0;
    uint8_t file_size[16], *file_ptr;
    uint8_t FileName[256] = {0};

    *out_len = 1;

    if (in_buf[0] != 0x01)//128
    {
        out_buf[0] = 0;
        return 1;
    }

    if (in_buf[1] != ((in_buf[2] ^ 0xff) & 0xff))
    {
        out_buf[0] = 0;
        return 1;
    }
    packets_received = 0;
    /* Initialize flashdestination variable */
    flashdestination = APPLICATION_ADDRESS;
    FLASH_If_Init();
    is_iap_status = true;
    if (packets_received == 0)//第一包数据不写入到flash中
    {
        /* Filename packet */
        if (in_buf[3] != 0)
        {
            /* Filename packet has valid data */
            for (i = 0, file_ptr = in_buf + 3; (*file_ptr != 0) && (i < 256);)
            {
                FileName[i++] = *file_ptr++;
            }
            FileName[i++] = '\0';

            //判斷file name是否符合要求
#if APP_A
            if (NULL == strstr((char*)FileName, "V2_"))
#elif APP_B | BOOTLOADER
            if (NULL == strstr((char*)FileName, "V1_"))
#endif
            {
                /* End session */
                out_buf[0] = 0;
                return 1;
            }

            for (i = 0, file_ptr ++; (*file_ptr != ' ') && (i < (16 - 1));)
            {
                file_size[i++] = *file_ptr++;
            }
            file_size[i++] = '\0';
            Str2Int(file_size, &size);

            /* Test the size of the image to be sent */
            /* Image size is greater than Flash size */
            if (size > (USER_FLASH_SIZE + 1))
            {
                /* End session */
                out_buf[0] = 0;
                return 1;
            }
            packets_received  = 1;
            /* erase user application area */
            FLASH_If_Erase(APPLICATION_ADDRESS);
            // 清除标志位，说明已经修改了固件，回不去了
#if APP_A
            const char app_run[10] = {0};
            I2C_EE_BufferWrite(0+sizeof(save_data_default)+sizeof(g_data_format.iccid)+APP_B_OFFSET_ADDR, (uint8_t *)app_run, 10);
#elif APP_B | BOOTLOADER
            const char app_run[10] = {0};
            I2C_EE_BufferWrite(0+sizeof(save_data_default)+sizeof(g_data_format.iccid)+APP_A_OFFSET_ADDR, (uint8_t *)app_run, 10);
#endif
            out_buf[0] = 1;
            return 1;
        }
    }
    out_buf[0] = 0;
    return 1;
}

int cmd_remote_upgrade_data1(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len)//0x3A,//远程升级文件
{
    *out_len = 1;

    if (in_buf[0] != 0x02)//1024
    {
        out_buf[0] = 0;
        return 1;
    }

    if (in_buf[1] != ((in_buf[2] ^ 0xff) & 0xff))
    {
        out_buf[0] = 0;
        return 1;
    }

    if (packets_received != 0)//第一包数据不写入到flash中
    {
        /* Write received data in Flash */
        if (FLASH_If_Write(&flashdestination, (uint32_t*) &in_buf[3], (uint16_t) 1024/4) == 0)
        {
            packets_received++;
            out_buf[0] = 1;
            return 1;
        }
    }
    out_buf[0] = 0;
    return 1;
}

int cmd_remote_upgrade_data2(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len)//0x4A,//远程升级结束
{
    *out_len = 1;
    is_iap_status = false;
    if (in_buf[0] != 0x01)//128
    {
        out_buf[0] = 0;
        return 1;
    }

    if (in_buf[1] != ((in_buf[2] ^ 0xff) & 0xff))
    {
        out_buf[0] = 0;
        return 1;
    }

    if (packets_received != 0)
    {
        //if ()//对升级的结果进行判断，如果正确就设置标志位
        {
#if APP_A
            const char app_run[10] = "app_b_on";
            I2C_EE_BufferWrite(0+sizeof(save_data_default)+sizeof(g_data_format.iccid)+APP_B_OFFSET_ADDR, (uint8_t *)app_run, 10);
#elif APP_B | BOOTLOADER
            const char app_run[10] = "app_a_on";
            I2C_EE_BufferWrite(0+sizeof(save_data_default)+sizeof(g_data_format.iccid)+APP_A_OFFSET_ADDR, (uint8_t *)app_run, 10);
#endif
            out_buf[0] = 1;
            return 1;
        }
    }
    out_buf[0] = 0;
    return 1;
}


int cmd_remote_upgrade_reboot(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len)//0x7A,//远程重启
{
    int ret = 0;
    out_buf[0] = 0x01;
    *out_len = 1;
    ret = 1;

    return ret;
}


int cmd_remote_upgrade_new(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len)//0x8A,//下次启动设备时是否运行新固件
{
    *out_len = 1;

#if APP_A
        char buff[10] = "app_b_on";
        I2C_EE_BufferRead(0+sizeof(save_data_default)+sizeof(g_data_format.iccid)+APP_B_OFFSET_ADDR, (uint8_t *)buff, 10);
        if (strstr(buff, "app_b_on"))
        {
            I2C_EE_BufferWrite(0+sizeof(save_data_default)+sizeof(g_data_format.iccid)+APP_O_OFFSET_ADDR, (uint8_t *)buff, 10);
            out_buf[0] = 0x01;//成功：固件正常，下次以新固件运行
            return 1;
        }
#elif APP_B | BOOTLOADER
        char buff[10] = "app_a_on";
        I2C_EE_BufferRead(0+sizeof(save_data_default)+sizeof(g_data_format.iccid)+APP_A_OFFSET_ADDR, (uint8_t *)buff, 10);
        if (strstr(buff, "app_a_on"))
        {
            I2C_EE_BufferWrite(0+sizeof(save_data_default)+sizeof(g_data_format.iccid)+APP_O_OFFSET_ADDR, (uint8_t *)buff, 10);
            out_buf[0] = 0x01;//成功：固件正常，下次以新固件运行
            return 1;
        }
#endif
    out_buf[0] = 0x00;//失败：固件接受存在问题，不能用新固件运行
    return 1;
}

int cmd_remote_upgrade_switch(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len)//0x9A,//33、运行固件切换
{
    *out_len = 1;

    if (in_buf[0] == 0x00)//运行BOOTLOADER
    {
        char buff[10] = "app_o_on";
        I2C_EE_BufferWrite(0+sizeof(save_data_default)+sizeof(g_data_format.iccid)+APP_O_OFFSET_ADDR, (uint8_t *)buff, 10);
        out_buf[0] = 0x01;
        return 1;
    }
    else if (in_buf[0] == 0x01)//运行A区固件
    {
        char buff[10] = {0};
        I2C_EE_BufferRead(0+sizeof(save_data_default)+sizeof(g_data_format.iccid)+APP_A_OFFSET_ADDR, (uint8_t *)buff, 10);
        if (strstr(buff, "app_a_on"))
        {
            I2C_EE_BufferWrite(0+sizeof(save_data_default)+sizeof(g_data_format.iccid)+APP_O_OFFSET_ADDR, (uint8_t *)buff, 10);
            out_buf[0] = 0x01;
            return 1;
        }
    }
    else if (in_buf[0] == 0x02)//运行B区固件
    {
        char buff[10] = {0};
        I2C_EE_BufferRead(0+sizeof(save_data_default)+sizeof(g_data_format.iccid)+APP_B_OFFSET_ADDR, (uint8_t *)buff, 10);
        if (strstr(buff, "app_b_on"))
        {
            I2C_EE_BufferWrite(0+sizeof(save_data_default)+sizeof(g_data_format.iccid)+APP_O_OFFSET_ADDR, (uint8_t *)buff, 10);
            out_buf[0] = 0x01;
            return 1;
        }
    }
    out_buf[0] = 0x00;
    return 1;
}

int cmd_screen_mode_switch(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len)//屏幕显示模式切换0x5A
{
    int ret = 0;

    if (1 == water.save.is_bind)//必须绑定套餐后操作，否则无响应。
    {
        if ( ((in_buf[0] == 0x00) || (in_buf[0] == 0x01))  &&  ((in_buf[1] == 0x02) || (in_buf[1] == 0x03)) )
        {
            water.save.screen_mode_switch.day = in_buf[0]; //0x00	已用天数   0x01	剩余天数
            water.save.screen_mode_switch.flow = in_buf[1];//0x02	已用流量   0x03	剩余流量

            *out_len = in_len;
            memcpy(out_buf, in_buf, *out_len);
            ret = 1;
        }
    }
    return ret;
}

//当连续制水超过8个小时说明出现了故障，需要停机操作，然后蜂鸣器提醒用户了
int cmd_overhaul_status_switch(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len)//检修状态切换0x6A
{
    int ret = 0;

    if (1 == water.save.is_bind)//必须绑定套餐后操作，否则无响应。
    {
        if ( (in_buf[0] == 0x00) || (in_buf[0] == 0x01) )
        {
            water.overhaul_status_switch = in_buf[0]; //0x00解除检修  //0x01进入检修

            if (water.overhaul_status_switch)//0x01进入检修
            {
                //设备故障
                water.is_overhaul_status = 1;
                water.is_overhaul_beep = 1;
            }
            else
            {
                water.is_overhaul_status = 0;
                water.is_overhaul_beep = 0;
                water.is_bushing_now = 0;
                water.io_high_status = -1;//记录IO1状态,高压开关状态
                water.io_high_last_status = -1;
                water.io_low_status = -1;//记录IO2状态,低压开关状态
                water.io_low_last_status = -1;
            }

            *out_len = in_len;
            memcpy(out_buf, in_buf, *out_len);
            ret = 1;
        }
    }

    return ret;
}

//89 86 02 b1 03 17 00 11 71 84 1C 00 01 01 B7 5D
int cmd_get_device_work_status(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len)//0x1c, 获取设备工作状态0x1c
{
    int ret = 0;

    if (1 == water.save.is_bind)//必须绑定套餐后操作，否则无响应。
    {
        if ( in_buf[0] == 0x01 )
        {
            uint8_t total_buf[100+15] = {0};
            int total_len = 0;

            g_data_format.cmd = 0x0C;
            int i = 0;
            g_data_format.data[i++] = (uint8_t)( (bool)water.is_make_water_status );//制水状态
            g_data_format.data[i++] = (uint8_t)( (bool)water.is_full_water_status );//水满状态
            g_data_format.data[i++] = (uint8_t)( (bool)water.is_overhaul_status );//检修
            g_data_format.data[i++] = (uint8_t)( (bool)water.is_drop_water_status );//漏水
            g_data_format.data[i++] = (uint8_t)( (bool)water.is_power_on_status );//开关机
            g_data_format.data[i++] = (uint8_t)( (bool)water.is_shortage_water_status );//缺水
            g_data_format.length = i;

            struct_to_buf(total_buf, &total_len, g_data_format);
            vSerialPutBytes( NULL, total_buf, total_len );
        }
    }
    return ret;
}

//-------------------------------------------------------------

void EDP_SendPacketPing(void) //0x01,//心跳帧
{
    if (edp.is_tcpip_connect)
    {
        uint8_t total_buf[100+15] = {0};
        int total_len = 0;

        water.date_time = time(NULL);

        g_data_format.cmd = 0x01;
        int i = 0;
        uint32_t remain_flow = (uint32_t)(water.save.remain_flow * 0.917 + 0.5);
        uint32_t used_flow = (uint32_t)(water.save.used_flow * 0.917 + 0.5);

        g_data_format.data[i++] = (uint8_t)(water.device_status);//设备状态
        g_data_format.data[i++] = (uint8_t)(water.is_screen_status_off);//屏幕状态
        g_data_format.data[i++] = (uint8_t)(water.save.work_mode);//工作模式
        g_data_format.data[i++] = (uint8_t)(remain_flow >> 24);//剩余流量
        g_data_format.data[i++] = (uint8_t)(remain_flow >> 16);//剩余流量
        g_data_format.data[i++] = (uint8_t)(remain_flow >> 8);//剩余流量
        g_data_format.data[i++] = (uint8_t)(remain_flow >> 0);//剩余流量
        g_data_format.data[i++] = (uint8_t)(water.remain_days >> 8);//剩余天数
        g_data_format.data[i++] = (uint8_t)(water.remain_days >> 0);//剩余天数
        g_data_format.data[i++] = (uint8_t)(used_flow >> 24);//已用流量
        g_data_format.data[i++] = (uint8_t)(used_flow >> 16);//已用流量
        g_data_format.data[i++] = (uint8_t)(used_flow >> 8);//已用流量
        g_data_format.data[i++] = (uint8_t)(used_flow >> 0);//已用流量
        g_data_format.data[i++] = (uint8_t)(water.used_days >> 8);//已用天数
        g_data_format.data[i++] = (uint8_t)(water.used_days >> 0);//已用天数
        if (water.save.clean_water_tds_switch == 0)//0显示默认值，1显示实时值
        {
            g_data_format.data[i++] = (uint8_t)(water.save.clean_water_ppm >> 8);//净水TDS
            g_data_format.data[i++] = (uint8_t)(water.save.clean_water_ppm >> 0);//净水TDS
        }
        else
        {
            g_data_format.data[i++] = (uint8_t)(water.clean_water_ppm >> 8);//净水TDS
            g_data_format.data[i++] = (uint8_t)(water.clean_water_ppm >> 0);//净水TDS
        }
        if (water.save.raw_water_tds_switch == 0)//0显示默认值，1显示实时值
        {
            g_data_format.data[i++] = (uint8_t)(water.save.raw_water_ppm >> 8);//原水TDS
            g_data_format.data[i++] = (uint8_t)(water.save.raw_water_ppm >> 0);//原水TDS
        }
        else
        {
            g_data_format.data[i++] = (uint8_t)(water.raw_water_ppm >> 8);//原水TDS
            g_data_format.data[i++] = (uint8_t)(water.raw_water_ppm >> 0);//原水TDS
        }
        g_data_format.data[i++] = (uint8_t)(water.rssi);//信号强度值
        g_data_format.data[i++] = (uint8_t)(water.lac_value >> 8);//LAC值
        g_data_format.data[i++] = (uint8_t)(water.lac_value >> 0);//LAC值
        g_data_format.data[i++] = (uint8_t)(water.cid_value >> 8);//CID值
        g_data_format.data[i++] = (uint8_t)(water.cid_value >> 0);//CID值
        g_data_format.data[i++] = 0;
        g_data_format.data[i++] = (uint8_t)(water.date_time >> 24);//数据时间
        g_data_format.data[i++] = (uint8_t)(water.date_time >> 16);//数据时间
        g_data_format.data[i++] = (uint8_t)(water.date_time >> 8);//数据时间
        g_data_format.data[i++] = (uint8_t)(water.date_time >> 0);//数据时间
        g_data_format.length = i;
        struct_to_buf(total_buf, &total_len, g_data_format);
        vSerialPutBytes( NULL, total_buf, total_len );
    }
}

//用水量上传
//每次水龙头出水10秒后上传本次用水量，单位/10ml。电脑板主动上传无需应答
void EDP_SendPacketWaterSync(uint32_t water_value) //0x0A,//用水同步
{
    if (edp.is_tcpip_connect)
    {
        uint8_t total_buf[100+15] = {0};
        int total_len = 0;

        water.date_time = time(NULL);

        g_data_format.cmd = 0x0A;
        int i = 0;
        g_data_format.data[i++] = (uint8_t)(water_value >> 24);
        g_data_format.data[i++] = (uint8_t)(water_value >> 16);
        g_data_format.data[i++] = (uint8_t)(water_value >> 8);
        g_data_format.data[i++] = (uint8_t)(water_value >> 0);
        g_data_format.data[i++] = 0;
        g_data_format.data[i++] = (uint8_t)(water.date_time >> 24);//数据时间
        g_data_format.data[i++] = (uint8_t)(water.date_time >> 16);//数据时间
        g_data_format.data[i++] = (uint8_t)(water.date_time >> 8);//数据时间
        g_data_format.data[i++] = (uint8_t)(water.date_time >> 0);//数据时间
        g_data_format.length = i;
        struct_to_buf(total_buf, &total_len, g_data_format);
        vSerialPutBytes( NULL, total_buf, total_len );
    }
}

//工作状态同步
//设备运行状态更新时，主动上传此消息。电脑板主动上传无需应答
/*
0x00	未制水
0x01	正在制水
0x00	未水满
0x01	水满
0x00	正常
0x01	检修
0x00	未漏水
0x01	漏水
0x00	关机
0x01	开机
0x00	未缺水
0x01	缺水
*/


bool SendPacketStatusSuccess = false;
void EDP_SendPacketStatus( void )//0x0C,//工作状态同步
{
    if (edp.is_tcpip_connect )
    {
        uint8_t total_buf[100+15] = {0};
        int total_len = 0;

        g_data_format.cmd = 0x0C;
        int i = 0;
        g_data_format.data[i++] = (uint8_t)( (bool)water.is_make_water_status );//制水状态
        g_data_format.data[i++] = (uint8_t)( (bool)water.is_full_water_status );//水满状态
        g_data_format.data[i++] = (uint8_t)( (bool)water.is_overhaul_status );//检修
        g_data_format.data[i++] = (uint8_t)( (bool)water.is_drop_water_status );//漏水
        g_data_format.data[i++] = (uint8_t)( (bool)water.is_power_on_status );//开关机
        g_data_format.data[i++] = (uint8_t)( (bool)water.is_shortage_water_status );//缺水
        g_data_format.length = i;

        SendPacketStatusSuccess = false;
        struct_to_buf(total_buf, &total_len, g_data_format);
        for (int j = 0; j < 5; j++)
        {
            vSerialPutBytes( NULL, total_buf, total_len );
            vTaskDelay( pdMS_TO_TICKS(2000) );
            if (SendPacketStatusSuccess == true)
                break;
        }
    }
}

//滤芯状态上传
void EDP_SendFilter(void)
{
    if (edp.is_tcpip_connect)
    {
        uint8_t total_buf[100+15] = {0};
        int total_len = 0;

        water.date_time = time(NULL);

        g_data_format.cmd = 0x09;
        int i = 0;
        g_data_format.data[i++] = (uint8_t)(water.filter_remain_value_1 >> 8);//第1级滤芯剩余值
        g_data_format.data[i++] = (uint8_t)(water.filter_remain_value_1 >> 0);//第1级滤芯剩余值
        g_data_format.data[i++] = (uint8_t)(water.filter_remain_value_2 >> 8);//第2级滤芯剩余值
        g_data_format.data[i++] = (uint8_t)(water.filter_remain_value_2 >> 0);//第2级滤芯剩余值
        g_data_format.data[i++] = (uint8_t)(water.filter_remain_value_3 >> 8);//第3级滤芯剩余值
        g_data_format.data[i++] = (uint8_t)(water.filter_remain_value_3 >> 0);//第3级滤芯剩余值
        g_data_format.data[i++] = (uint8_t)(water.filter_remain_value_4 >> 8);//第4级滤芯剩余值
        g_data_format.data[i++] = (uint8_t)(water.filter_remain_value_4 >> 0);//第4级滤芯剩余值
        g_data_format.data[i++] = (uint8_t)(water.filter_remain_value_5 >> 8);//第5级滤芯剩余值
        g_data_format.data[i++] = (uint8_t)(water.filter_remain_value_5 >> 0);//第5级滤芯剩余值
        g_data_format.data[i++] = (uint8_t)(water.save.filter_max_value_1 >> 8);//第1级滤芯最大值
        g_data_format.data[i++] = (uint8_t)(water.save.filter_max_value_1 >> 0);//第1级滤芯最大值
        g_data_format.data[i++] = (uint8_t)(water.save.filter_max_value_2 >> 8);//第2级滤芯最大值
        g_data_format.data[i++] = (uint8_t)(water.save.filter_max_value_2 >> 0);//第2级滤芯最大值
        g_data_format.data[i++] = (uint8_t)(water.save.filter_max_value_3 >> 8);//第3级滤芯最大值
        g_data_format.data[i++] = (uint8_t)(water.save.filter_max_value_3 >> 0);//第3级滤芯最大值
        g_data_format.data[i++] = (uint8_t)(water.save.filter_max_value_4 >> 8);//第4级滤芯最大值
        g_data_format.data[i++] = (uint8_t)(water.save.filter_max_value_4 >> 0);//第4级滤芯最大值
        g_data_format.data[i++] = (uint8_t)(water.save.filter_max_value_5 >> 8);//第5级滤芯最大值
        g_data_format.data[i++] = (uint8_t)(water.save.filter_max_value_5 >> 0);//第5级滤芯最大值
        g_data_format.data[i++] = 0;
        g_data_format.data[i++] = (uint8_t)(water.date_time >> 24);//数据时间
        g_data_format.data[i++] = (uint8_t)(water.date_time >> 16);//数据时间
        g_data_format.data[i++] = (uint8_t)(water.date_time >> 8);//数据时间
        g_data_format.data[i++] = (uint8_t)(water.date_time >> 0);//数据时间
        g_data_format.length = i;
        struct_to_buf(total_buf, &total_len, g_data_format);
        vSerialPutBytes( NULL, total_buf, total_len );
    }
}

int buf_to_struct(data_format_t *data_format, uint8_t *buf, int buf_len)
{
    if (buf_len <= 15)
        return 0;
    memcpy(&data_format->iccid[0], &buf[ICCID_ADDR_START], sizeof(data_format->iccid) / sizeof(data_format->iccid[0]));
    data_format->cmd = buf[CMD_ADDR_START];
    data_format->length = (buf[LENGTH_ADDR_START] << 8) + buf[LENGTH_ADDR_START+1];
    if (data_format->length > 0)
        memcpy(&data_format->data[0], &buf[DATA_ADDR_START], data_format->length);
    data_format->crc[0] = buf[DATA_ADDR_START+data_format->length];
    data_format->crc[1] = buf[DATA_ADDR_START+data_format->length+1];
    uint16_t crc16 = CRC16_MODBUS(buf, buf_len-2);
    if (crc16 != (((uint16_t)data_format->crc[0] << 8) + (uint16_t)data_format->crc[1]))
        return 0;
    else
        return 1;
}

int struct_to_buf(uint8_t *buf, int *buf_len, data_format_t data_format)
{
    memcpy(&buf[ICCID_ADDR_START], &data_format.iccid[0], sizeof(data_format.iccid) / sizeof(data_format.iccid[0]));
    buf[CMD_ADDR_START] = data_format.cmd;
    buf[LENGTH_ADDR_START] = (uint8_t)(data_format.length >> 8);
    buf[LENGTH_ADDR_START+1] = (uint8_t)(data_format.length >> 0);
    memcpy(&buf[DATA_ADDR_START], &data_format.data[0], data_format.length);
    *buf_len = DATA_ADDR_START + data_format.length + 2;
    uint16_t crc16 = CRC16_MODBUS(buf, *buf_len-2);
    data_format.crc[0] = (uint8_t)(crc16 >> 8);
    data_format.crc[1] = (uint8_t)(crc16 >> 0);
    buf[DATA_ADDR_START+data_format.length] = data_format.crc[0];
    buf[DATA_ADDR_START+data_format.length+1] = data_format.crc[1];
    return 1;
}

int ProtocolProcess(uint8_t* buf, int len)
{
    int out_len = 0;
    int resp = 0;

    uint8_t cmd = buf[CMD_ADDR_START];
    uint16_t in_len = (buf[LENGTH_ADDR_START] << 8) + buf[LENGTH_ADDR_START+1];
    uint16_t crc16_get = (buf[DATA_ADDR_START+in_len] << 8) + buf[DATA_ADDR_START+in_len+1];
    uint16_t crc16_calc = CRC16_MODBUS(buf, len-2);
    if (crc16_calc != crc16_get)
        return 0;

    for (int i = 0; i < cmd_process_cnt; i++)
    {
        if (cmd == cmd_process[i].cmd)
        {
            //printf("cmd:%02x\r\n", data_format.cmd);
            if ((cmd != CMD_HEARTBEAT) || (cmd != CMD_REMOTE_UPGRADE_DATA1))
                BEEP_ON;
            resp = cmd_process[i].cb(&buf[DATA_ADDR_START], &out_len, &buf[DATA_ADDR_START], in_len);
            if ((cmd != CMD_HEARTBEAT) || (cmd != CMD_REMOTE_UPGRADE_DATA1))
            {
                I2C_EE_BufferWriteSafe(0, (uint8_t *)&water.save, sizeof(save_data_default));
                BEEP_OFF;
                I2C_EE_BufferRead(0, (uint8_t *)&water.save, sizeof(save_data_default));
            }
            break;
        }
    }
    if (0 != resp)
    {
        int total_len = 0;
        total_len = DATA_ADDR_START + out_len + 2;
        buf[LENGTH_ADDR_START] = (uint8_t)(out_len >> 8);
        buf[LENGTH_ADDR_START+1] = (uint8_t)(out_len >> 0);
        uint16_t crc16 = CRC16_MODBUS(buf, total_len-2);
        buf[total_len-2] = (uint8_t)(crc16 >> 8);
        buf[total_len-1] = (uint8_t)(crc16 >> 0);

        vSerialPutBytes( NULL, buf, total_len );

        if (cmd == CMD_REMOTE_UPGRADE_REBOOT)//重启并运行需要运行的程序
        {
            vTaskDelay( pdMS_TO_TICKS(1000) );
            NVIC_SystemReset();
        }
    }

    return 0;
}


