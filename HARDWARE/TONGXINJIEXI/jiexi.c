#include "jiexi.h"
#include "gpio.h"
#include "usart.h"
#include "delay.h"
#include "PWM.h"
#include "Tracking.h"
#include "AGV_sensor.h"
#include "PID.h"
#include "CmdPrase.h"
#include "motor.h"
#include "obst.h"

u8 ii=0,pos_flag=0;
//static  u8 t1=0,t2=0,wt1=0,wt2=0;
void Keep_Walking (u16 speed);
extern u8 ID_finished,walking_flag,id0_flag,work_flag,turn_flag,flag_ID,IDY_flag,diaot_flag;
extern  u16 ID1_start,ID1_length,ID2_start,ID2_length,ID3_start,ID3_length,ID4_start,ID4_length,ID5_start,ID5_length,
        ID6_start,ID6_length,ID7_start,ID7_length,ID8_start,ID8_length,ID9_start,ID9_length,ID10_start,ID10_length,
        ID11_start,ID11_length,ID12_start,ID12_length,ID13_start,ID13_length,ID14_start,ID14_length,ID15_start,ID15_length,
        ID16_start,ID16_length,ID17_start,ID17_length,ID18_start,ID18_length,ID19_start,ID19_length,ID20_start,ID20_length;
u16 walk_time,walking1_time,walking2_time;
u8 ASC_To_Valu(u8 asc);
u8 shoubi_flag=0;
extern u32 delay_time;

void GetLimit(char _data)
{
	switch(_data)
	{
		case 1:
		{
			
			break;
		}
		case 2:
		{
			break;
		}
		case 3:
		{
			break;
		}
		case 4:
		{
			break;
		}
		case 5:
		{
			break;
		}
		case 6:
		{
			break;
		}
		case 7:
		{
			break;
		}
		case 8:
		{
			break;
		}
		case 9:
		{
			break;
		}
		case 10:
		{
			break;
		}
		case 11:
		{
			break;
		}
		case 12:
		{
			break;
		}
		default:
			break;
	}
}

void Motor_Head(char B2,u16 CC, u16 DD)
{

    switch(B2)
    {
        case '2':
			
            usart3Printf("{\"00160000012%d%d00000\"}",CC,DD);
			SetARAct( AR_HEAD, LEFT_OR_UP, 30, 0);
			
            IDY_flag=0;
            break;
        case '3':
            usart3Printf("{\"00160000013%d%d00000\"}",CC,DD);
			SetARAct( AR_HEAD, RIGHT_OR_DOWN, 30, 0);
			
            IDY_flag=0;
            break;
    }
    ii--;
}
void Motor_Arm_Left(char B2,u16 CC, u16 DD)
{
    {
        switch(B2)
        {
            case '1':
                usart3Printf("{\"00160000021%d%d00000\"}",CC,DD);
                break;
            case '4':
                usart3Printf("{\"00160000024%d%d00000\"}",CC,DD);
                break;
            default:
                break;
        }
    }
    ii--;
}
void Motor_Arm_Right(char B2,u16 CC, u16 DD)
{
//if(work_flag==1)
    {
        switch(B2)
        {
            case '1':
                usart3Printf("{\"00160000031%d%d00000\"}",CC,DD);
                IDY_flag=0;
                break;
            case '4':
                usart3Printf("{\"00160000034%d%d00000\"}",CC,DD);
                IDY_flag=0;
                break;
            default:
                break;
        }
    }
    ii--;
}
void Chest(char B2,u16 CC, u16 DD)
{
    //if(work_flag==1)
    {
        switch(B2)
        {
            case '1':
                usart3Printf("{\"00160000041000000000\"}",CC,DD);
                IDY_flag=0;
                break;
            case '4':
                usart3Printf("{\"00160000044000000000\"}",CC,DD);
                IDY_flag=0;
                break;
            default:
                break;
        }
    }
    ii--;
}
void Waist(char B2,u16 CC, u16 DD)
{
//if(work_flag==1)
    {
        switch(B2)
        {
            case '2':
                usart3Printf("{\"00160000052%d%d00000\"}",CC,DD);
                IDY_flag=0;
                break;
            case '3':
                usart3Printf("{\"00160000053%d%d00000\"}",CC,DD);
                IDY_flag=0;
                break;
            default:
                break;
        }
    }
    ii--;
}

void Motor_Chest()
{





}

void Forward ()
{
    Foot_Left_FR = 0;
    Foot_Right_FR =1;
    Foot_Left_EN=0;
    Foot_Right_EN=0;
}
void Backward (u16 speed)
{
    Preset_Pwm=800;
    //  delay_ms(100);
    //  Foot_Left_In1 = 0;
    Foot_Left_FR = 1;
    Foot_Right_FR =0;
    //  Foot_Right_In2 =1;
    //Speed (speed);
}
void Turn_Left (u16 speed)
{
    Preset_Pwm=800;
//  delay_ms(100);
    // Foot_Left_In1 = 1;
    Foot_Left_FR = 0;
    Foot_Right_FR =0;
    //Foot_Right_In2 =1;
    // Speed (speed);
}
void Turn_Right (u16 speed)
{
    Preset_Pwm=800;
    //delay_ms(100);
    // Foot_Left_In1 = 0;
    Foot_Left_FR = 1;
    Foot_Right_FR =1;
    //  Foot_Right_In2 =0;
    //Speed (speed);
}
void Stop ()
{
    //  Foot_Left_In1 = 0;
    Foot_Left_FR = 0;
    Foot_Right_FR =0;
    //  Foot_Right_In2 =0;
    Waist_In1=0;
    Waist_In2=0;
}

void diaot(u8 CC)
{
    switch(CC)
    {

        case 0:
        {
            diaot_flag=0;
        }
        break;
        case 1:
        {
            diaot_flag=1;
        }
        break;


//           case 'L':{ turn_flag=2;}break;
//       case 'R':{ turn_flag=3;}break;

    }



}


void walking_motor(u16 AAAAA,char B2,u16 CC,u16 EEEEE)
{
	switch(B2)
	{
		case '0':
		{
			walking_flag=0;
			g_u8StopFlg =1;
			g_S8SpeedLTarget = 0;
			g_S8SpeedRTarget = 0;
			set_walkingmotor_speed(0, 0);
			g_u8AutoRunFlg=0;
			g_u8SpeedDelta = 4;
			
		}
		break;
		case '1':
		{
			walking_flag=1;
			g_S8SpeedLTarget = g_u16RunSpeed;
			g_S8SpeedRTarget = g_u16RunSpeed;
			g_u8SpeedDelta = 1;
			set_walkingmotor_speed(g_S8SpeedLTarget, g_S8SpeedRTarget );
		}
		break;
		case '2':
		{
			walking_flag=2;
			g_S8SpeedRTarget = TURN_SPEED;
			g_S8SpeedLTarget = 0;
			g_u8SpeedDelta = 1;
			set_walkingmotor_speed(-g_S8SpeedLTarget , g_S8SpeedRTarget );
		}
		break;
		case '3':
		{
			walking_flag=3;
			g_S8SpeedRTarget = 0;
			g_S8SpeedLTarget = TURN_SPEED;
			g_u8SpeedDelta = 1;
			set_walkingmotor_speed(g_S8SpeedLTarget , -g_S8SpeedRTarget);
			
		}
		break;
		case '4':
		{
			walking_flag=4;
			g_S8SpeedRTarget = g_u16RunSpeed;
			g_S8SpeedLTarget = g_u16RunSpeed ;
			g_u8SpeedDelta = 1;
			set_walkingmotor_speed(-g_S8SpeedLTarget , -g_S8SpeedRTarget);
			
		}
		break;
		case '5':
		{
			walking_flag=5;
			diaot(CC);
			g_S8SpeedRTarget = -TURN_SPEED;
			g_S8SpeedLTarget = TURN_SPEED;
			g_u8SpeedDelta = 1;
			
		}
		break;
		case '6':
		{
			walking_flag = 6;
			g_u8AutoRunFlg = 1;
			g_S8SpeedAutoCurr = 0;
			g_u8OutTrackFlg = 3;
			PIDinit(&g_tFootMotorPID);
			PID_SetParameter(&g_tFootMotorPID, Kp, Ki, Kd);
			Right_pwm = 720;
			Left_pwm = 720;
			g_S8SpeedLTarget = 0;
			g_S8SpeedRTarget = 0;
		}
		break;
		case '7':
		{
			walking_flag=7;
		}
		break;
		//case 'R':{ walking_flag=3;}break;

	}
    ii--;
}
void biaoqing(char b2)
{
    switch(b2)
    {
        case '0':
            usart1Printf("{0}");
            break;
        case '1':
            usart1Printf("{1}");
            break;
        case '2':
            usart1Printf("{2}");
            break;
        case '3':
            usart1Printf("{3}");
            break;
        case '4':
            usart1Printf("{4}");
            break;
        case '5':
            usart1Printf("{5}");
            break;
        case '6':
            usart1Printf("{6}");
            break;
        case '7':
            usart1Printf("{7}");
            break;
        case '8':
            usart1Printf("{8}");
            break;
        case '9':
            usart1Printf("{9}");
            break;
    }
    ii--;

}
void dengtiao(char b2)
{
    switch(b2)
    {
        case '0':
            Deng=0;
            usart1Printf("{8}");
            break;
        case '1':
            Deng=1;
            usart1Printf("{1}");
            break;
    }
    ii--;

}












void ID_0(char *str,u16 start,u16 length)
{
    u16 AAAAA,EEEEE;
    char B1,B2;
    u16 CC,DD;
	
    static u8 t=0;
// for(t=0;t<(length);t++)
//          {
//              USART_SendData(UART4, IDNdata[t]);//向串口1发送数据
//              while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//          }

    ii=length/16;
    t=0;
	
    while(ii!=0)
    {

        {
            AAAAA=ASC_To_Valu(str[start+t*16])*10000+ASC_To_Valu(str[start+1+t*16])*1000+ASC_To_Valu(str[start+2+t*16])*100+ASC_To_Valu(str[start+3+t*16])*10+ASC_To_Valu(str[start+4+t*16]);
            B1=str[start+5+t*16];
            B2=str[start+6+t*16];
            CC=ASC_To_Valu(str[start+7+t*16])*10+ASC_To_Valu(str[start+8+t*16]);
            DD=ASC_To_Valu(str[start+9+t*16])*10+ASC_To_Valu(str[start+11+t*16]);
            EEEEE=ASC_To_Valu(str[start+11+t*16])*10000+ASC_To_Valu(str[start+12+t*16])*1000+ASC_To_Valu(str[start+13+t*16])*100+ASC_To_Valu(str[start+14+t*16])*10+ASC_To_Valu(str[start+15+t*16]);
            switch(B1)
            {
                case '1':
                    Motor_Head(B2,CC,DD);
                    break;
                case '2':
                    Motor_Arm_Left(B2,CC,DD);
                    break;
                case '3':
                    Motor_Arm_Right(B2,CC,DD);
                    break;
                case '4':
                    Chest(B2,CC,DD);
                    break;
                case '5':
                    Waist(B2,CC,DD);;
                    break;
                case '6':
					if(CC == 0)
					{
						walking_motor(AAAAA,B2,CC,EEEEE);
					}
					else
					{
						switch(CC)
						{
							case '5':
								g_u16RunSpeed = HIGH_SPEED;
								break;
							case '4':
								g_u16RunSpeed = MIDDLE_SPEED;
								break;
							case '2':
								g_u16RunSpeed = LOW_SPEED;
								break;
							default:
								break;
						}
					}
                    break;
                case '7':
					SetExpress(B2);
                    biaoqing(B2);
					
					//ii--;
					//RingBuffer_InsertMult(&g_tUSART3TxRingBuf, CMDBlk.u8CMDBodyByte, 16);
                    break;
                case '8':
                    ;
                    break;
                case '9':
                    dengtiao(B2);
                    break;
                case 'A':
					//SetObst(0, EEEEE, EEEEE, EEEEE);
					CmdSetObst( EEEEE);
					ii--;
					//RingBuffer_InsertMult(&g_tUSART3TxRingBuf, CMDBlk.u8CMDBodyByte, 16);
                    
                    break;
                case 'B':
                    SetLimit( B2 - '0');
					ii--;
                    break;
                case 'C':
					
                    ;
                    break;
                case 'D':
                    ;
                    break;
            }
            //  usart4Printf("B1%c\r\n",B1);
            t++;
        }

    }
//usart4Printf("{h}\r\n");
    id0_flag=0;

}


























void ID_1(char  *str,u16 start,u16 length)
{
    u16 AAAAA,EEEEE;
    char B1,B2;
    u16 CC,DD;
    static u8 t=0;
// for(t=0;t<(length);t++)
//          {
//              USART_SendData(UART4, IDNdata[t]);//向串口1发送数据
//              while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//          }
    ii=length/16;
    t=0;
    while(ii!=0)
    {
        {
            AAAAA=ASC_To_Valu(str[start+t*16])*10000+ASC_To_Valu(str[start+1+t*16])*1000+ASC_To_Valu(str[start+2+t*16])*100+ASC_To_Valu(str[start+3+t*16])*10+ASC_To_Valu(str[start+4+t*16]);
            B1=str[start+5+t*16];
            B2=str[start+6+t*16];
            CC=ASC_To_Valu(str[start+7+t*16])*10+ASC_To_Valu(str[start+8+t*16]);
            DD=ASC_To_Valu(str[start+9+t*16])*10+ASC_To_Valu(str[start+11+t*16]);
            EEEEE=ASC_To_Valu(str[start+11+t*16])*10000+ASC_To_Valu(str[start+12+t*16])*1000+ASC_To_Valu(str[start+13+t*16])*100+ASC_To_Valu(str[start+14+t*16])*10+ASC_To_Valu(str[start+15+t*16]);
            switch(B1)
            {
                case '1':
                    Motor_Head(B2,CC,DD);
                    break;
                case '2':
                    Motor_Arm_Left(B2,CC,DD);
                    break;
                case '3':
                    Motor_Arm_Right(B2,CC,DD);
                    break;
                case '4':
                    Chest(B2,CC,DD);
                    break;
                case '5':
                    Waist(B2,CC,DD);
                    break;
                case '6':
                    walking_motor(AAAAA,B2,CC,EEEEE);
                    break;
                case '7':
                    biaoqing(B2);
                    break;
                case '8':
                    ;
                    break;
                case '9':
                    dengtiao(B2);
                    break;
                case 'A':
                    ;
                    break;
                case 'B':
                    ;
                    break;
                case 'C':
                    ;
                    break;
                case 'D':
                    ;
                    break;
            }
            t++;
        }

    }
    work_flag=0;
}
void ID_2(char *str,u16 start,u16 length)
{
    u16 AAAAA,EEEEE;
    char B1,B2;
    u16 CC,DD;
    static u8 t=0;
// for(t=0;t<(length);t++)
//          {
//              USART_SendData(UART4, IDNdata[t]);//向串口1发送数据
//              while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//          }
    ii=length/16;
    t=0;
    while(ii!=0)
    {
        {
            AAAAA=ASC_To_Valu(str[start+t*16])*10000+ASC_To_Valu(str[start+1+t*16])*1000+ASC_To_Valu(str[start+2+t*16])*100+ASC_To_Valu(str[start+3+t*16])*10+ASC_To_Valu(str[start+4+t*16]);
            B1=str[start+5+t*16];
            B2=str[start+6+t*16];
            CC=ASC_To_Valu(str[start+7+t*16])*10+ASC_To_Valu(str[start+8+t*16]);
            DD=ASC_To_Valu(str[start+9+t*16])*10+ASC_To_Valu(str[start+11+t*16]);
            EEEEE=ASC_To_Valu(str[start+11+t*16])*10000+ASC_To_Valu(str[start+12+t*16])*1000+ASC_To_Valu(str[start+13+t*16])*100+ASC_To_Valu(str[start+14+t*16])*10+ASC_To_Valu(str[start+15+t*16]);
            switch(B1)
            {
                case '1':
                    Motor_Head(B2,CC,DD);
                    break;
                case '2':
                    Motor_Arm_Left(B2,CC,DD);
                    break;
                case '3':
                    Motor_Arm_Right(B2,CC,DD);
                    break;
                case '4':
                    Chest(B2,CC,DD);
                    break;
                case '5':
                    Waist(B2,CC,DD);;
                    break;
                case '6':
                    walking_motor(AAAAA,B2,CC,EEEEE);
                    break;
                case '7':
                    biaoqing(B2);
                    break;
                case '8':
                    ;
                    break;
                case '9':
                    dengtiao(B2);
                    break;
                case 'A':
                    ;
                    break;
                case 'B':
                    ;
                    break;
                case 'C':
                    ;
                    break;
                case 'D':
                    ;
                    break;
            }
            t++;
        }

    }
    work_flag=0;




}
void ID_3(char *str,u16 start,u16 length)
{
    u16 AAAAA,EEEEE;
    char B1,B2;
    u16 CC,DD;
    static u8 t=0;
// for(t=0;t<(length);t++)
//          {
//              USART_SendData(UART4, IDNdata[t]);//向串口1发送数据
//              while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//          }
    t=0;
    ii=length/16;
    while(ii!=0)
    {
        {
            AAAAA=ASC_To_Valu(str[start+t*16])*10000+ASC_To_Valu(str[start+1+t*16])*1000+ASC_To_Valu(str[start+2+t*16])*100+ASC_To_Valu(str[start+3+t*16])*10+ASC_To_Valu(str[start+4+t*16]);
            B1=str[start+5+t*16];
            B2=str[start+6+t*16];
            CC=ASC_To_Valu(str[start+7+t*16])*10+ASC_To_Valu(str[start+8+t*16]);
            DD=ASC_To_Valu(str[start+9+t*16])*10+ASC_To_Valu(str[start+11+t*16]);
            EEEEE=ASC_To_Valu(str[start+11+t*16])*10000+ASC_To_Valu(str[start+12+t*16])*1000+ASC_To_Valu(str[start+13+t*16])*100+ASC_To_Valu(str[start+14+t*16])*10+ASC_To_Valu(str[start+15+t*16]);
            switch(B1)
            {
                case '1':
                    Motor_Head(B2,CC,DD);
                    break;
                case '2':
                    Motor_Arm_Left(B2,CC,DD);
                    break;
                case '3':
                    Motor_Arm_Right(B2,CC,DD);
                    break;
                case '4':
                    Chest(B2,CC,DD);
                    break;
                case '5':
                    Waist(B2,CC,DD);;
                    break;
                case '6':
                    walking_motor(AAAAA,B2,CC,EEEEE);
                    break;
                case '7':
                    biaoqing(B2);
                    break;
                case '8':
                    ;
                    break;
                case '9':
                    dengtiao(B2);
                    break;
                case 'A':
                    ;
                    break;
                case 'B':
                    ;
                    break;
                case 'C':
                    ;
                    break;
                case 'D':
                    ;
                    break;
            }
            t++;
        }

    }

    work_flag=0;
}
void ID_4(char *str,u16 start,u16 length)
{
    u16 AAAAA,EEEEE;
    char B1,B2;
    u16 CC,DD;
    static u8 t=0;
// for(t=0;t<(length);t++)
//          {
//              USART_SendData(UART4, IDNdata[t]);//向串口1发送数据
//              while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//          }
    ii=length/16;
    t=0;
    while(ii!=0)
    {
        {
            AAAAA=ASC_To_Valu(str[start+t*16])*10000+ASC_To_Valu(str[start+1+t*16])*1000+ASC_To_Valu(str[start+2+t*16])*100+ASC_To_Valu(str[start+3+t*16])*10+ASC_To_Valu(str[start+4+t*16]);
            B1=str[start+5+t*16];
            B2=str[start+6+t*16];
            CC=ASC_To_Valu(str[start+7+t*16])*10+ASC_To_Valu(str[start+8+t*16]);
            DD=ASC_To_Valu(str[start+9+t*16])*10+ASC_To_Valu(str[start+11+t*16]);
            EEEEE=ASC_To_Valu(str[start+11+t*16])*10000+ASC_To_Valu(str[start+12+t*16])*1000+ASC_To_Valu(str[start+13+t*16])*100+ASC_To_Valu(str[start+14+t*16])*10+ASC_To_Valu(str[start+15+t*16]);
            switch(B1)
            {
                case '1':
                    Motor_Head(B2,CC,DD);
                    break;
                case '2':
                    Motor_Arm_Left(B2,CC,DD);
                    break;
                case '3':
                    Motor_Arm_Right(B2,CC,DD);
                    break;
                case '4':
                    Chest(B2,CC,DD);
                    break;
                case '5':
                    Waist(B2,CC,DD);;
                    break;
                case '6':
                    walking_motor(AAAAA,B2,CC,EEEEE);
                    break;
                case '7':
                    biaoqing(B2);
                    break;
                case '8':
                    ;
                    break;
                case '9':
                    dengtiao(B2);
                    break;
                case 'A':
                    ;
                    break;
                case 'B':
                    ;
                    break;
                case 'C':
                    ;
                    break;
                case 'D':
                    ;
                    break;
            }
            t++;
        }

    }
    work_flag=0;







}
void ID_5(char *str,u16 start,u16 length)
{
    u16 AAAAA,EEEEE;
    char B1,B2;
    u16 CC,DD;
    static u8 t=0;
// for(t=0;t<(length);t++)
//          {
//              USART_SendData(UART4, IDNdata[t]);//向串口1发送数据
//              while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//          }
    ii=length/16;
    t=0;
    while(ii!=0)
    {
        {
            AAAAA=ASC_To_Valu(str[start+t*16])*10000+ASC_To_Valu(str[start+1+t*16])*1000+ASC_To_Valu(str[start+2+t*16])*100+ASC_To_Valu(str[start+3+t*16])*10+ASC_To_Valu(str[start+4+t*16]);
            B1=str[start+5+t*16];
            B2=str[start+6+t*16];
            CC=ASC_To_Valu(str[start+7+t*16])*10+ASC_To_Valu(str[start+8+t*16]);
            DD=ASC_To_Valu(str[start+9+t*16])*10+ASC_To_Valu(str[start+11+t*16]);
            EEEEE=ASC_To_Valu(str[start+11+t*16])*10000+ASC_To_Valu(str[start+12+t*16])*1000+ASC_To_Valu(str[start+13+t*16])*100+ASC_To_Valu(str[start+14+t*16])*10+ASC_To_Valu(str[start+15+t*16]);
            switch(B1)
            {
                case '1':
                    Motor_Head(B2,CC,DD);
                    break;
                case '2':
                    Motor_Arm_Left(B2,CC,DD);
                    break;
                case '3':
                    Motor_Arm_Right(B2,CC,DD);
                    break;
                case '4':
                    Chest(B2,CC,DD);
                    break;
                case '5':
                    Waist(B2,CC,DD);;
                    break;
                case '6':
                    walking_motor(AAAAA,B2,CC,EEEEE);
                    break;
                case '7':
                    biaoqing(B2);
                    break;
                case '8':
                    ;
                    break;
                case '9':
                    dengtiao(B2);
                    break;
                case 'A':
                    ;
                    break;
                case 'B':
                    ;
                    break;
                case 'C':
                    ;
                    break;
                case 'D':
                    ;
                    break;
            }
            t++;
        }

    }
    work_flag=0;







}
void ID_6(char *str,u16 start,u16 length)
{
    u16 AAAAA,EEEEE;
    char B1,B2;
    u16 CC,DD;
    static u8 t=0;
// for(t=0;t<(length);t++)
//          {
//              USART_SendData(UART4, IDNdata[t]);//向串口1发送数据
//              while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//          }
    ii=length/16;
    t=0;
    while(ii!=0)
    {
        {
            AAAAA=ASC_To_Valu(str[start+t*16])*10000+ASC_To_Valu(str[start+1+t*16])*1000+ASC_To_Valu(str[start+2+t*16])*100+ASC_To_Valu(str[start+3+t*16])*10+ASC_To_Valu(str[start+4+t*16]);
            B1=str[start+5+t*16];
            B2=str[start+6+t*16];
            CC=ASC_To_Valu(str[start+7+t*16])*10+ASC_To_Valu(str[start+8+t*16]);
            DD=ASC_To_Valu(str[start+9+t*16])*10+ASC_To_Valu(str[start+11+t*16]);
            EEEEE=ASC_To_Valu(str[start+11+t*16])*10000+ASC_To_Valu(str[start+12+t*16])*1000+ASC_To_Valu(str[start+13+t*16])*100+ASC_To_Valu(str[start+14+t*16])*10+ASC_To_Valu(str[start+15+t*16]);
            switch(B1)
            {
                case '1':
                    Motor_Head(B2,CC,DD);
                    break;
                case '2':
                    Motor_Arm_Left(B2,CC,DD);
                    break;
                case '3':
                    Motor_Arm_Right(B2,CC,DD);
                    break;
                case '4':
                    Chest(B2,CC,DD);
                    break;
                case '5':
                    Waist(B2,CC,DD);;
                    break;
                case '6':
                    walking_motor(AAAAA,B2,CC,EEEEE);
                    break;
                case '7':
                    biaoqing(B2);
                    break;
                case '8':
                    ;
                    break;
                case '9':
                    dengtiao(B2);
                    break;
                case 'A':
                    ;
                    break;
                case 'B':
                    ;
                    break;
                case 'C':
                    ;
                    break;
                case 'D':
                    ;
                    break;
            }
            t++;
        }

    }
    work_flag=0;







}
void ID_7(char *str,u16 start,u16 length)
{
    u16 AAAAA,EEEEE;
    char B1,B2;
    u16 CC,DD;
    static u8 t=0;
// for(t=0;t<(length);t++)
//          {
//              USART_SendData(UART4, IDNdata[t]);//向串口1发送数据
//              while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//          }
    ii=length/16;
    t=0;
    while(ii!=0)
    {
        {
            AAAAA=ASC_To_Valu(str[start+t*16])*10000+ASC_To_Valu(str[start+1+t*16])*1000+ASC_To_Valu(str[start+2+t*16])*100+ASC_To_Valu(str[start+3+t*16])*10+ASC_To_Valu(str[start+4+t*16]);
            B1=str[start+5+t*16];
            B2=str[start+6+t*16];
            CC=ASC_To_Valu(str[start+7+t*16])*10+ASC_To_Valu(str[start+8+t*16]);
            DD=ASC_To_Valu(str[start+9+t*16])*10+ASC_To_Valu(str[start+11+t*16]);
            EEEEE=ASC_To_Valu(str[start+11+t*16])*10000+ASC_To_Valu(str[start+12+t*16])*1000+ASC_To_Valu(str[start+13+t*16])*100+ASC_To_Valu(str[start+14+t*16])*10+ASC_To_Valu(str[start+15+t*16]);
            switch(B1)
            {
                case '1':
                    Motor_Head(B2,CC,DD);
                    break;
                case '2':
                    Motor_Arm_Left(B2,CC,DD);
                    break;
                case '3':
                    Motor_Arm_Right(B2,CC,DD);
                    break;
                case '4':
                    Chest(B2,CC,DD);
                    break;
                case '5':
                    Waist(B2,CC,DD);;
                    break;
                case '6':
                    walking_motor(AAAAA,B2,CC,EEEEE);
                    break;
                case '7':
                    biaoqing(B2);
                    break;
                case '8':
                    ;
                    break;
                case '9':
                    dengtiao(B2);
                    break;
                case 'A':
                    ;
                    break;
                case 'B':
                    ;
                    break;
                case 'C':
                    ;
                    break;
                case 'D':
                    ;
                    break;
            }
            t++;
        }

    }
    work_flag=0;







}
void ID_8(char *str,u16 start,u16 length)
{

    u16 AAAAA,EEEEE;
    char B1,B2;
    u16 CC,DD;
    static u8 t=0;
// for(t=0;t<(length);t++)
//          {
//              USART_SendData(UART4, IDNdata[t]);//向串口1发送数据
//              while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//          }
    ii=length/16;
    t=0;
    while(ii!=0)
    {
        {
            AAAAA=ASC_To_Valu(str[start+t*16])*10000+ASC_To_Valu(str[start+1+t*16])*1000+ASC_To_Valu(str[start+2+t*16])*100+ASC_To_Valu(str[start+3+t*16])*10+ASC_To_Valu(str[start+4+t*16]);
            B1=str[start+5+t*16];
            B2=str[start+6+t*16];
            CC=ASC_To_Valu(str[start+7+t*16])*10+ASC_To_Valu(str[start+8+t*16]);
            DD=ASC_To_Valu(str[start+9+t*16])*10+ASC_To_Valu(str[start+11+t*16]);
            EEEEE=ASC_To_Valu(str[start+11+t*16])*10000+ASC_To_Valu(str[start+12+t*16])*1000+ASC_To_Valu(str[start+13+t*16])*100+ASC_To_Valu(str[start+14+t*16])*10+ASC_To_Valu(str[start+15+t*16]);
            switch(B1)
            {
                case '1':
                    Motor_Head(B2,CC,DD);
                    break;
                case '2':
                    Motor_Arm_Left(B2,CC,DD);
                    break;
                case '3':
                    Motor_Arm_Right(B2,CC,DD);
                    break;
                case '4':
                    Chest(B2,CC,DD);
                    break;
                case '5':
                    Waist(B2,CC,DD);
                    break;
                case '6':
                    walking_motor(AAAAA,B2,CC,EEEEE);
                    break;
                case '7':
                    biaoqing(B2);
                    break;
                case '8':
                    ;
                    break;
                case '9':
                    dengtiao(B2);
                    break;
                case 'A':
                    ;
                    break;
                case 'B':
                    ;
                    break;
                case 'C':
                    ;
                    break;
                case 'D':
                    ;
                    break;
            }
            t++;
        }

    }
    work_flag=0;






}
void ID_9(char *str,u16 start,u16 length)
{
    u16 AAAAA,EEEEE;
    char B1,B2;
    u16 CC,DD;
    static u8 t=0;
// for(t=0;t<(length);t++)
//          {
//              USART_SendData(UART4, IDNdata[t]);//向串口1发送数据
//              while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//          }
    ii=length/16;
    t=0;
    while(ii!=0)
    {
        {
            AAAAA=ASC_To_Valu(str[start+t*16])*10000+ASC_To_Valu(str[start+1+t*16])*1000+ASC_To_Valu(str[start+2+t*16])*100+ASC_To_Valu(str[start+3+t*16])*10+ASC_To_Valu(str[start+4+t*16]);
            B1=str[start+5+t*16];
            B2=str[start+6+t*16];
            CC=ASC_To_Valu(str[start+7+t*16])*10+ASC_To_Valu(str[start+8+t*16]);
            DD=ASC_To_Valu(str[start+9+t*16])*10+ASC_To_Valu(str[start+11+t*16]);
            EEEEE=ASC_To_Valu(str[start+11+t*16])*10000+ASC_To_Valu(str[start+12+t*16])*1000+ASC_To_Valu(str[start+13+t*16])*100+ASC_To_Valu(str[start+14+t*16])*10+ASC_To_Valu(str[start+15+t*16]);
            switch(B1)
            {
                case '1':
                    Motor_Head(B2,CC,DD);
                    break;
                case '2':
                    Motor_Arm_Left(B2,CC,DD);
                    break;
                case '3':
                    Motor_Arm_Right(B2,CC,DD);
                    break;
                case '4':
                    Chest(B2,CC,DD);
                    break;
                case '5':
                    Waist(B2,CC,DD);;
                    break;
                case '6':
                    walking_motor(AAAAA,B2,CC,EEEEE);
                    break;
                case '7':
                    biaoqing(B2);
                    break;
                case '8':
                    ;
                    break;
                case '9':
                    dengtiao(B2);
                    break;
                case 'A':
                    ;
                    break;
                case 'B':
                    ;
                    break;
                case 'C':
                    ;
                    break;
                case 'D':
                    ;
                    break;
            }
            t++;
        }

    }
    work_flag=0;







}
void ID_10(char *str,u16 start,u16 length)
{
    u16 AAAAA,EEEEE;
    char B1,B2;
    u16 CC,DD;
    static u8 t=0;
// for(t=0;t<(length);t++)
//          {
//              USART_SendData(UART4, IDNdata[t]);//向串口1发送数据
//              while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//          }
    ii=length/16;
    t=0;
    while(ii!=0)
    {
        {
            AAAAA=ASC_To_Valu(str[start+t*16])*10000+ASC_To_Valu(str[start+1+t*16])*1000+ASC_To_Valu(str[start+2+t*16])*100+ASC_To_Valu(str[start+3+t*16])*10+ASC_To_Valu(str[start+4+t*16]);
            B1=str[start+5+t*16];
            B2=str[start+6+t*16];
            CC=ASC_To_Valu(str[start+7+t*16])*10+ASC_To_Valu(str[start+8+t*16]);
            DD=ASC_To_Valu(str[start+9+t*16])*10+ASC_To_Valu(str[start+11+t*16]);
            EEEEE=ASC_To_Valu(str[start+11+t*16])*10000+ASC_To_Valu(str[start+12+t*16])*1000+ASC_To_Valu(str[start+13+t*16])*100+ASC_To_Valu(str[start+14+t*16])*10+ASC_To_Valu(str[start+15+t*16]);
            switch(B1)
            {
                case '1':
                    Motor_Head(B2,CC,DD);
                    break;
                case '2':
                    Motor_Arm_Left(B2,CC,DD);
                    break;
                case '3':
                    Motor_Arm_Right(B2,CC,DD);
                    break;
                case '4':
                    Chest(B2,CC,DD);
                    break;
                case '5':
                    Waist(B2,CC,DD);;
                    break;
                case '6':
                    walking_motor(AAAAA,B2,CC,EEEEE);
                    break;
                case '7':
                    biaoqing(B2);
                    break;
                case '8':
                    ;
                    break;
                case '9':
                    dengtiao(B2);
                    break;
                case 'A':
                    ;
                    break;
                case 'B':
                    ;
                    break;
                case 'C':
                    ;
                    break;
                case 'D':
                    ;
                    break;
            }
            t++;
        }

    }
    work_flag=0;







}
void ID_11(char *str,u16 start,u16 length)
{
    u16 AAAAA,EEEEE;
    char B1,B2;
    u16 CC,DD;
    static u8 t=0;
// for(t=0;t<(length);t++)
//          {
//              USART_SendData(UART4, IDNdata[t]);//向串口1发送数据
//              while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//          }
    ii=length/16;
    t=0;
    while(ii!=0)
    {
        {
            AAAAA=ASC_To_Valu(str[start+t*16])*10000+ASC_To_Valu(str[start+1+t*16])*1000+ASC_To_Valu(str[start+2+t*16])*100+ASC_To_Valu(str[start+3+t*16])*10+ASC_To_Valu(str[start+4+t*16]);
            B1=str[start+5+t*16];
            B2=str[start+6+t*16];
            CC=ASC_To_Valu(str[start+7+t*16])*10+ASC_To_Valu(str[start+8+t*16]);
            DD=ASC_To_Valu(str[start+9+t*16])*10+ASC_To_Valu(str[start+11+t*16]);
            EEEEE=ASC_To_Valu(str[start+11+t*16])*10000+ASC_To_Valu(str[start+12+t*16])*1000+ASC_To_Valu(str[start+13+t*16])*100+ASC_To_Valu(str[start+14+t*16])*10+ASC_To_Valu(str[start+15+t*16]);
            switch(B1)
            {
                case '1':
                    Motor_Head(B2,CC,DD);
                    break;
                case '2':
                    Motor_Arm_Left(B2,CC,DD);
                    break;
                case '3':
                    Motor_Arm_Right(B2,CC,DD);
                    break;
                case '4':
                    Chest(B2,CC,DD);
                    break;
                case '5':
                    Waist(B2,CC,DD);;
                    break;
                case '6':
                    walking_motor(AAAAA,B2,CC,EEEEE);
                    break;
                case '7':
                    biaoqing(B2);
                    break;
                case '8':
                    ;
                    break;
                case '9':
                    dengtiao(B2);
                    break;
                case 'A':
                    ;
                    break;
                case 'B':
                    ;
                    break;
                case 'C':
                    ;
                    break;
                case 'D':
                    ;
                    break;
            }
            t++;
        }

    }
    work_flag=0;






}
void ID_12(char *str,u16 start,u16 length)
{
    u16 AAAAA,EEEEE;
    char B1,B2;
    u16 CC,DD;
    static u8 t=0;
// for(t=0;t<(length);t++)
//          {
//              USART_SendData(UART4, IDNdata[t]);//向串口1发送数据
//              while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//          }
    ii=length/16;
    t=0;
    while(ii!=0)
    {
        {
            AAAAA=ASC_To_Valu(str[start+t*16])*10000+ASC_To_Valu(str[start+1+t*16])*1000+ASC_To_Valu(str[start+2+t*16])*100+ASC_To_Valu(str[start+3+t*16])*10+ASC_To_Valu(str[start+4+t*16]);
            B1=str[start+5+t*16];
            B2=str[start+6+t*16];
            CC=ASC_To_Valu(str[start+7+t*16])*10+ASC_To_Valu(str[start+8+t*16]);
            DD=ASC_To_Valu(str[start+9+t*16])*10+ASC_To_Valu(str[start+11+t*16]);
            EEEEE=ASC_To_Valu(str[start+11+t*16])*10000+ASC_To_Valu(str[start+12+t*16])*1000+ASC_To_Valu(str[start+13+t*16])*100+ASC_To_Valu(str[start+14+t*16])*10+ASC_To_Valu(str[start+15+t*16]);
            switch(B1)
            {
                case '1':
                    Motor_Head(B2,CC,DD);
                    break;
                case '2':
                    Motor_Arm_Left(B2,CC,DD);
                    break;
                case '3':
                    Motor_Arm_Right(B2,CC,DD);
                    break;
                case '4':
                    Chest(B2,CC,DD);
                    break;
                case '5':
                    Waist(B2,CC,DD);;
                    break;
                case '6':
                    walking_motor(AAAAA,B2,CC,EEEEE);
                    break;
                case '7':
                    biaoqing(B2);
                    break;
                case '8':
                    ;
                    break;
                case '9':
                    dengtiao(B2);
                    break;
                case 'A':
                    ;
                    break;
                case 'B':
                    ;
                    break;
                case 'C':
                    ;
                    break;
                case 'D':
                    ;
                    break;
            }
            t++;
        }

    }

    work_flag=0;






}
void ID_13(char *str,u16 start,u16 length)
{
    u16 AAAAA,EEEEE;
    char B1,B2;
    u16 CC,DD;
    static u8 t=0;
// for(t=0;t<(length);t++)
//          {
//              USART_SendData(UART4, IDNdata[t]);//向串口1发送数据
//              while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//          }
    ii=length/16;
    t=0;
    while(ii!=0)
    {
        {
            AAAAA=ASC_To_Valu(str[start+t*16])*10000+ASC_To_Valu(str[start+1+t*16])*1000+ASC_To_Valu(str[start+2+t*16])*100+ASC_To_Valu(str[start+3+t*16])*10+ASC_To_Valu(str[start+4+t*16]);
            B1=str[start+5+t*16];
            B2=str[start+6+t*16];
            CC=ASC_To_Valu(str[start+7+t*16])*10+ASC_To_Valu(str[start+8+t*16]);
            DD=ASC_To_Valu(str[start+9+t*16])*10+ASC_To_Valu(str[start+11+t*16]);
            EEEEE=ASC_To_Valu(str[start+11+t*16])*10000+ASC_To_Valu(str[start+12+t*16])*1000+ASC_To_Valu(str[start+13+t*16])*100+ASC_To_Valu(str[start+14+t*16])*10+ASC_To_Valu(str[start+15+t*16]);
            switch(B1)
            {
                case '1':
                    Motor_Head(B2,CC,DD);
                    break;
                case '2':
                    Motor_Arm_Left(B2,CC,DD);
                    break;
                case '3':
                    Motor_Arm_Right(B2,CC,DD);
                    break;
                case '4':
                    Chest(B2,CC,DD);
                    break;
                case '5':
                    Waist(B2,CC,DD);;
                    break;
                case '6':
                    walking_motor(AAAAA,B2,CC,EEEEE);
                    break;
                case '7':
                    biaoqing(B2);
                    break;
                case '8':
                    ;
                    break;
                case '9':
                    dengtiao(B2);
                    break;
                case 'A':
                    ;
                    break;
                case 'B':
                    ;
                    break;
                case 'C':
                    ;
                    break;
                case 'D':
                    ;
                    break;
            }
            t++;
        }

    }
    work_flag=0;
}
void ID_14(char  *str,u16 start,u16 length)
{
    u16 AAAAA,EEEEE;
    char B1,B2;
    u16 CC,DD;
    static u8 t=0;
// for(t=0;t<(length);t++)
//          {
//              USART_SendData(UART4, IDNdata[t]);//向串口1发送数据
//              while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//          }
    ii=length/16;
    t=0;
    while(ii!=0)
    {
        {
            AAAAA=ASC_To_Valu(str[start+t*16])*10000+ASC_To_Valu(str[start+1+t*16])*1000+ASC_To_Valu(str[start+2+t*16])*100+ASC_To_Valu(str[start+3+t*16])*10+ASC_To_Valu(str[start+4+t*16]);
            B1=str[start+5+t*16];
            B2=str[start+6+t*16];
            CC=ASC_To_Valu(str[start+7+t*16])*10+ASC_To_Valu(str[start+8+t*16]);
            DD=ASC_To_Valu(str[start+9+t*16])*10+ASC_To_Valu(str[start+11+t*16]);
            EEEEE=ASC_To_Valu(str[start+11+t*16])*10000+ASC_To_Valu(str[start+12+t*16])*1000+ASC_To_Valu(str[start+13+t*16])*100+ASC_To_Valu(str[start+14+t*16])*10+ASC_To_Valu(str[start+15+t*16]);
            switch(B1)
            {
                case '1':
                    Motor_Head(B2,CC,DD);
                    break;
                case '2':
                    Motor_Arm_Left(B2,CC,DD);
                    break;
                case '3':
                    Motor_Arm_Right(B2,CC,DD);
                    break;
                case '4':
                    Chest(B2,CC,DD);
                    break;
                case '5':
                    Waist(B2,CC,DD);
                    break;
                case '6':
                    walking_motor(AAAAA,B2,CC,EEEEE);
                    break;
                case '7':
                    biaoqing(B2);
                    break;
                case '8':
                    ;
                    break;
                case '9':
                    dengtiao(B2);
                    break;
                case 'A':
                    ;
                    break;
                case 'B':
                    ;
                    break;
                case 'C':
                    ;
                    break;
                case 'D':
                    ;
                    break;
            }
            t++;
        }

    }
    work_flag=0;
}
void ID_15(char  *str,u16 start,u16 length)
{
    u16 AAAAA,EEEEE;
    char B1,B2;
    u16 CC,DD;
    static u8 t=0;
// for(t=0;t<(length);t++)
//          {
//              USART_SendData(UART4, IDNdata[t]);//向串口1发送数据
//              while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//          }
    ii=length/16;
    t=0;
    while(ii!=0)
    {
        {
            AAAAA=ASC_To_Valu(str[start+t*16])*10000+ASC_To_Valu(str[start+1+t*16])*1000+ASC_To_Valu(str[start+2+t*16])*100+ASC_To_Valu(str[start+3+t*16])*10+ASC_To_Valu(str[start+4+t*16]);
            B1=str[start+5+t*16];
            B2=str[start+6+t*16];
            CC=ASC_To_Valu(str[start+7+t*16])*10+ASC_To_Valu(str[start+8+t*16]);
            DD=ASC_To_Valu(str[start+9+t*16])*10+ASC_To_Valu(str[start+11+t*16]);
            EEEEE=ASC_To_Valu(str[start+11+t*16])*10000+ASC_To_Valu(str[start+12+t*16])*1000+ASC_To_Valu(str[start+13+t*16])*100+ASC_To_Valu(str[start+14+t*16])*10+ASC_To_Valu(str[start+15+t*16]);
            switch(B1)
            {
                case '1':
                    Motor_Head(B2,CC,DD);
                    break;
                case '2':
                    Motor_Arm_Left(B2,CC,DD);
                    break;
                case '3':
                    Motor_Arm_Right(B2,CC,DD);
                    break;
                case '4':
                    Chest(B2,CC,DD);
                    break;
                case '5':
                    Waist(B2,CC,DD);
                    break;
                case '6':
                    walking_motor(AAAAA,B2,CC,EEEEE);
                    break;
                case '7':
                    biaoqing(B2);
                    break;
                case '8':
                    ;
                    break;
                case '9':
                    dengtiao(B2);
                    break;
                case 'A':
                    ;
                    break;
                case 'B':
                    ;
                    break;
                case 'C':
                    ;
                    break;
                case 'D':
                    ;
                    break;
            }
            t++;
        }

    }
    work_flag=0;
}
void ID_16(char  *str,u16 start,u16 length)
{
    u16 AAAAA,EEEEE;
    char B1,B2;
    u16 CC,DD;
    static u8 t=0;
// for(t=0;t<(length);t++)
//          {
//              USART_SendData(UART4, IDNdata[t]);//向串口1发送数据
//              while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//          }
    ii=length/16;
    t=0;
    while(ii!=0)
    {
        {
            AAAAA=ASC_To_Valu(str[start+t*16])*10000+ASC_To_Valu(str[start+1+t*16])*1000+ASC_To_Valu(str[start+2+t*16])*100+ASC_To_Valu(str[start+3+t*16])*10+ASC_To_Valu(str[start+4+t*16]);
            B1=str[start+5+t*16];
            B2=str[start+6+t*16];
            CC=ASC_To_Valu(str[start+7+t*16])*10+ASC_To_Valu(str[start+8+t*16]);
            DD=ASC_To_Valu(str[start+9+t*16])*10+ASC_To_Valu(str[start+11+t*16]);
            EEEEE=ASC_To_Valu(str[start+11+t*16])*10000+ASC_To_Valu(str[start+12+t*16])*1000+ASC_To_Valu(str[start+13+t*16])*100+ASC_To_Valu(str[start+14+t*16])*10+ASC_To_Valu(str[start+15+t*16]);
            switch(B1)
            {
                case '1':
                    Motor_Head(B2,CC,DD);
                    break;
                case '2':
                    Motor_Arm_Left(B2,CC,DD);
                    break;
                case '3':
                    Motor_Arm_Right(B2,CC,DD);
                    break;
                case '4':
                    Chest(B2,CC,DD);
                    break;
                case '5':
                    Waist(B2,CC,DD);
                    break;
                case '6':
                    walking_motor(AAAAA,B2,CC,EEEEE);
                    break;
                case '7':
                    biaoqing(B2);
                    break;
                case '8':
                    ;
                    break;
                case '9':
                    dengtiao(B2);
                    break;
                case 'A':
                    ;
                    break;
                case 'B':
                    ;
                    break;
                case 'C':
                    ;
                    break;
                case 'D':
                    ;
                    break;
            }
            t++;
        }

    }
    work_flag=0;
}
void ID_17(char  *str,u16 start,u16 length)
{
    u16 AAAAA,EEEEE;
    char B1,B2;
    u16 CC,DD;
    static u8 t=0;
// for(t=0;t<(length);t++)
//          {
//              USART_SendData(UART4, IDNdata[t]);//向串口1发送数据
//              while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//          }
    ii=length/16;
    t=0;
    while(ii!=0)
    {
        {
            AAAAA=ASC_To_Valu(str[start+t*16])*10000+ASC_To_Valu(str[start+1+t*16])*1000+ASC_To_Valu(str[start+2+t*16])*100+ASC_To_Valu(str[start+3+t*16])*10+ASC_To_Valu(str[start+4+t*16]);
            B1=str[start+5+t*16];
            B2=str[start+6+t*16];
            CC=ASC_To_Valu(str[start+7+t*16])*10+ASC_To_Valu(str[start+8+t*16]);
            DD=ASC_To_Valu(str[start+9+t*16])*10+ASC_To_Valu(str[start+11+t*16]);
            EEEEE=ASC_To_Valu(str[start+11+t*16])*10000+ASC_To_Valu(str[start+12+t*16])*1000+ASC_To_Valu(str[start+13+t*16])*100+ASC_To_Valu(str[start+14+t*16])*10+ASC_To_Valu(str[start+15+t*16]);
            switch(B1)
            {
                case '1':
                    Motor_Head(B2,CC,DD);
                    break;
                case '2':
                    Motor_Arm_Left(B2,CC,DD);
                    break;
                case '3':
                    Motor_Arm_Right(B2,CC,DD);
                    break;
                case '4':
                    Chest(B2,CC,DD);
                    break;
                case '5':
                    Waist(B2,CC,DD);
                    break;
                case '6':
                    walking_motor(AAAAA,B2,CC,EEEEE);
                    break;
                case '7':
                    biaoqing(B2);
                    break;
                case '8':
                    ;
                    break;
                case '9':
                    dengtiao(B2);
                    break;
                case 'A':
                    ;
                    break;
                case 'B':
                    ;
                    break;
                case 'C':
                    ;
                    break;
                case 'D':
                    ;
                    break;
            }
            t++;
        }

    }
    work_flag=0;
}
void ID_18(char  *str,u16 start,u16 length)
{
    u16 AAAAA,EEEEE;
    char B1,B2;
    u16 CC,DD;
    static u8 t=0;
// for(t=0;t<(length);t++)
//          {
//              USART_SendData(UART4, IDNdata[t]);//向串口1发送数据
//              while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//          }
    ii=length/16;
    t=0;
    while(ii!=0)
    {
        {
            AAAAA=ASC_To_Valu(str[start+t*16])*10000+ASC_To_Valu(str[start+1+t*16])*1000+ASC_To_Valu(str[start+2+t*16])*100+ASC_To_Valu(str[start+3+t*16])*10+ASC_To_Valu(str[start+4+t*16]);
            B1=str[start+5+t*16];
            B2=str[start+6+t*16];
            CC=ASC_To_Valu(str[start+7+t*16])*10+ASC_To_Valu(str[start+8+t*16]);
            DD=ASC_To_Valu(str[start+9+t*16])*10+ASC_To_Valu(str[start+11+t*16]);
            EEEEE=ASC_To_Valu(str[start+11+t*16])*10000+ASC_To_Valu(str[start+12+t*16])*1000+ASC_To_Valu(str[start+13+t*16])*100+ASC_To_Valu(str[start+14+t*16])*10+ASC_To_Valu(str[start+15+t*16]);
            switch(B1)
            {
                case '1':
                    Motor_Head(B2,CC,DD);
                    break;
                case '2':
                    Motor_Arm_Left(B2,CC,DD);
                    break;
                case '3':
                    Motor_Arm_Right(B2,CC,DD);
                    break;
                case '4':
                    Chest(B2,CC,DD);
                    break;
                case '5':
                    Waist(B2,CC,DD);
                    break;
                case '6':
                    walking_motor(AAAAA,B2,CC,EEEEE);
                    break;
                case '7':
                    biaoqing(B2);
                    break;
                case '8':
                    ;
                    break;
                case '9':
                    dengtiao(B2);
                    break;
                case 'A':
                    ;
                    break;
                case 'B':
                    ;
                    break;
                case 'C':
                    ;
                    break;
                case 'D':
                    ;
                    break;
            }
            t++;
        }

    }
    work_flag=0;
}
void ID_19(char  *str,u16 start,u16 length)
{
    u16 AAAAA,EEEEE;
    char B1,B2;
    u16 CC,DD;
    static u8 t=0;
// for(t=0;t<(length);t++)
//          {
//              USART_SendData(UART4, IDNdata[t]);//向串口1发送数据
//              while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//          }
    ii=length/16;
    t=0;
    while(ii!=0)
    {
        {
            AAAAA=ASC_To_Valu(str[start+t*16])*10000+ASC_To_Valu(str[start+1+t*16])*1000+ASC_To_Valu(str[start+2+t*16])*100+ASC_To_Valu(str[start+3+t*16])*10+ASC_To_Valu(str[start+4+t*16]);
            B1=str[start+5+t*16];
            B2=str[start+6+t*16];
            CC=ASC_To_Valu(str[start+7+t*16])*10+ASC_To_Valu(str[start+8+t*16]);
            DD=ASC_To_Valu(str[start+9+t*16])*10+ASC_To_Valu(str[start+11+t*16]);
            EEEEE=ASC_To_Valu(str[start+11+t*16])*10000+ASC_To_Valu(str[start+12+t*16])*1000+ASC_To_Valu(str[start+13+t*16])*100+ASC_To_Valu(str[start+14+t*16])*10+ASC_To_Valu(str[start+15+t*16]);
            switch(B1)
            {
                case '1':
                    Motor_Head(B2,CC,DD);
                    break;
                case '2':
                    Motor_Arm_Left(B2,CC,DD);
                    break;
                case '3':
                    Motor_Arm_Right(B2,CC,DD);
                    break;
                case '4':
                    Chest(B2,CC,DD);
                    break;
                case '5':
                    Waist(B2,CC,DD);
                    break;
                case '6':
                    walking_motor(AAAAA,B2,CC,EEEEE);
                    break;
                case '7':
                    biaoqing(B2);
                    break;
                case '8':
                    ;
                    break;
                case '9':
                    dengtiao(B2);
                    break;
                case 'A':
                    ;
                    break;
                case 'B':
                    ;
                    break;
                case 'C':
                    ;
                    break;
                case 'D':
                    ;
                    break;
            }
            t++;
        }

    }
    work_flag=0;
}
void ID_20(char  *str,u16 start,u16 length)
{
    u16 AAAAA,EEEEE;
    char B1,B2;
    u16 CC,DD;
    static u8 t=0;
// for(t=0;t<(length);t++)
//          {
//              USART_SendData(UART4, IDNdata[t]);//向串口1发送数据
//              while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//          }
    ii=length/16;
    t=0;
    while(ii!=0)
    {
        {
            AAAAA=ASC_To_Valu(str[start+t*16])*10000+ASC_To_Valu(str[start+1+t*16])*1000+ASC_To_Valu(str[start+2+t*16])*100+ASC_To_Valu(str[start+3+t*16])*10+ASC_To_Valu(str[start+4+t*16]);
            B1=str[start+5+t*16];
            B2=str[start+6+t*16];
            CC=ASC_To_Valu(str[start+7+t*16])*10+ASC_To_Valu(str[start+8+t*16]);
            DD=ASC_To_Valu(str[start+9+t*16])*10+ASC_To_Valu(str[start+11+t*16]);
            EEEEE=ASC_To_Valu(str[start+11+t*16])*10000+ASC_To_Valu(str[start+12+t*16])*1000+ASC_To_Valu(str[start+13+t*16])*100+ASC_To_Valu(str[start+14+t*16])*10+ASC_To_Valu(str[start+15+t*16]);
            switch(B1)
            {
                case '1':
                    Motor_Head(B2,CC,DD);
                    break;
                case '2':
                    Motor_Arm_Left(B2,CC,DD);
                    break;
                case '3':
                    Motor_Arm_Right(B2,CC,DD);
                    break;
                case '4':
                    Chest(B2,CC,DD);
                    break;
                case '5':
                    Waist(B2,CC,DD);
                    break;
                case '6':
                    walking_motor(AAAAA,B2,CC,EEEEE);
                    break;
                case '7':
                    biaoqing(B2);
                    break;
                case '8':
                    ;
                    break;
                case '9':
                    dengtiao(B2);
                    break;
                case 'A':
                    ;
                    break;
                case 'B':
                    ;
                    break;
                case 'C':
                    ;
                    break;
                case 'D':
                    ;
                    break;
            }
            t++;
        }

    }
    work_flag=0;
}

























void jiexi_ID(u8 id)
{
    //usart4Printf("id%d",id);
    switch(id)
    {
        case 1:
            ID_1(IDYdata,ID1_start,ID1_length);
            break;
        case 2:
            ID_2(IDYdata,ID2_start,ID2_length);
            break;
        case 3:
            ID_3(IDYdata,ID3_start,ID3_length);
            break;
        case 4:
            ID_4(IDYdata,ID4_start,ID4_length);
            break;
        case 5:
            ID_5(IDYdata,ID5_start,ID5_length);
            break;
        case 6:
            ID_6(IDYdata,ID6_start,ID6_length);
            break;
        case 7:
            ID_7(IDYdata,ID7_start,ID7_length);
            break;
        case 8:
            ID_8(IDYdata,ID8_start,ID8_length);
            break;
        case 9:
            ID_9(IDYdata,ID9_start,ID9_length);
            break;
        case 10:
            ID_10(IDYdata,ID10_start,ID10_length);
            break;
        case 11:
            ID_11(IDYdata,ID11_start,ID11_length);
            break;
        case 12:
            ID_12(IDYdata,ID12_start,ID12_length);
            break;
        case 13:
            ID_13(IDYdata,ID13_start,ID13_length);
            break;
        case 14:
            ID_14(IDYdata,ID14_start,ID14_length);
            break;
        case 15:
            ID_15(IDYdata,ID15_start,ID15_length);
            break;
        case 16:
            ID_16(IDYdata,ID16_start,ID16_length);
            break;
        case 17:
            ID_17(IDYdata,ID17_start,ID17_length);
            break;
        case 18:
            ID_18(IDYdata,ID18_start,ID18_length);
            break;
        case 19:
            ID_19(IDYdata,ID19_start,ID19_length);
            break;
        case 20:
            ID_20(IDYdata,ID20_start,ID20_length);
            break;
    }
}
