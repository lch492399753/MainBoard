#ifndef __MSGWITHPAD_H
#define __MSGWITHPAD_H

#include "sys.h"
#include "usart.h"

void SendMsg2PAD(void);
void SetMsgUpdateFlg(void);

void SetTouchVal(uint8_t _val);
void SetObstVal(uint8_t _val);

void MsgUpdateOver(void);
void SetCardVal(uint8_t _card);

#endif



