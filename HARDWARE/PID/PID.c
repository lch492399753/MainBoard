#include "PID.h"
#include "pwm.h"
#include "string.h"
#include "stdio.h"
#include "usart.h"
#include "gpio.h"
//extern u32 tempup1;   //²¶»ñ×Ü¸ßµçÆ½µÄÊ±¼ä
//extern u32 tempup2;   //²¶»ñ×Ü¸ßµçÆ½µÄÊ±¼

#define _PIDGLOBAL_

PIDTypeDef g_tFootMotorPID;

//float Kp=-0.1,Ki=-0.3,Kd=-0.4;
short error,last_error,pre_error;

static PID sPID;
static PID *sptr = &sPID;
float error_p,last_error_p,pre_error_P;
int32_t Left_pwm=740,Right_pwm=740;

void Speed_Left (short std_speed,short curr_speed)
{
	 __IO uint32_t count=0;
	__IO int para=0;
	count = g_u32CaptureNumberL;
	g_u32CaptureNumberL = 0;
	sptr->SetPoint=std_speed;
	para = IncPIDCalc(g_u32CaptureNumberL);
	Left_pwm = SPEEDL + para;
	SetSPeed(LeftMotorID,Left_pwm);
}
void Speed_Right (short std_speed,short curr_speed)
{



	__IO uint32_t count=0;
	__IO int para=0;
	count = g_u32CaptureNumberR;
	//g_u32CaptureNumberR = 0;
	sptr->SetPoint=std_speed;
	para = IncPIDCalc(g_u32CaptureNumberR);
	Right_pwm = SPEEDR + para;
	SetSPeed(RightMotorID,Right_pwm);
}

/********************???PID????************************************/
int IncPIDCalc(int NextPoint) 
{
  int iError,iIncpid;                                 //????
  iError=sptr->SetPoint-NextPoint;                    //????
  iIncpid=(sptr->Proportion * iError)                 //E[k]?
              -(sptr->Integral * sptr->LastError)     //E[k-1]?
              +(sptr->Derivative * sptr->PrevError);  //E[k-2]?
              
  sptr->PrevError=sptr->LastError;                    //????,??????
  sptr->LastError=iError;
  return(iIncpid);                                    //?????
}

void IncPIDInit(void) 
{
    sptr->LastError=0;            //Error[-1]
    sptr->PrevError=0;            //Error[-2]
    sptr->Proportion=P_DATA;      //???? Proportional Const
    sptr->Integral=I_DATA;        //????  Integral Const
    sptr->Derivative=D_DATA;      //???? Derivative Const
    //sptr->SetPoint=100;           //????Desired Value
}

//float PID (int8_t target, int8_t currt)
//{

//	float out=0.0;

//// if(curr_speed>5)
//    error=target-currt;
//    //Right_pwm=Right_pwm+Kp*error+Ki*(error-last_error)+Kd*(error+pre_error-2*last_error);

//    out=currt+(Kp*error+Ki*(error-last_error));
//    last_error=pre_error;
//    pre_error=error;

//	return out;


////    if(Right_pwm>720)Right_pwm=720;
////    if(Right_pwm<200)Right_pwm=200;
////    TIM_SetCompare1(TIM3,Right_pwm);

//}

void SetSPeed(char MotorID,int16_t Speed_PWM)
{
//	if((Speed_PWM>720) || (Speed_PWM<0))
//	{
//		return;
//	}
	//Speed_PWM = 720 - Speed_PWM;
	
	switch(MotorID)
	{
		case LeftMotorID:
		{
			if(Speed_PWM<0)
			{
				WALK_LEFT_BACK();
				Speed_PWM *= -1;
				TIM_SetCompare2(TIM3,720 - Speed_PWM);
			}
			else if(Speed_PWM>0)
			{
				WALK_LEFT_FRONT();
				TIM_SetCompare2(TIM3,720 - Speed_PWM);
			}
			else
			{
				WALK_LEFT_STOP();
				TIM_SetCompare1(TIM3,720 - Speed_PWM);
				
			}
			//TIM_SetCompare2(TIM3,Speed_PWM);
			break;
		}
		case RightMotorID:
		{
			if(Speed_PWM<0)
			{
				WALK_RIGHT_BACK();
				Speed_PWM *= -1;
				TIM_SetCompare1(TIM3,720 - Speed_PWM);
			}
			else if(Speed_PWM>0)
			{
				WALK_RIGHT_FRONT();
				TIM_SetCompare1(TIM3,720 - Speed_PWM);
			}
			else
			{
				WALK_RIGHT_STOP();
				TIM_SetCompare1(TIM3,720 - Speed_PWM);
				
			}
			break;
		}
		default:
			break;
	}
	
	
}

void SetSPeedRight(int16_t Speed_PWM)
{
	Speed_PWM = 720 - Speed_PWM;
	
}


/******************************************************************************
*
* º¯ÊýÃû³Æ:	  void PIDinit(PIDTypeDef* PIDx)
* ¹¦ÄÜÃèÊö:   PID³õÊ¼»¯
* ÊäÈë²ÎÊý:   PIDx  PID²ÎÊý½á¹¹
* ·µ »Ø Öµ:   ÎÞ       
*             
******************************************************************************/
void PIDinit(PIDTypeDef* PIDx)
{
	memset(PIDx, 0, sizeof(PIDTypeDef));  //½«ËùÓÐÖµÇåÁã
	PIDx->Error_DnLimit = -30;   //´Åµ¼º½´«¸ÐÆ÷Æ«²î30mm
	PIDx->Error_UpLimit = 30;
	PIDx->I_Sign = 1;
}

/******************************************************************************
*
* º¯ÊýÃû³Æ:	  void PID_Setpoint(PIDTypeDef* PIDx, GR_S8 SetValue)
* ¹¦ÄÜÃèÊö:   PIDÉèÖÃÄ¿±ê
* ÊäÈë²ÎÊý:   PIDx  PID²ÎÊý½á¹¹
*             SetValue  Ä¿±êÖµ
* ·µ »Ø Öµ:   ÎÞ       
*             
******************************************************************************/
void PID_Setpoint(PIDTypeDef* PIDx, int8_t SetValue)
{
	PIDx->SetPoint = SetValue;
}

/**********************************************************************************************************
*
* º¯ÊýÃû³Æ:	  void PID_SetParameter(PIDTypeDef* PIDx, GR_FP32 Kp_Param, GR_FP32 Ki_Param, GR_FP32 Kd_Param)
* ¹¦ÄÜÃèÊö:   PIDÉèÖÃ²ÎÊý
* ÊäÈë²ÎÊý:   PIDx  PID²ÎÊý½á¹¹
*             Kp_Param   ±ÈÀý²ÎÊý
*             Ki_Param   »ý·Ö²ÎÊý
*             Kd_Param   Î¢·Ö²ÎÊý
* ·µ »Ø Öµ:   ÎÞ       
*             
**********************************************************************************************************/
void PID_SetParameter(PIDTypeDef* PIDx, float Kp_Param, float Ki_Param, float Kd_Param)
{
	PIDx->Proportion = Kp_Param;
	PIDx->Integral = Ki_Param;
	PIDx->Derivative = Kd_Param;
}

/*******************************************************************
*   º¯ÊýÃû³Æ£º GR_S32 incPIDcalc(PIDTypeDef *PIDx, GR_S8 NextPoint)                                  
*   ¹¦ÄÜÃèÊö£º PID¿ØÖÆ³ÌÐò (ÔöÁ¿Ê½)  
*   ²ÎÊýÁÐ±í£º 
*   ·µ»Ø½á¹û£º                                             
********************************************************************/
int32_t incPIDcalc(PIDTypeDef *PIDx, int8_t NextPoint) 
{
	/*
	Proportion    Kp
	Integral			Kp*T/Ti
	Derivative    Kp*Td/T
	*/
	int32_t iError = 0, iincpid = 0;
	int8_t Delta_Ei_PN = 0;

	iError = PIDx->SetPoint - NextPoint;  //µ±Ç°Îó²î
	PIDx->Sum_Error += iError;
    
	
	PIDx->Error_P = iError - PIDx->Last_Error;   /*e(k)-e(k-1)*/
	PIDx->Error_D = iError - 2 * PIDx->Last_Error + PIDx->Prev_Error; /*e(k)-2e(k-1)+e(k-2)*/
	PIDx->Error_I = iError;  /*e(k)*/
	
	if(PIDx->Error_P > 0) 
		Delta_Ei_PN = 1;
	else if(PIDx->Error_P < 0) 
		Delta_Ei_PN = -1;
	 /*ÔöÁ¿¼ÆËã
		iincpid = PIDx->Proportion * (iError - PIDx->Last_Error)
				+ PIDx->Integral * iError
				+ PIDx->Derivative * (iError - 2 * PIDx->Last_Error + PIDx->Prev_Error);*/

	if(((iError > 0) && (Delta_Ei_PN < 0)) || ((iError < 0) && (Delta_Ei_PN > 0)))
		PIDx->I_Sign = 0;
	else
		PIDx->I_Sign = 1;
	
	iincpid =  PIDx->Proportion * PIDx->Error_P
			+ PIDx->I_Sign * PIDx->Integral * PIDx->Error_I
			+ PIDx->Derivative * PIDx->Error_D;
	
	PIDx->DeltaUk = iincpid;
	if(iincpid > PIDx->MaxDeltaUk) 
	{
		PIDx->MaxDeltaUk = iincpid;
	}
	else if(iincpid < PIDx->MinDeltaUk)
	{		
		PIDx->MinDeltaUk = iincpid;
	}
	//´æ´¢Îó²î£¬±ãÓÚÏÂ´Î¼ÆËã  
	PIDx->Prev_Error = PIDx->Last_Error; 
	PIDx->Last_Error = iError;
	
	PIDx->Uk_Out = PIDx->Uk_PrevOut + iincpid;
	PIDx->Uk_PrevOut = PIDx->Uk_Out;

	return (iincpid); 
}

int32_t PIDCalc( PIDTypeDef *PIDx, int8_t NextPoint)
{
	return incPIDcalc(PIDx, NextPoint);
}
