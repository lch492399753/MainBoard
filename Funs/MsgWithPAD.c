#include "MsgWithPAD.h"

uint16_t g_u16TouchVal2Pad = 0;
uint8_t	g_u8ObstVal2Pad = 0;
uint8_t g_u8CardNO2Pad = 0;
uint8_t g_u8Update2PAD_FLG = 0;

void SendMsg2PAD()
{
	if( g_u8Update2PAD_FLG == 0 )
	{
		return;
	}
	else
	{
		usart4Printf("{\"content\":\"00000%d000000000\"}",touch_val);
	}
}

void SetMsgUpdateFlg()
{
	g_u8Update2PAD_FLG = 1;
}

void SetTouchVal(uint16_t _val)
{
	g_u16TouchVal2Pad = _val;
}

void SetObstVal(uint8_t _val)
{
	g_u8ObstVal2Pad = _val;
}