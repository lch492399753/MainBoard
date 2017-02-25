#ifndef  __AGV_SENSOR_H
#define  __AGV_SENSOR_H

#include "stdint.h"
#include "PID.h"
#define GUIDESENSOR_ERR   1
#define GUIDESENSOR_ERR_NONE 0

#define LEFT_FORK_ROAD   0
#define RIGHT_FORK_ROAD   1

/*void Angle_GetData ( void );*/

extern int8_t PositionOffsetHW[16];
extern uint16_t g_u16GuideSensorValLast;

extern int8_t g_s8GuideVal;

extern __IO uint8_t g_u8OutTrackFlg;


int8_t sensor_guide_dscrt ( uint16_t angel_val);
uint16_t AGV_GetGuideSensorVal( void );
int8_t sensor_guide_forward(uint8_t* pos_buff, uint8_t pos_num);
#endif

