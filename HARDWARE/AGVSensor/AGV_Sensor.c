/*
*********************************************************************************************************
* Copyright (c) 2016, XXXX公司
* All rights reserved.
*
* 文件名称: AGV_Sensor.c
* 文件标识:
* 摘    要: AGV导航传感器驱动
*
* 版    本: Ver 1.00
* 原 作 者:
* 完成日期: 2016.10.15
*
* 版    本:
* 作    者:
* 完成日期:
* 更改说明:
*
*********************************************************************************************************
*/

#include "AGV_Sensor.h"
#include "stm32f10x.h"
#include "gpio.h"
#include "sys.h" 
#include "delay.h"
#include "pwm.h"
#include "string.h"
#include "motor.h"

#define GUIDESENSOR_PITCH 10    //导向传感器测量点间距mm
#define HALF_GUIDESENSOR_PITCH (GUIDESENSOR_PITCH/2) //导向传感器测量点半间距mm

uint16_t DEG_SPEEDR = 0;
uint16_t DEG_SPEEDL = 0;


/*加权处理*/
int8_t PositionOffsetHW[16] =
{
	-11 * HALF_GUIDESENSOR_PITCH,
	-9 * HALF_GUIDESENSOR_PITCH,
	-7 * HALF_GUIDESENSOR_PITCH,
	-5 * HALF_GUIDESENSOR_PITCH,
	-4 * HALF_GUIDESENSOR_PITCH,
	-3 * HALF_GUIDESENSOR_PITCH,
	-2 * HALF_GUIDESENSOR_PITCH,
	-1 * HALF_GUIDESENSOR_PITCH,
	1 * HALF_GUIDESENSOR_PITCH,
	2 * HALF_GUIDESENSOR_PITCH,
	3 * HALF_GUIDESENSOR_PITCH,
	4 * HALF_GUIDESENSOR_PITCH,
	5 * HALF_GUIDESENSOR_PITCH,
	7 * HALF_GUIDESENSOR_PITCH,
	9 * HALF_GUIDESENSOR_PITCH,
	11 * HALF_GUIDESENSOR_PITCH
};

/*
*********************************************************************************************************
*
* 函数名称:	void AGV_GuideSensorInit()
* 作 用 域:
* 功能描述:	AGV导航传感器初始化
* 输入参数: 
* 输出参数:
* 全局变量: 
* 返 回 值:
* 备    注：
* 版    本: Ver 1.0
* 原 作 者: Iwen.Liu
* 完成日期: 2016.12.15
*
*********************************************************************************************************
*/
void AGV_GuideSensorInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE,ENABLE);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
}

/*
*********************************************************************************************************
*
* 函数名称:	uint16_t AGV_GetGuideSensorVal()
* 作 用 域:
* 功能描述:	获取导航传感器的数据
* 输入参数: 
* 输出参数:
* 全局变量: 
* 返 回 值:	导航传感器数据
* 备    注：导航传感器检测到磁条后输出低电平
* 版    本: Ver 1.0
* 原 作 者: Iwen.Liu
* 完成日期: 2016.12.15
*
*********************************************************************************************************
*/
uint16_t Debug_SensorVal=0;

uint16_t AGV_GetGuideSensorVal()
{
	uint16_t u16SensorVal = 0;				//用来存储导航传感器的数据
	
	u16SensorVal = (GPIO_ReadInputData(GPIOD) & 0xff00 ) |
				   ((GPIO_ReadInputData(GPIOB) & 0xf000 ) >>8) |
				   ((GPIO_ReadInputData(GPIOE) & 0xf000 ) >>12);
	Debug_SensorVal = u16SensorVal;
	//u16SensorVal &= 0xFFFF;
	
	return u16SensorVal;

}

int8_t g_s8GuideVal = 0;
extern uint8_t g_agv_fork_flg;
int8_t sensor_guide_forward(uint8_t* pos_buff, uint8_t pos_num);
uint8_t g_u8ForkPos[2];
uint8_t g_u8ForkCnt = 0;
uint16_t g_u16GuideSensorValLast = 0;	//用来存储出轨前最后一次导航传感器的值

/*对导航传感器采集的数值进行离散化处理*/
int8_t sensor_guide_dscrt ( uint16_t angel_val)
{
	uint8_t u8PosValidCount = 0;   //位置有效计数
	uint8_t u8Cnt = 0;
	uint8_t u8Cnt2 = 0;
	uint8_t u8GSValidSeq[16] = {0}; //存储有效位置序号数组

	g_s8GuideVal = 0;
	
	if( angel_val == 0xFFFF )		//未检测到磁条 2016-12-16
	{
		return 0;
	}
	else
	{
		g_u16GuideSensorValLast = angel_val;	//记录 出轨前传感器的值 2016-12-16
	}

	for( u8Cnt=0; u8Cnt<16; u8Cnt++ )
	{
		/*磁条低电平有效*/
		if ( (angel_val & (1 << u8Cnt) ) == 0 )
		{
			u8GSValidSeq[u8Cnt2++] = u8Cnt;
			u8PosValidCount++;
		}
	}

//	if( u8PosValidCount == 0 )					//未找到磁条
//	{
//		return 0;
//	}

//	g_u8ForkCnt = 0;							
//	for( u8Cnt2 = 0; u8Cnt2 < ( u8PosValidCount - 1 ); u8Cnt2++ )
//	{
//		if( u8GSValidSeq[u8Cnt2] != ( u8GSValidSeq[u8Cnt2 + 1] - 1 ) )	//如果有分叉点
//		{
//			g_u8ForkPos[g_u8ForkCnt++] = u8Cnt2;
//			if(g_u8ForkCnt >= 2)
//			{
//				break;
//			}
//		}
//	}
	
	return sensor_guide_forward(u8GSValidSeq, u8PosValidCount);    	
}


int8_t sensor_guide_forward(uint8_t* pos_buff, uint8_t pos_num)
{
	int8_t s8PosSum = 0;
	char index = 0;
	uint8_t left_cnt;
	uint8_t right_cnt;
	uint8_t gap_pos = 0;

    //if( g_u8ForkCnt == 0 )					//没有分叉点
    {
        for(index = 0; index < pos_num; index++)
		{
			s8PosSum += PositionOffsetHW[pos_buff[index]];
		}
        g_s8GuideVal = s8PosSum / pos_num;   			
    }
/*
	else if(g_u8ForkCnt == 1)				//有1个分叉点
	{
		gap_pos = g_u8ForkPos[0];			
		left_cnt  = gap_pos + 1;    
		right_cnt = pos_num - gap_pos - 1;
		if(left_cnt > right_cnt)
		{
			for(index = 0; index <= gap_pos; index++)
			{
				s8PosSum += PositionOffsetHW[pos_buff[index]];
			}
			g_u16GuideVal = s8PosSum / left_cnt;    
		}	
		else
		{
			for(index = gap_pos + 1; index < pos_num; index++)
			{
				s8PosSum += PositionOffsetHW[pos_buff[index]];
			}

			g_u16GuideVal = s8PosSum / right_cnt;
		}					
	}         
	else
	{
		for(index = g_u8ForkPos[0] + 1; index <= g_u8ForkPos[1]; index++)
		{
			s8PosSum +=  PositionOffsetHW[pos_buff[index]];   
		}
		g_u16GuideVal = s8PosSum / (g_u8ForkPos[1] - g_u8ForkPos[0]);
	}
*/		
    return 1;		
}
int16_t flg = 0;
uint16_t val=0;
void AGV_Sensor(void);
int16_t g_s16PosSumPre = 0;
int16_t s8PosSum = 0;
int8_t sumpos[16] = {0};
int8_t poscnt = 0;

__IO uint8_t g_u8OutTrackFlg = 3;		//出轨标记 0未找到磁条 1从左侧出轨	2从右侧出轨 3找磁条成功
int8_t g_u8OutTrackCnt=0;
void GuideWalking(uint8_t uSpeed)
{
	uint8_t i =0;
	
	int16_t sum = 0;
	int16_t Speed = 0;
	int speedl = 0;
	int speedr = 0;
	memset(key ,1 ,16);
	AGV_Sensor();
	
	for(i=0; i<16; i++)
	{
		sum += key[i];
	}
	
	if(sum==16)						//已经脱离轨道
	{
		g_u8OutTrackCnt++;
		if(g_u8OutTrackCnt>10)
			g_u8OutTrackCnt=10;
	}
	else
	{
		
		if(g_u8OutTrackCnt >0 )
			g_u8OutTrackCnt--;
		else
			g_u8OutTrackCnt=0;
			
	}
	
	if(g_u8OutTrackCnt>=5)
	{
		//g_u8OutTrackCnt = 5;
		if(s8PosSum < 0)
		{
			speedr = 0;
			speedl = TURN_SPEED;
			
			set_walkingmotor_speed(speedl , speedr);
			
			g_u8OutTrackFlg = 2; //从right侧出轨标志
		}
		else if(s8PosSum > 0)
		{
			g_S8SpeedRTarget = TURN_SPEED;
			g_S8SpeedLTarget = 0 ;
			
			speedl = 0;
			speedr = TURN_SPEED;
			
			set_walkingmotor_speed(speedl , speedr);
			g_u8OutTrackFlg = 1; //从left侧出轨标志
		}
		else //开机找磁条
		{
			g_S8SpeedRTarget = TURN_SPEED;
			g_S8SpeedLTarget = 0;
			
			speedl = 0;
			speedr = TURN_SPEED;
			
			set_walkingmotor_speed(speedl , speedr );
			g_u8OutTrackFlg = 0;
		}
		SPEEDR=g_S8SpeedRCurr;
		SPEEDL=g_S8SpeedLCurr;
		g_S8SpeedAutoCurr = 0;
		
		g_S8SpeedAutoTarget=0;
	}
	else if(g_u8OutTrackCnt == 0)
	{
		
		s8PosSum = 0;
		poscnt = 0;

		g_S8SpeedAutoTarget = g_u16RunSpeed;
		//SpeedCover(AutoMotorID, g_S8SpeedAutoCurr,&u16Speed );
		
		for(i=0; i<16; i++)
		{

			sumpos[i]  = (1-key[i])*PositionOffsetHW[i];
			if(key[i] == 0)
			{
				poscnt++;
			}

		}
		for(i=0; i<16; i++)
		{
			s8PosSum +=sumpos[i];
		}
		s8PosSum = (s8PosSum/poscnt);
		
		if(g_u8OutTrackFlg<3)	//找磁条未成功
		{
			if(g_u8OutTrackFlg < 2)
			{
				if(s8PosSum < 4)
				{
					g_u8OutTrackFlg = 3;
				}
			}
			else
			{
				if(s8PosSum > -4)
				{
					g_u8OutTrackFlg = 3;
				}
			}
			g_S8SpeedAutoCurr=0;
			
		}
		else	//找磁条成功
		{
			g_S8SpeedAutoTarget = g_u16RunSpeed;
			Speed = g_u16RunSpeed;
			g_u8OutTrackFlg = 3;
			if(s8PosSum>0)
			{
//				SPEEDL=Speed - 4*s8PosSum;
//				SPEEDR=Speed ;
	
				speedl=Speed - 4*s8PosSum;
				speedr=Speed ;				
				
//				if(SPEEDL<0)
//				{
//					SPEEDL = 0;
//				}
				
				if(speedl<0)
				{
					speedl = 0;
				}
				
				set_walkingmotor_speed(speedl , speedr);
			}
			else
			{
//				SPEEDL=Speed ;
//				SPEEDR=Speed + 4*s8PosSum;
				
				speedl=Speed ;
				speedr=Speed + 4*s8PosSum ;	
				
				if(SPEEDR<0)
				{
					SPEEDR = 0;

				}
				
				if(speedr<0)
				{
					speedr = 0;

				}
				
				set_walkingmotor_speed(speedl , speedr);
			}
		}

		g_s16PosSumPre = s8PosSum;
	}

}




/*******************************************end of file**************************************************/
