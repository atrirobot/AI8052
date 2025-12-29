#ifndef _img_h
#define _img_h
#include "zf_common_headfile.h"
typedef struct {
    float Kp;
    float Ki;
    float Kd;
    
    float target_val;
    float actual_val;
    float err;
    float last_err;
    float integral;
    float output;
} PID_Type;
extern PID_Type steer_loop;
extern PID_Type speed_loop;
extern unsigned char xdata image_data[60][94];
void Send_Text(char* str);
void Send_Image(uint8* image_data,uint8 type);
#endif
