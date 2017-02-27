#ifndef __GPIO_H
#define __GPIO_H	 
#include "sys.h"
	 
#define Obstacles   GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11)

//#define KEY0 PEin(4)   	//PE4
//#define KEY1 PEin(3)	//PE3 
//#define KEY2 PEin(2)	//PE2
//#define WK_UP PAin(0)	//PA0  WK_UP


#define Foot_Left_EN     PBout(5)
#define Foot_Left_BRK    PBout(8)
#define Foot_Left_FR     PBout(4)
//#define Foot_Left_SPEED  PBout(6)


#define Foot_Right_EN	PEout(0)
#define Foot_Right_BRK	PEout(1)
#define Foot_Right_FR	PBout(9)
//#define Foot_Right_SPEED PBout(7)

#define Walk_Stop		{ Foot_Right_EN=0; Foot_Left_EN=0; Foot_Right_BRK=0; Foot_Right_FR=0; Foot_Left_BRK=0;  Foot_Left_FR=1; }
#define Walk_Forward 	{ Foot_Right_EN=1; Foot_Left_EN=1; Foot_Right_BRK=0; Foot_Right_FR=0; Foot_Left_BRK=0;  Foot_Left_FR=1; }
#define Walk_Backward	{ Foot_Right_EN=1; Foot_Left_EN=1; Foot_Right_BRK=0; Foot_Right_FR=1; Foot_Left_BRK=0;  Foot_Left_FR=0; }
#define Walk_Left_Guai	{ Foot_Right_EN=1; Foot_Left_EN=0; Foot_Right_BRK=0; Foot_Right_FR=0; Foot_Left_BRK=1;  Foot_Left_FR=1; }
#define Walk_Right_Guai	{ Foot_Right_EN=0; Foot_Left_EN=1; Foot_Right_BRK=1; Foot_Right_FR=0; Foot_Left_BRK=0;  Foot_Left_FR=1; }
#define Walk_Right_Zhuan	{ Foot_Right_EN=1; Foot_Left_EN=1; Foot_Right_BRK=0; Foot_Right_FR=1; Foot_Left_BRK=0;  Foot_Left_FR=1; }
#define Walk_Left_Zhuan	{ Foot_Right_EN=1; Foot_Left_EN=1; Foot_Right_BRK=0; Foot_Right_FR=0; Foot_Left_BRK=0;  Foot_Left_FR=0; }

#define WALK_LEFT_FRONT();	{ Foot_Left_EN=1; Foot_Left_BRK=0;  Foot_Left_FR=1; }
#define WALK_LEFT_BACK();	{ Foot_Left_EN=1; Foot_Left_BRK=0;  Foot_Left_FR=0; }
#define WALK_LEFT_STOP();	{ Foot_Left_EN=0;}
#define WALK_LEFT_EN();		Foot_Left_EN = 1;
#define WALK_LEFT_DISEN();	Foot_Left_EN = 0;


#define WALK_RIGHT_FRONT();	{ Foot_Right_EN=1; Foot_Right_BRK=0; Foot_Right_FR=0; }
#define WALK_RIGHT_BACK();	{ Foot_Right_EN=1; Foot_Right_BRK=0; Foot_Right_FR=1; }
#define WALK_RIGHT_STOP();	{ Foot_Right_EN=0; }
#define WALK_RIGHT_EN();	Foot_Right_EN = 1;
#define WALK_RIGHT_DISEN();	Foot_Right_EN = 0;

#define WALK_STOP();		{ WALK_LEFT_STOP();WALK_RIGHT_STOP();}

#define Waist_In1  PDout(5)
#define Waist_In2  PDout(7)
#define Turn_In1   PDout(4)
#define Turn_In2   PDout(6)	
#define LED0       PEout(2)
#define Deng       PEout(3)	

#define Beep		PEout(5)	
#define TEST_LED	PEout(2)

void Bizhang_Gpio_Init(void);
void LED_AND_DENGTIAO_Init(void); 
void SPEED_GPIO_Init(void); 
void Foot_Motor_Gpio_Init(void);//初始化
void WaistAndTurn_Motor_Gpio_Init(void);//初始化
void AGV_GPIO_Init(void);
void BeepGPIOInit(void);
void RobotGpioInit(void);
extern u8  left1,right1;

#endif
