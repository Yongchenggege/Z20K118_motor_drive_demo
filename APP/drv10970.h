#ifndef __DRV10970_H_
#define __DRV10970_H_

#include "Z20K118_conf.h"
#include "pwm.h"
#include "uart.h"
#include "enconder.h"

// FREQ = 72000000/(TIM_PER*PULSE_MAX)
#define TIM_PER			5
#define PULSE_MAX		7200

// 电机的旋转一圈的脉冲数(这里需要根据你的电机资料去改)
#define MOTOR_PULSE	11

// 速度测量间隔(取值为0-65535 对的时间为x*0.0001 这里取值2000 对应是2000*0.0001=0.2 (s) 建议取值在 500-10000 )
#define SPEED_INTERVAL	2000

// 齿轮电机减速比9.6 电机转速620 最终空载转速620/9.6=64.6 霍尔反馈一圈11个脉冲



typedef enum{
	motor1,
	motor2

}motor_type;

//FR
 typedef enum {
	reverse,
	forward

}fr_dir;
 
void fr_control_dir(fr_dir dir1,fr_dir dir2);
void drv10970_init(void);

//FG
float get_motor_speed(motor_type motor);



#endif
