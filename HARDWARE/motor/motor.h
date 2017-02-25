/*
 * SlamTec Base Ref Design
 * Copyright 2009 - 2015 RoboPeak
 * Copyright 2013 - 2015 Shanghai SlamTec Co., Ltd.
 * http://www.slamtec.com
 * All rights reserved.
 */
/*
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"
#include "common.h"
#include "rpstm32.h"
#include "user_config.h"

#define PWM_MAX (10000)



#define DELTA_L_SPEED	0
#define DELTA_R_SPEED	00


//左右行走电机管脚定义

#define MOTOR_L_EN		GPIO_Pin_5
#define MOTOR_L_FR		GPIO_Pin_4
#define MOTOR_L_BRK		GPIO_Pin_8	

#define MOTOR_R_EN		GPIO_Pin_0
#define MOTOR_R_FR		GPIO_Pin_9	
#define MOTOR_R_BRK		GPIO_Pin_1

#define MOTO_L_PWM_ID   3
#define MOTO_L_PWM_CHN  2
#define MOTO_L_PWM      (GET_TIM(MOTO_L_PWM_ID))

             

#define MOTO_R_PWM_CHN  1
#define MOTO_R_PWM_ID   3
#define MOTO_R_PWM      (GET_TIM(MOTO_R_PWM_ID))

#define MOTO_L_PWM_GPIO_PORT  GPIOA
#define MOTO_L_PWM_GPIO_PIN   GPIO_Pin_7

#define MOTO_R_PWM_GPIO_PORT  GPIOA
#define MOTO_R_PWM_GPIO_PIN   GPIO_Pin_6

//每米编码器脉冲数
#define ODOMETER_EST_PULSE_PER_METER  191UL
#define ODOMETER_GPIO       GPIOB
#define ODOMETER_LEFT       GPIO_Pin_6
#define ODOMETER_RIGHT      GPIO_Pin_7


//行走电机速度控制频率：60hz
#define CONF_MOTOR_HEARTBEAT_FREQ     20
#define CONF_MOTOR_HEARTBEAT_DURATION (1000/(CONF_MOTOR_HEARTBEAT_FREQ))

enum motorCtrlState_t
{
    MOTOR_CTRL_STATE_RELEASE = 0,
    MOTOR_CTRL_STATE_FORWARD = 1,
    MOTOR_CTRL_STATE_BACKWARD = 2,
    MOTOR_CTRL_STATE_BRAKE = 3,
};

void init_walkingmotor(void);
void set_walkingmotor_lduty(_s32 lDuty, _s32 ctrl);
void set_walkingmotor_rduty(_s32 rDuty, _s32 ctrl);
void init_walkingmotor_odometer(void);
void control_walkingmotor_speed(void);
void set_walkingmotor_speed(_s32 lSpeed, _s32 rSpeed);
_u32 get_walkingmotor_lspeed_mm_q16(void);
_u32 get_walkingmotor_rspeed_mm_q16(void);
_s32 get_walkingmotor_lspeed_set(void);
_s32 get_walkingmotor_rspeed_set(void);
void wait_walkingmotor_brake(void);
void brake_walkingmotor(void);
_u32 walkingmotor_cumulate_ldist_mm(void);
_u32 walkingmotor_cumulate_rdist_mm(void);
float walkingmotor_delta_ldist_mm_f(void);
float walkingmotor_delta_rdist_mm_f(void);

void init_ontheground_detect(void);
_u8 is_ontheground(void);
void init_brushmotor(void);
void speedctl_heartbeat(_u32 durationMs);

#endif
