#ifndef __IFLYTEK_H
#define __IFLYTEK_H

#include "sys.h"
#include "myiic.h"

#define IFLYTEK_USART_MODE	0
#define IFLYTEK_IIC_MODE	1

#define IFLYTEK_WORK_MODE	IFLYTEK_USART_MODE

extern uint16_t g_u16iFlyTekAngel ;
extern uint16_t g_u16iFlyTekAngelPre ;

void CheckWakeUpSta(void);
uint8_t ReadWakeUpSta(void);
uint16_t ReadWakeUpAngel(void);
void IFlyTekReset(void);
uint8_t GetIFlyTekWorkSta(void);
void IFlyTekInit(void);
void GetIFlyTekAngel(void);
#endif

