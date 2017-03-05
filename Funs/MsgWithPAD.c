#include "MsgWithPAD.h"

uint8_t g_u8TouchVal2Pad = 0;
uint8_t	g_u8ObstVal2Pad = 0;
uint8_t g_u8CardNO2Pad = 0;
uint8_t g_u8Update2PAD_FLG = 0;

void SendMsg2PAD()
{
	uint8_t cardH = 0, cardL = 0;
	cardH = g_u8CardNO2Pad/10;
	cardL = g_u8CardNO2Pad%10;
	if( g_u8Update2PAD_FLG == 0 )
	{
		return;
	}
	else
	{

		usart4Printf("{\"content\":\"00%d%d%d%d00000000\"}",cardH,cardL,g_u8ObstVal2Pad,g_u8TouchVal2Pad);
		MsgUpdateOver();
	}
}

void SetCardVal(uint8_t _card)
{
	g_u8CardNO2Pad = _card;
}


void SetMsgUpdateFlg()
{
	g_u8Update2PAD_FLG = 1;
}

void SetTouchVal(uint8_t _val)
{
	g_u8TouchVal2Pad = _val;
}

void SetObstVal(uint8_t _val)
{
	g_u8ObstVal2Pad = _val;
}

void MsgUpdateOver()
{
	g_u8TouchVal2Pad = 0;
	g_u8ObstVal2Pad = 0;
	g_u8Update2PAD_FLG = 0;
}


