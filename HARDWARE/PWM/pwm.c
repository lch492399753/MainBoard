#include "pwm.h"
#include "usart.h"
#include "PID.h"
#include "gpio.h"
#include "Tracking.h"
#include "iFlytek.h"
#include "PoseWithVoice_Fun.h"
#include "motor.h"

s16 SPEEDL=0;
s16 SPEEDR=0;
double dp;
extern volatile u32 time,time_alive;
extern u32 stop;
extern u16 walk_time,walking1_time,walking2_time,agv_time,wifi_time,zhanhui_time;
extern u8 agv_flag,wifi_flag,position_flag;
u32 delay_time=0,position_time=0;

u8 TIM4CH1_CAPTURE_STA = 0; //通道1输入捕获标志，高两位做捕获标志，低6位做溢出标志
u16 TIM4CH1_CAPTURE_UPVAL;
u16 TIM4CH1_CAPTURE_DOWNVAL;

u8 TIM4CH2_CAPTURE_STA = 0; //通道2输入捕获标志，高两位做捕获标志，低6位做溢出标志
u16 TIM4CH2_CAPTURE_UPVAL;
u16 TIM4CH2_CAPTURE_DOWNVAL;

u8 L=0,R=0;
u32 curr_left[10];
u32 curr_right[10];

__IO uint32_t g_u32CaptureNumberL = 0;
__IO uint32_t g_u32CaptureNumberR = 0;
//u8  TIM4CH1_CAPTURE_STA=0;    //输入捕获状态




u8 g_u8SpeedDelta = 5;
u32 curr_cycle_left = 0;    //捕获总高电平的时间
u32 curr_cycle_right = 0;   //捕获总高电平的时间

u32 pre_cycle_left = 0; //捕获总高电平的时间
u32 pre_cycle_right = 0;    //捕获总高电平的时间

u32 tim4_T1;
u32 tim4_T2;


int pwmout1, pwmout2 ;              //输出占空比

void TIM2_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值     计数到5000为500ms
    TIM_TimeBaseStructure.TIM_Prescaler =(psc-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    TIM_Cmd(TIM2, DISABLE);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);
    TIM_ITConfig(  //使能或者失能指定的TIM中断
        TIM2, //TIM2
        TIM_IT_Update  |  //TIM 中断源
        TIM_IT_Trigger,   //TIM 触发中断源
        ENABLE  //使能
    );
   // RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);
}

void TouchValUpdate(void);
void TouchValUpdateHead(void);
uint16_t g_u16TouchTimer = 0;
void GuideWalking(uint8_t Speed);
uint16_t g_u16ObstTimeCount=0;
uint8_t g_u8ObstCount=0;
uint16_t g_u16SpeedChangeFlg=0;

int16_t g_S8SpeedLTarget = 0;
int16_t g_S8SpeedRTarget = 0;
int16_t g_S8SpeedAutoTarget = 0;

int16_t g_S8SpeedLCurr = 0;
int16_t g_S8SpeedRCurr = 0;
int16_t g_S8SpeedAutoCurr = 0;

uint8_t g_u8SpeedChangeTime = 0;
uint16_t g_u16ObstVoice = 0;
uint8_t g_u8MotorFeresh = 0;

void TIM2_IRQHandler(void)   //TIM3中断
{
	uint8_t tmp = 0;
    if( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET )
    {
       
        time++;

        g_u16TouchTimer++;
        g_u16ObstTimeCount++;
		g_u8SpeedChangeTime++;
		g_u8MotorFeresh++;

        if(g_u16ObstTimeCount >= 20)
        {
            g_u16ObstTimeCount=0;
            if(g_u8ObstFlg > 0)
            {
                g_u8ObstCount++;
				g_u16ObstVoice++;
                if(g_u8ObstCount>30)
                    g_u8ObstCount=30;
            }
            else
            {
                if( g_u8ObstCount>0)
                    g_u8ObstCount--;
				else
				{
					g_u8ObstCount=0;
				}
				g_u16ObstVoice = 0;
            }
        }
		
//		if(g_u8SpeedChangeTime >= 10)
//		{
//			g_u8SpeedChangeTime = 0;
//			//if(g_u8StopFlg == 0)
//			{
//				if(g_S8SpeedRTarget > g_S8SpeedRCurr)
//				{
//					
//					if( (g_S8SpeedRTarget - g_S8SpeedRCurr) <20)
//					{
//						g_S8SpeedRCurr ++;
//					}
//					else
//					{
//						g_S8SpeedRCurr += g_u8SpeedDelta;
//					}

//				}
//				else if(g_S8SpeedRTarget < g_S8SpeedRCurr)
//				{
//					if((g_S8SpeedRCurr - g_S8SpeedRTarget) <20 )
//					{
//						g_S8SpeedRCurr --;
//					}
//					else
//					{
//						g_S8SpeedRCurr -= g_u8SpeedDelta;
//					}
//					

//				}
//				else
//				{
//					g_S8SpeedRCurr = g_S8SpeedRTarget;

//				}
//				

//				if(g_S8SpeedLTarget > g_S8SpeedLCurr)
//				{
//					if( (g_S8SpeedLTarget - g_S8SpeedLCurr) <20)
//					{
//						g_S8SpeedLCurr ++;
//					}
//					else
//					{
//						g_S8SpeedLCurr += g_u8SpeedDelta;
//					}

//				}
//				else if(g_S8SpeedLTarget < g_S8SpeedLCurr)
//				{
//					if((g_S8SpeedLCurr - g_S8SpeedLTarget) <20 )
//					{
//						g_S8SpeedLCurr --;
//					}
//					else
//					{
//						g_S8SpeedLCurr -= g_u8SpeedDelta;
//					}

//				}
//				else
//				{
//					g_S8SpeedLCurr = g_S8SpeedLTarget;

//					//g_u8StopFlg =1;
//				}
//				
//				if(g_S8SpeedAutoTarget > g_S8SpeedAutoCurr)
//				{
//					g_S8SpeedAutoCurr +=1;
//				}
//				else if(g_S8SpeedAutoTarget < g_S8SpeedAutoCurr)
//				{
//					g_S8SpeedAutoCurr-=1;
//				}
//				else
//				{
//					g_S8SpeedAutoCurr = g_S8SpeedAutoTarget;
//					//g_u8StopFlg =1;
//				}
//				
//			}
//			
//		}
		
		if(g_u8MotorFeresh >= CONF_MOTOR_HEARTBEAT_DURATION)
		{
			speedctl_heartbeat(g_u8MotorFeresh);
			g_u8MotorFeresh = 0;
		}
		
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
        TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
    }
	
	
//	if(g_u16TouchTimer ==3)
//		CheckWakeUpSta();
//	if(g_u16TouchTimer ==4)
//		tmp = ReadWakeUpSta();
    if(g_u16TouchTimer > 5)
    {
		//tmp = ReadWakeUpSta();
//        TouchValUpdate();
		//if(tmp == 1)
//		{
//			g_u16iFlyTekAngel = ReadWakeUpAngel();
//		
//			if((g_u16iFlyTekAngel>0) && (g_u16iFlyTekAngel<360))
//			{
//				if(g_u16iFlyTekAngel != g_u16iFlyTekAngelPre)
//			{
//				g_u16iFlyTekAngelPre = g_u16iFlyTekAngel;
//				g_tVoiceCtl.u16WakeAng = g_u16iFlyTekAngel;
//				g_tVoiceCtl.u8IsWakeUp = 1;
//			}
//			}
//			
//		}
		//if(ReadWakeUpSta() == 1)
			
		
		g_u16TouchTimer = 0;
    }
    if(time>=10	)
    {
        time=0;
		
		if(g_u8AutoRunFlg == 1)
		{
			GuideWalking(70);
			
		} 
		else
		{
//			SPEEDL = g_S8SpeedLCurr ;
//			SPEEDR = g_S8SpeedRCurr ;
		}
		//SetSPeed(LeftMotorID,SPEEDL);
		//SetSPeed(RightMotorID,SPEEDR);
//		SetSPeed(LeftMotorID,SPEEDL);
//		SetSPeed(RightMotorID,SPEEDR);
		
		
    }
}

void TIM3_PWM_Init(u16 arr,u16 psc)
{


    GPIO_InitTypeDef    GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef   TIM_OCInitStructure;

    TIM_DeInit(TIM3);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);



    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler =psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);


    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = arr/2;
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);
//    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
//    TIM_OC4Init(TIM3, &TIM_OCInitStructure);


    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
//    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
//    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_Cmd(TIM3, ENABLE);
    TIM_CtrlPWMOutputs(TIM3, ENABLE);
}




TIM_ICInitTypeDef TIM4_ICInitStructure;

void TIM4_Cap_Init(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);    //使能TIM4时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIOB时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PB6,7,8,9 输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7 );//PB6,7,8,9  下拉

    //初始化定时器4 TIM4
    TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值
    TIM_TimeBaseStructure.TIM_Prescaler = psc;  //预分频器
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    //初始化TIM4输入捕获参数 通道1
    TIM4_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01     选择输入端 IC1映射到TI1上
    TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    //上升沿捕获
    TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
    TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;    //配置输入分频,不分频
    TIM4_ICInitStructure.TIM_ICFilter = 0x00;     //IC1F=0000 配置输入滤波器 不滤波
    TIM_ICInit(TIM4, &TIM4_ICInitStructure);

    //初始化TIM4输入捕获参数 通道2
    TIM4_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01     选择输入端 IC1映射到TI1上
    TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    //上升沿捕获
    TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
    TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;    //配置输入分频,不分频
    TIM4_ICInitStructure.TIM_ICFilter = 0x00;     //IC1F=0000 配置输入滤波器 不滤波
    TIM_ICInit(TIM4, &TIM4_ICInitStructure);



    //中断分组初始化
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级1级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);   //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    TIM_ITConfig(TIM4, TIM_IT_CC1 | TIM_IT_CC2 ,
                 ENABLE);   //不允许更新中断，允许CC1IE,CC2IE,CC3IE,CC4IE捕获中断

    TIM_Cmd(TIM4, ENABLE);      //使能定时器4

}


//定时器4中断服务程序
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)         //捕获1发生捕获事件
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);        //清除中断标志位
		g_u32CaptureNumberL++;
    }
	
	if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)         //捕获1发生捕获事件
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);        //清除中断标志位
		g_u32CaptureNumberR++;
    }

}

void SpeedLCover(int16_t Speed,int16_t *SetSpeed )
{
	if(Speed>0)
	{
		WALK_LEFT_FRONT();
		*SetSpeed = Speed;
	}
		
	else if(Speed<0)
	{
		WALK_LEFT_BACK();
		Speed = Speed * (-1);
		*SetSpeed = Speed;
	}
	else
	{
		WALK_LEFT_STOP();
	}
}

void SpeedRCover(int16_t Speed,int16_t *SetSpeed )
{
	if(Speed>0)
	{
		WALK_RIGHT_FRONT();
		*SetSpeed = Speed;
	}
		
	else if(Speed<0)
	{
		WALK_RIGHT_BACK();
		Speed = Speed * (-1);
		*SetSpeed = Speed;
	}
	else
	{
		WALK_RIGHT_STOP();
	}
}
