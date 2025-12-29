#include "cmd_parse.h"

// ================= 解析 PID 指令 =================
// 格式: SET_PID:STEER:1.5:0.01:0.5
void Parse_PID_Command(char* msg)
{
    char *ptr = msg;
    float p = 0, i = 0, d = 0;
    PID_Type *target_pid = NULL;
    while(*ptr && *ptr != ':') ptr++; 
    if(*ptr == ':') ptr++; // 跳过第一个冒号
    if(ptr[0] == 'S' && ptr[1] == 'T' && ptr[2] == 'E') target_pid = &steer_loop;
    else if(ptr[0] == 'S' && ptr[1] == 'P' && ptr[2] == 'E') target_pid = &speed_loop;
    else return;
    while(*ptr && *ptr != ':') ptr++;
    if(*ptr == ':') ptr++; // 此时 ptr 指向 P 的值
    // 4. 解析 P
    p = my_atof(ptr);
    // 移到下一个冒号
    while(*ptr && *ptr != ':') ptr++;
    if(*ptr == ':') ptr++; // 此时 ptr 指向 I 的值
    // 5. 解析 I
    i = my_atof(ptr);
    // 移到下一个冒号
    while(*ptr && *ptr != ':') ptr++;
    if(*ptr == ':') ptr++; // 此时 ptr 指向 D 的值
    // 6. 解析 D
    d = my_atof(ptr);
    // 7. 应用参数并反馈
    if(target_pid != NULL) {
		char reply[64];
        target_pid->Kp = p;
        target_pid->Ki = i;
        target_pid->Kd = d;
        // 发送调试信息回手机，确认收到
        // 注意：printf 是发给串口助手的，wifi_spi_send_string 是发给手机APP的
        printf("Set Success: P=%.3f I=%.3f D=%.3f\n", p, i, d);
        // 构造反馈字符串发回手机
        sprintf(reply, "Update OK! P:%.3f I:%.3f D:%.3f\n", p, i, d);
        wifi_spi_send_string(reply); 
    }
}