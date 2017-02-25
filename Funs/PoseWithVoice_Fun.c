#include "PoseWithVoice_Fun.h"
#include "pwm.h"
#include "common.h"
#include "inv_mpu.h"
#include "iFlytek.h"

#define VOICE_TURN_SPEED	50

VOICE_CTR_DEF g_tVoiceCtl;

static uint8_t count ;
float yaw_sum = 0.0;
int16_t RobotGetPos()
{
	float pitch,roll,yaw; 		//欧拉角
//	short aacx,aacy,aacz;		//加速度传感器原始数据
//	short gyrox,gyroy,gyroz;	//陀螺仪原始数据
	short temp;					//温度	
	
	int16_t yaw_out =0;
	if( mpu_dmp_get_data(&pitch,&roll,&yaw)==0 )
	{
		yaw = yaw+180;
		yaw_sum += (yaw);
		count++;
		if(count == 15)
		{
			count =0;
				
			yaw_out = (int16_t)(yaw_sum /16.0);
			if(yaw_out < 0)
				yaw_out = -yaw_out;
					
			yaw_sum = 0;
			return yaw_out;
		}
		else
			return -1;
	}
	else
			return -1;
}

void TurnWithVoice(int16_t _angel)
{
	GetIFlyTekAngel();
	
	if(_angel > 0)
		g_tVoiceCtl.u16CurrPos = _angel;
	
	if(g_tVoiceCtl.u8IsWakeUp == 1)
	{
		g_S8SpeedRTarget = 0;
		g_S8SpeedLTarget = 0;
		g_tVoiceCtl.u8RunFlg = 1;
		g_tVoiceCtl.u16DefPos = g_tVoiceCtl.u16CurrPos;
		g_tVoiceCtl.u8IsWakeUp = 0;
	}
	
	if(g_tVoiceCtl.u8RunFlg == 1)
	{
		
		if(g_tVoiceCtl.u16WakeAng > 180)
		{
			g_tVoiceCtl.u16WakeDeltaAngel =360 - g_tVoiceCtl.u16WakeAng ;
			g_tVoiceCtl.u8RunDir = TURN_LEFT_FLG;
			g_tVoiceCtl.u16TargetPos = g_tVoiceCtl.u16DefPos + g_tVoiceCtl.u16WakeDeltaAngel;
			if( g_tVoiceCtl.u16TargetPos > 360)
			{
				g_tVoiceCtl.u16TargetPos = g_tVoiceCtl.u16TargetPos - 360;
			}
		}
		else
		{
			g_tVoiceCtl.u16WakeDeltaAngel =g_tVoiceCtl.u16WakeAng ;
			g_tVoiceCtl.u8RunDir = TURN_RIGHT_FLG;
			
			if( (g_tVoiceCtl.u16DefPos - g_tVoiceCtl.u16WakeDeltaAngel) < 0)
			{
				g_tVoiceCtl.u16TargetPos = 360+g_tVoiceCtl.u16DefPos - g_tVoiceCtl.u16WakeDeltaAngel ;
			}
			else
				g_tVoiceCtl.u16TargetPos = g_tVoiceCtl.u16DefPos - g_tVoiceCtl.u16WakeDeltaAngel;
		}
		
		if( !(IsInRange(g_tVoiceCtl.u16CurrPos, g_tVoiceCtl.u16TargetPos, 10)) )
		{
			if(g_tVoiceCtl.u8RunDir == TURN_LEFT_FLG)
			{
				g_S8SpeedRTarget = VOICE_TURN_SPEED;
				g_S8SpeedLTarget = -VOICE_TURN_SPEED;
				g_u8SpeedDelta = 1;
			}
			else if( g_tVoiceCtl.u8RunDir == TURN_RIGHT_FLG )
			{
				g_S8SpeedRTarget = -VOICE_TURN_SPEED;
				g_S8SpeedLTarget = VOICE_TURN_SPEED;
				g_u8SpeedDelta = 1;
			}
			else
			{
				g_S8SpeedRTarget = 0;
				g_S8SpeedLTarget = 0;
//				g_tVoiceCtl.u8RunFlg = 0;
//				g_tVoiceCtl.u8RunDir = 0;
				g_u8SpeedDelta = VOICE_TURN_SPEED/2;
			}
		}
		else
		{
			g_S8SpeedRTarget = 0;
			g_S8SpeedLTarget = 0;
			g_tVoiceCtl.u8RunFlg = 0;
			g_tVoiceCtl.u8RunDir = 0;
			g_u8SpeedDelta = VOICE_TURN_SPEED/2;
			
		}
		
		
		
	}
}

