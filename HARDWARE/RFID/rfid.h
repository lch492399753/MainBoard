#ifndef __RFID_H
#define __RFID_H

#include "sys.h"
#include "MsgWithPAD.h"

#define USART5_BUFF_SIZE 32

extern uint8_t g_u8Usart5RecFlg;
extern uint8_t g_u8Usart5RecLen;


extern uint8_t UART5_RX_BUF[USART5_BUFF_SIZE];

void RfidInit(void);
void Rfid_heartbeat(void);
	
#endif