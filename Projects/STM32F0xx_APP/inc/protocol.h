#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

typedef enum {
    CMD_HEARTBEAT = 0X01,//心跳帧
    CMD_BINDING_PACKAGE = 0X02,//绑定套餐
    CMD_CLOSE_THE_SCREEN = 0X03,//关闭屏幕
    CMD_OPEN_THE_SCREEN = 0X04,//打开屏幕
    CMD_POWER_OFF = 0X05,//关机
    CMD_POWER_ON = 0X06,//开机
    CMD_STRONG_PUNCH = 0X07,//强冲
    CMD_POSITIVE_VALUE = 0X08,//充正值
    
    CMD_FILTER_SEND = 0x09,//用户设备主动上报滤芯状态，每天上报一次的方式
    CMD_FILTER_REQUIRE = 0x19,//主动获取滤芯状态
    
    CMD_SYNCHRONIZE_WITH_WATER = 0X0A,//用水同步
    //CMD_TIME_SYNCHRONIZATION = 0X0B,//用时同步
    CMD_TIME_SYNCHRONIZATION_USED = 0XBA,//用时同步 0xBA已用天数
    CMD_TIME_SYNCHRONIZATION_REMAIN = 0XBB,//用时同步 0xBB剩余天数
    CMD_WORK_STATUS_SYNCHRONIZATION = 0X0C,//工作状态同步
    CMD_QUERY_DEVICE_OPERATION_INFORMATION = 0X0D,//查询设备运行信息
    CMD_FILTER_RESET_AND_MODIFICATION = 0X0E,//滤芯复位与修改
    CMD_MODE_SWITCHING = 0X0F,//模式切换
    CMD_RESET = 0X10,//恢复出厂设置
    //修改域名和端口号
    CMD_PARAMETER_MODIFICATION = 0X11,//参数修改
    CMD_TIMING_FLUSH_PARAMETER_MODIFICATION = 0X12,//定时冲洗参数修改
    CMD_MAINTENANCE_PARAMETER_MODIFICATION = 0X13,//检修参数修改
    CMD_CONTROL_PARAMETER_MODIFICATION_1 = 0X14,//控制参数修改1
    CMD_CONTROL_PARAMETER_MODIFICATION_2 = 0X15,//控制参数修改2
    CMD_TEST_MODE_SWITCH = 0X16,//开启关闭测试模式
    CMD_COMPUTER_BOARD_TIME_SYNCHRONIZATION_1 = 0x17,// 电脑板时间同步1
    CMD_COMPUTER_BOARD_TIME_SYNCHRONIZATION_2 = 0x18,// 电脑板时间同步2
    CMD_SYNCHRONIZATION_OF_WATER_CONSUMPTION_USED = 0xAA,// 用水量同步 0xAA已用流量
    CMD_SYNCHRONIZATION_OF_WATER_CONSUMPTION_REMAIN = 0xAB,// 用水量同步 0xAB剩余流量
    CMD_REMOTE_UPGRADE_VERSION = 0x1A,//服务端获取固件当前版本号
    CMD_REMOTE_UPGRADE_DATA0 = 0x2A,//远程升级文件名
    CMD_REMOTE_UPGRADE_DATA1 = 0x3A,//远程升级文件
    CMD_REMOTE_UPGRADE_DATA2 = 0x4A,//远程升级结束
    CMD_REMOTE_UPGRADE_REBOOT = 0x7A,//远程重启
    CMD_REMOTE_UPGRADE_NEW = 0x8A,//下次启动设备时是否运行新固件
    CMD_REMOTE_UPGRADE_SWITCH = 0x9A,//运行固件切换
    CMD_SCREEN_MODE_SWITCH = 0x5A,//屏幕显示模式切换0x5A
    CMD_OVERHAUL_STATUS_SWITCH = 0x6A//检修状态切换0x6A
}cmd_type_t;

typedef int (*cmd_process_cb)(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);

typedef struct{
    cmd_type_t cmd;
    cmd_process_cb cb;
}cmd_process_t;

extern int cmd_heartbeat_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x01,//心跳帧
extern int cmd_binding_package_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x02,//绑定套餐
extern int cmd_close_the_screen_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x03,//关闭屏幕
extern int cmd_open_the_screen_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x04,//打开屏幕
extern int cmd_power_off_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x05,//关机
extern int cmd_power_on_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x06,//开机
extern int cmd_strong_push_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x07,//强冲
extern int cmd_positive_value_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x08,//充正值

extern int cmd_filter_send_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//用户设备主动上报滤芯状态，每天上报一次的方式
extern int cmd_filter_require_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//主动获取滤芯状态

extern int cmd_synchronize_with_water_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x0a,//用水同步
//extern int cmd_time_synchronization_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x0b,//用时同步
extern int cmd_time_synchronization_used_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//用时同步 0xBA已用天数
extern int cmd_time_synchronization_remain_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//用时同步 0xBB剩余天数
extern int cmd_work_status_synchronization_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x0c,//工作状态同步
extern int cmd_query_device_operation_information_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x0d,//查询设备运行信息
extern int cmd_filter_reset_and_modification_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x0e,//滤芯复位与修改
extern int cmd_mode_switching_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x0f,//模式切换
extern int cmd_reset_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x10,//恢复出厂设置
//修改域名和端口号
extern int cmd_parameter_modification_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x11,//参数修改
extern int cmd_timing_flush_parameter_modification_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x12,//定时冲洗参数修改
extern int cmd_maintenance_parameter_modification_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x13,//检修参数修改
extern int cmd_control_parameter_modification_1_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x14,//控制参数修改1
extern int cmd_control_parameter_modification_2_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x15,//控制参数修改2
extern int cmd_test_mode_switch_process(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x16,//开启关闭测试模式
extern int cmd_computer_board_time_synchronization_1(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x17,// 电脑板时间同步1
extern int cmd_computer_board_time_synchronization_2(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x18,// 电脑板时间同步2
extern int cmd_synchronization_of_water_consumption_used(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0xAA,// 用水量同步 0xAA已用流量
extern int cmd_synchronization_of_water_consumption_remain(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0xAB,// 用水量同步 0xAB剩余流量
extern int cmd_remote_upgrade_version(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x1A,//服务端获取固件当前版本号
extern int cmd_remote_upgrade_data0(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x2A,//远程升级文件名
extern int cmd_remote_upgrade_data1(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x3A,//远程升级文件
extern int cmd_remote_upgrade_data2(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x4A,//远程升级结束
extern int cmd_remote_upgrade_reboot(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x7A,//远程重启
extern int cmd_remote_upgrade_new(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x8A,//下次启动设备时是否运行新固件
extern int cmd_remote_upgrade_switch(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x9A,//33、运行固件切换
extern int cmd_screen_mode_switch(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x5A,//屏幕显示模式切换
extern int cmd_overhaul_status_switch(uint8_t* out_buf, int* out_len, uint8_t* in_buf, int in_len);//0x6A,//检修状态切换0x6A

struct screen_mode_switch_t{
    uint8_t day;//0x00	已用天数   0x01	剩余天数
    uint8_t flow;//0x02	已用流量   0x03	剩余流量
};



struct save_data{
    int e2prom_init;
    
    uint8_t is_bind;
    time_t bind_timestamp;//记录绑定那天的时间戳
    
    //工作模式	        HEX（1字节）
    /*
    0	流量模式
    1	时长模式
    */
    uint8_t work_mode;
    
    //已用流量	        HEX（4字节）
    uint32_t used_flow;
    
    //剩余流量	        HEX（4字节）
    uint32_t remain_flow;
    
    //已用天数时间戳
    time_t used_days_timestamp;//用于计算已经使用的天数，（当前时间搓-这里记录的时间搓）/ (3600*24)
    
    //剩余天数时间戳
    time_t remain_days_timestamp;//记录还有多少时间搓可以使用
    
    //净水TDS	            HEX（2字节）
    uint16_t clean_water_ppm;//服务器加下的假的ppm值
    uint8_t clean_water_tds_switch;//0显示默认值，1显示实时值
    
    //原水TDS	            HEX（2字节）
    uint16_t raw_water_ppm;//服务器加下的假的ppm值
    uint8_t raw_water_tds_switch;//0显示默认值，1显示实时值
    
    //第 1 级滤芯的开始使用时间戳
    time_t filter_used_timestamp_1;
    
    //第 2 级滤芯的开始使用时间戳
    time_t filter_used_timestamp_2;
    
    //第 3 级滤芯的开始使用时间戳
    time_t filter_used_timestamp_3;
    
    //第 4 级滤芯的开始使用时间戳
    time_t filter_used_timestamp_4;
    
    //第 5 级滤芯的开始使用时间戳
    time_t filter_used_timestamp_5;
    
    //第一级滤芯最大值	HEX（2字节）
    uint16_t filter_max_value_1;
    
    //第二级滤芯最大值	HEX（2字节）
    uint16_t filter_max_value_2;
    
    //第三级滤芯最大值	HEX（2字节）
    uint16_t filter_max_value_3;
    
    //第四级滤芯最大值	HEX（2字节）
    uint16_t filter_max_value_4;
    
    //第五级滤芯最大值	HEX（2字节）
    uint16_t filter_max_value_5;
    
    uint8_t force_flush_time;//单位：秒
    uint32_t period_flush_time;//秒
    uint32_t maintenance_time;//单位：秒
    
    uint16_t ping_time;//单位：秒
    uint16_t reconnect_time;//单位：秒
    
    struct screen_mode_switch_t screen_mode_switch;
    
};

    
//高字节在前，低字节在后
struct water_s{
//设备状态	        HEX（1字节）
/*
0	备用
1	待激活
2	未注册
3	欠费
4	滤芯待复位
5	硬件测试
*/
uint8_t device_status;

//屏幕状态	        HEX（1字节）
/*
0	屏幕打开（上电默认值）
1	屏幕关闭
*/
uint8_t is_screen_status_off;

//已用天数	        HEX（2字节）
uint16_t used_days;

//剩余天数	        HEX（2字节）
uint16_t remain_days;
    
//第一级滤芯剩余值	HEX（2字节）
uint16_t filter_remain_value_1;

//第二级滤芯剩余值	HEX（2字节）
uint16_t filter_remain_value_2;

//第三级滤芯剩余值	HEX（2字节）
uint16_t filter_remain_value_3;

//第四级滤芯剩余值	HEX（2字节）
uint16_t filter_remain_value_4;

//第五级滤芯剩余值	HEX（2字节）
uint16_t filter_remain_value_5;

//信号强度值	        HEX（1字节）
uint8_t rssi;

//LAC值	            HEX（2字节）
uint16_t lac_value;

//CID值	            HEX（2字节）
uint16_t cid_value;

//数据时间          HEX（4字节）
uint32_t date_time;

//其他一些值
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
uint8_t is_make_water_status;
uint8_t is_full_water_status;
uint8_t is_overhaul_status;
uint8_t is_drop_water_status;
uint8_t is_power_on_status;
uint8_t is_shortage_water_status;

uint8_t is_bushing_now;
uint8_t is_overhaul_beep;

uint8_t is_test_mode_switch_off;//0x00  开启 0x01  关闭
uint16_t test_used_days;
uint16_t test_used_flow;

uint32_t flowmeter_count;//流量计记脉冲

uint16_t raw_water_ppm;//实际检测到的ppm值

uint16_t clean_water_ppm;//实际检测到的ppm值

//检修状态切换0x6A
uint8_t overhaul_status_switch;//0x00解除检修  //0x01进入检修
    
int io_high_status;//记录IO1状态,高压开关状态
int io_high_last_status;
int io_low_status;//记录IO2状态,低压开关状态
int io_low_last_status;
    
//需要存储的值
struct save_data save;
};

extern struct water_s water;
extern const struct save_data save_data_default;

#define ICCID_ADDR_START        0
#define CMD_ADDR_START          10
#define LENGTH_ADDR_START       11
#define DATA_ADDR_START         13

#define APP_O_OFFSET_ADDR       (0)
#define APP_A_OFFSET_ADDR       (10)
#define APP_B_OFFSET_ADDR       (20)

typedef struct{
    uint8_t iccid[10];
    uint8_t cmd;
    uint16_t length;
    uint8_t data[100];//变长，根据具体功能决定
    uint8_t crc[2];
}data_format_t;

extern data_format_t g_data_format;

void EDP_SendPacketPing( void );

int buf_to_struct(data_format_t *data_format, uint8_t *buf, int buf_len);
int struct_to_buf(uint8_t *buf, int *buf_len, data_format_t data_format);

int ProtocolProcess(uint8_t* buf, int len);

#endif
