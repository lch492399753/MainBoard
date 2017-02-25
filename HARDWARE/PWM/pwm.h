#ifndef PWM_H
#define PWM_H

#include "stm32f10x.h"
 
#define START_TIME  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);TIM_Cmd(TIM2, ENABLE)
//#define STOP_TIME  TIM_Cmd(TIM2, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE)
#define EXTI_Enable  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
#define EXTI_Disable  EXTI_InitStructure.EXTI_LineCmd = DISABLE;

#define LeftMotorID	0
#define RightMotorID 1
#define AutoMotorID 3
//void TIM2_NVIC_Configuration(void);
//void TIM2_Configuration(void);
//void TIM3_Int_Init1(u16 arr,u16 psc);



extern uint8_t g_u8ObstFlg;

void TIM2_Init(u16 arr,u16 psc);
void TIM4_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);

extern u32 curr_speed_left;
extern u32 curr_speed_right;	

extern u32 curr_cycle_left;	
extern u32 curr_cycle_right;	
extern uint8_t g_u8ObstCount;
extern u32 pre_cycle_left;	//捕获总高电平的时间
extern u32 pre_cycle_right ;	//捕获总高电平的时间
extern uint16_t g_u16SpeedChangeFlg;

extern int16_t g_S8SpeedLTarget;
extern int16_t g_S8SpeedRTarget;
extern int16_t g_S8SpeedAutoTarget;
extern uint8_t g_u8StopFlg;
extern int16_t g_S8SpeedLCurr;
extern int16_t g_S8SpeedRCurr;
extern int16_t g_S8SpeedAutoCurr;

extern uint16_t g_u16RunSpeed;
extern uint8_t g_u8SpeedDelta;
void   TIM4_Cap_Init(u16 arr, u16 psc);
extern s16 SPEEDL;
extern s16 SPEEDR;
extern u32 curr_left[10];
extern u32 curr_right[10];
extern uint16_t g_u16ObstVoice;

extern __IO uint32_t g_u32CaptureNumberL;
extern __IO uint32_t g_u32CaptureNumberR;

void SpeedLCover(int16_t Speed,int16_t *SetSpeed );
void SpeedRCover(int16_t Speed,int16_t *SetSpeed );
//TIM3_PWM_Init1
#endif	
