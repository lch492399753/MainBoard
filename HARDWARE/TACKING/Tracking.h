#ifndef __Tracking_H
#define __Tracking_H
#include "sys.h"
void Speed_Left (short std_speed,short curr_speed);
void Speed_Right (short std_speed,short curr_speed);
void Tracking(short SPEED);
extern u8 zuo_chu,you_chu;
extern __IO uint8_t g_u8AutoRunFlg;
#endif
