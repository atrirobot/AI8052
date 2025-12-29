#include "zf_common_headfile.h"

uint8 rx_buffer[128]; 
uint32 rx_len = 0;
float total_distance=10.0f;
void PID_Calc(PID_Type *pid) {
    pid->err = pid->target_val - pid->actual_val;
    pid->integral += pid->err;
    pid->output = (pid->Kp * pid->err) + (pid->Ki * pid->integral) + (pid->Kd * (pid->err - pid->last_err));
    pid->last_err = pid->err;
    pid->actual_val += pid->output * 0.05f; 
}
float a=0;
void main(void)
{
    uint32 loop_count = 0;
    clock_init(SYSTEM_CLOCK_108M); 				
    debug_init();
//	pit_ms_init(TIMER0_IRQn,5);	
//	interrupt_set_priority(TIMER0_IRQn,0);
	gpio_init(IO_P52, GPO, 1, GPO_PUSH_PULL);
	gpio_set_level(IO_P52, 0);
    printf("Wait Connect...\n");
    while(wifi_spi_init("Ciallo", "0d000721") != 0) {
        system_delay_ms(1000); 
    }
    printf("Success!\n");
    while(1)
    {
        // 1. 接收数据
        rx_len = wifi_spi_read_buffer(rx_buffer, 127);
        if(rx_len > 0)
        {
            rx_buffer[rx_len] = 0; 
            printf("RX: %s\n", rx_buffer);
            if(my_strstr((char*)rx_buffer, "SET_PID")) {
                Parse_PID_Command((char*)rx_buffer);
            }
            else if(my_strstr((char*)rx_buffer, "CMD_LED")) {
                gpio_set_level(IO_P52, !gpio_get_level(IO_P52));
                Send_Text("LED Toggled\n");
            }
        }
        // 2. PID 仿真
        PID_Calc(&steer_loop);
        // 3. 定时发送状态 (每 200ms)
        loop_count++;
        if(loop_count % 20 == 0) {
			char log_msg[64];
			a++;
			total_distance++;
			speed_loop.actual_val+=0.1f;
            loop_count = 0;  
            //sprintf(log_msg, "Steer P:%.2f I:%.2f D:%.2f\n", steer_loop.Kp, steer_loop.Ki, steer_loop.Kd);
            //wifi_spi_send_string(log_msg);
			sprintf(log_msg, "DAT:%.1f:%d:%.1f:Running\n", speed_loop.actual_val,(int)total_distance,a);
			Send_Text(log_msg);
        }
		Send_Image((uint8*)image_data,0); // ★ 发送图片
		if(loop_count > 1000) loop_count = 0;
        system_delay_ms(100); 
    }
}