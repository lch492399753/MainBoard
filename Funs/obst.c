
#include "obst.h"
#include "stmflash.h"

uint8_t g_u8SonarDis[SONAR_CH_CONUT] = {0};
uint8_t g_u8ObstDis = 0;
uint8_t g_u8ObstFlg = 0x80;
uint8_t g_u8IsOpenObst = 1;

static void SetObstDis(uint16_t _obst_dis );
static void SaveObstSta(uint16_t _sta);

void SetObstBit(uint8_t _ch)
{
	g_u8ObstFlg |= 1 << _ch;
}

void ResetObstBit(uint8_t _ch)
{
	g_u8ObstFlg &= ( (~(1 << _ch)) | 0x80 );
}

uint8_t GetObstBitSta()
{
	return g_u8ObstFlg;
}

void ObstDisFLASHCheck()
{
	
	uint8_t tmp1 = 0,tmp2 =0;
	
	tmp1 = (uint8_t) STMFLASH_ReadHalfWord(FLASH_OBST_BASE_ADDR );
	tmp2 = (uint8_t) STMFLASH_ReadHalfWord(FLASH_OBST_STA_ADDR );
	 
	if( tmp1 < 100)
	{
		g_u8ObstDis = tmp1;
	}
	else
	{
		g_u8ObstDis = OBST_BASE_DIS;
	}
	
	if( tmp2 != 0)
	{
		g_u8ObstFlg |= 0x80;
	}
	else
	{
		g_u8ObstFlg &= 0x7f;
	}
	
}

static void SetObstDis(uint16_t _obst_dis )
{
	uint8_t index =0;
	//for(index=0; index<SONAR_CH_CONUT; index++)
	{
		g_u8ObstDis = _obst_dis;
	}
	
	STMFLASH_Write(FLASH_OBST_BASE_ADDR ,&_obst_dis,1);
}

static void SaveObstSta(uint16_t _sta)
{
	STMFLASH_Write(FLASH_OBST_BASE_ADDR ,&_sta,1);
}

uint8_t IsObstOpen()
{
	if( g_u8ObstFlg & 0x80)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void CmdSetObst( uint8_t dis)
{
	if(dis <= 0)
	{
		g_u8ObstFlg &= 0x7F;
	}
	else
	{
		g_u8ObstFlg |= 0x80;
		SetObstDis(dis );
	}
	SaveObstSta(g_u8ObstFlg);
}
