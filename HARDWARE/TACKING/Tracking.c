#include "Tracking.h"
#include "gpio.h"
#include "PID.h"
#include "PWM.h"
#include "AGV_sensor.h"

u8 X=35,Y=35;
u8 zuo_chu=0,you_chu=0;

u16 speed_zhuanhuan( u32 speed_zhouqi)
{
    return (u16)((1.0/speed_zhouqi)*10000.0);
}

void AGV_Sensor(void);
uint8_t zuo1,zhong1,you1,zuo_chu1,you_chu1;
int16_t g_u16Sum=0;
void Tracking(short SPEED)
{
    u8 i=0,flag_agv=0;
    u8 zuo=0,zhong=0,you=0;
    flag_agv=0;
    if((key[0]+left1+right1+key[15]!=16))
    {
        while(i!=16)
        {
            {
                if(key[i]==0)
                {
                    flag_agv=1;
                    zhong++;
                }
                else
                {
                    if(flag_agv==0)
                        you++;
                    else
                        zuo++;
                }
				g_u16Sum += (1-key[i]) * PositionOffsetHW[i];
				
                i++;
            }
        }
//        C
    }
	
	if(g_u16Sum>0)
	{
		zuo_chu = 0;
		you_chu = 1;
	}
	else if(g_u16Sum<0)
	{
		zuo_chu = 1 ;
		you_chu = 0;
	}
	else
	{
		zuo_chu = 0;
		you_chu = 0;
	}
	
	zuo1 = zuo;
	you1 = you;
	zhong1 = zhong;
    if(zuo>you)
    {
//        SPEEDR=SPEED;
//        SPEEDL=SPEED-(zuo-you)*2;
//		g_S8SpeedLTarget = SPEED+(zuo-you)*2;
//		g_S8SpeedRTarget = SPEED-(zuo-you)*2;
//		SpeedCover(RightMotorID, g_S8SpeedRCurr,&SPEEDR );
//		SpeedCover(LeftMotorID, g_S8SpeedLCurr,&SPEEDL );
//		SPEEDL=SPEED+(zuo-you)*2;
//        SPEEDR=SPEED-(zuo-you)*2;
//		if(g_u8AutoRunFlg == 1)
//		{
//			//g_u8AutoRunFlg =1;
//			g_S8SpeedLTarget = SPEED+(zuo-you)*2;
//			g_S8SpeedRTarget = SPEED-(zuo-you)*2;
//			SpeedCover(RightMotorID, g_S8SpeedRCurr,&SPEEDR );
//			SpeedCover(LeftMotorID, g_S8SpeedLCurr,&SPEEDL );
//			
//		}
//        else
		{
			SPEEDL=SPEED+(zuo-you)*2;
			SPEEDR=SPEED-(zuo-you)*2;
		}
    }
    else
    {
//        SPEEDL=SPEED;
//        SPEEDR=SPEED-(you-zuo)*2;
		
//		SPEEDR=SPEED+(you-zuo)*2;
//        SPEEDL=SPEED-(you-zuo)*2;
//		g_S8SpeedRTarget = SPEED+(zuo-you)*2;
//		g_S8SpeedLTarget = SPEED-(zuo-you)*2;
//		SpeedCover(RightMotorID, g_S8SpeedRCurr,&SPEEDR );
//		SpeedCover(LeftMotorID, g_S8SpeedLCurr,&SPEEDL );
//		if(g_u8AutoRunFlg == 1)
//		{
//			//g_u8AutoRunFlg =1;
//			g_S8SpeedRTarget = SPEED+(zuo-you)*2;
//			g_S8SpeedLTarget = SPEED-(zuo-you)*2;
//			SpeedCover(RightMotorID, g_S8SpeedRCurr,&SPEEDR );
//			SpeedCover(LeftMotorID, g_S8SpeedLCurr,&SPEEDL );
//			
//		}
//        else
		{
			SPEEDR=SPEED+(you-zuo)*2;
			SPEEDL=SPEED-(you-zuo)*2;
		}
    }



    //usart2Printf("%d,%d,%d\r\n",zuo,zhong,you);
}

