#ifndef __POSEWITHVOICE_FUN_H
#define __POSEWITHVOICE_FUN_H

#include "sys.h"
#include "myiic.h"


enum _turndir
{
	TURN_LEFT_FLG = 1,
	TURN_RIGHT_FLG,
};

typedef struct
{
	uint8_t u8RunFlg;
	uint8_t u8RunDir;
	uint8_t	u8IsWakeUp;
	uint16_t u16WakeAng;
	uint16_t u16DefPos;
	uint16_t u16CurrPos;
	uint16_t u16TargetPos;
	uint16_t u16WakeDeltaAngel;
}VOICE_CTR_DEF;

extern VOICE_CTR_DEF g_tVoiceCtl;
void TurnWithVoice(int16_t _angel);
int16_t RobotGetPos(void);
#endif

