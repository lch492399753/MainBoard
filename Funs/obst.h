
#ifndef _OBST_H_
#define _OBST_H_

#include "sys.h"

#define FLASH_OBST_BASE_ADDR 	0X0803F800
#define FLASH_OBST_STA_ADDR 	0X0803F802
#define SONAR_CH_CONUT		3
#define OBST_BASE_DIS		40				/*µ¥Î» cm*/

extern uint8_t g_u8ObstDis;
extern uint8_t g_u8SonarDis[SONAR_CH_CONUT];
extern uint8_t g_u8ObstFlg;
extern uint8_t g_u8IsOpenObst;

void ObstDisFLASHCheck(void);
uint8_t IsObstOpen(void);

void ObstDisFLASHCheck(void);
void CmdSetObst( uint8_t dis);
#endif

