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

/*����ʽPID���ݽṹ*/
typedef struct 
{ 
	int8_t SetPoint;	   //�趨Ŀ��
	uint32_t U0;         //��������ʼֵ
	int32_t Sum_Error;	   //����ۼ�
	int8_t Error_UpLimit; //�����ֵ����
	int8_t Error_DnLimit; //�����ֵ����		
	float Proportion;  //��������
	float Integral;	   //���ֳ���
	float Derivative;  //΢�ֳ���
	int8_t Last_Error;	   //e[k-1]
	int8_t Prev_Error;	   //e[k-2] 
	int8_t I_Sign;        //���ٻ��ֳ���
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
PID_GLOBAL void PIDinit(PIDTypeDef* PIDx);              //��ʼ���������������� 
PID_GLOBAL int32_t PIDCalc(PIDTypeDef* PIDx, int8_t NextPoint);    //PID���� 
PID_GLOBAL void PID_Setpoint(PIDTypeDef* PIDx, int8_t SetValue);  //�趨 PIDԤ��ֵ 
PID_GLOBAL void PID_SetParameter(PIDTypeDef* PIDx, float Kp_Param, float Ki_Param, float Kd_Param);//�趨PID  kp ki kd�������� 

extern int32_t Left_pwm,Right_pwm;
void SetSPeed(char MotorID,int16_t Speed_PWM);
int IncPIDCalc(int NextPoint) ;
void IncPIDInit(void) ;


#endif
