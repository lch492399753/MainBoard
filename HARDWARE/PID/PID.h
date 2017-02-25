#ifndef __PID_H
#define __PID_H	 


#ifndef		_PIDGLOBAL_
  #define	PID_GLOBAL	extern
#else
  #define	PID_GLOBAL	
#endif


#include "sys.h"
#include <string.h>
  
void Speed_Left (short std_speed,short curr_speed);
void Speed_Right (short std_speed,short curr_speed);	  

/*增量式PID数据结构*/
typedef struct 
{ 
	int8_t SetPoint;	   //设定目标
	uint32_t U0;         //操纵量初始值
	int32_t Sum_Error;	   //误差累计
	int8_t Error_UpLimit; //误差阈值上限
	int8_t Error_DnLimit; //误差阈值下限		
	float Proportion;  //比例常数
	float Integral;	   //积分常数
	float Derivative;  //微分常数
	int8_t Last_Error;	   //e[k-1]
	int8_t Prev_Error;	   //e[k-2] 
	int8_t I_Sign;        //变速积分常数
//	GR_S8 LastError[10];
	int8_t Error_P;    //error[k]- error[k-1]
	int8_t Error_D;    //error[k]- 2*error[k-1] + error[k-2]
	int8_t Error_I;    //error[k]
	int32_t DeltaUk;
	int32_t MaxDeltaUk;
	int32_t MinDeltaUk;
	int32_t Uk_PrevOut;
	int32_t Uk_Out;	
}PIDTypeDef;

typedef struct 
{
   __IO int      SetPoint;                                 //???? Desired Value
   __IO double   Proportion;                               //???? Proportional Const
   __IO double   Integral;                                 //???? Integral Const
   __IO double   Derivative;                               //???? Derivative Const
   __IO int      LastError;                                //Error[-1]
   __IO int      PrevError;                                //Error[-2]
}PID;

/* ????? ----------------------------------------------------------------*/
/*************************************/
//??PID???
// ?????????????????
/*************************************/
#define  P_DATA      -1.5                                 //P??
#define  I_DATA      0.8                                //I??
#define  D_DATA      0.4                               //D??

	
PID_GLOBAL PIDTypeDef g_tFootMotorPID;
PID_GLOBAL float Kp, Ki, Kd;
PID_GLOBAL void PIDinit(PIDTypeDef* PIDx);              //初始化，参数清零清零 
PID_GLOBAL int32_t PIDCalc(PIDTypeDef* PIDx, int8_t NextPoint);    //PID计算 
PID_GLOBAL void PID_Setpoint(PIDTypeDef* PIDx, int8_t SetValue);  //设定 PID预期值 
PID_GLOBAL void PID_SetParameter(PIDTypeDef* PIDx, float Kp_Param, float Ki_Param, float Kd_Param);//设定PID  kp ki kd三个参数 

extern int32_t Left_pwm,Right_pwm;
void SetSPeed(char MotorID,int16_t Speed_PWM);
int IncPIDCalc(int NextPoint) ;
void IncPIDInit(void) ;


#endif
