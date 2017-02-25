#include "CmdPrase.h"
#include "usart.h"
#include "stmflash.h"
#include "usart.h"
#include "string.h"
CMDBlkTypeDef CMDBlk;

uint8_t GetCheckSum(uint8_t *uBuff, uint8_t uBuffLen) 
{ 
	uint8_t  i, uSum = 0; 

	for(i = 0; i < uBuffLen; i++) 
	{ 
		uSum = uSum + uBuff[i]; 
	} 
	uSum = (~uSum) + 1; 

	return uSum; 
} 

void SetARAct( char ar, char dir, uint16_t Angel, uint16_t Speed)
{
	CMDBlkTypeDef _blk;
	uint8_t u8CheckSum = 0;
	memset(&_blk, 0, sizeof(_blk));
	
	_blk.CMDBodyBlk.u8HeadFlag = CMD_HEAD;
	
	_blk.CMDBodyBlk.u8Angel = Angel ;
	
	_blk.CMDBodyBlk.u8ARFlag = CMD_HEAD;
	
	_blk.CMDBodyBlk.u8CMDId = CMD_ACT;
	
	_blk.CMDBodyBlk.u8DirFlag = dir;
	
	_blk.CMDBodyBlk.u8ExpressFlag = CMD_NONE;
	
	_blk.CMDBodyBlk.u8LimitFlag = CMD_NONE;
	
	_blk.CMDBodyBlk.u8ObstDis0 = CMD_NONE;
	_blk.CMDBodyBlk.u8ObstDis1 = CMD_NONE;
	_blk.CMDBodyBlk.u8ObstDis1 = CMD_NONE;
	
	_blk.CMDBodyBlk.u8ObstFlag = CMD_NONE;
	
	_blk.CMDBodyBlk.u8SpeedH = CMD_NONE;
	_blk.CMDBodyBlk.u8SpeedL = CMD_NONE;
	
	_blk.CMDBodyBlk.u8ErrMsg = CMD_NONE;
	
	_blk.CMDBodyBlk.u8ForceMsg = CMD_NONE;
	
	_blk.CMDBodyBlk.u8TailFlag = CMD_TAIL;
	
	u8CheckSum = GetCheckSum(_blk.u8CMDBodyByte, 17) ;
	
	_blk.CMDBodyBlk.u8CheckSum = u8CheckSum;
	
	RingBuffer_InsertMult(&g_tUSART3TxRingBuf, _blk.u8CMDBodyByte, 17);
}

void SetObst(uint8_t obst_id, uint8_t dis0, uint8_t dis1, uint8_t dis2)
{
	CMDBlkTypeDef _blk;
	uint8_t u8CheckSum = 0;
	memset(&_blk, 0, sizeof(_blk));
	
	_blk.CMDBodyBlk.u8HeadFlag = CMD_HEAD;
	
	_blk.CMDBodyBlk.u8Angel = CMD_NONE;
	
	_blk.CMDBodyBlk.u8ARFlag = CMD_NONE;
	
	_blk.CMDBodyBlk.u8CMDId = CMD_OBST;
	
	_blk.CMDBodyBlk.u8DirFlag = CMD_NONE;
	
	_blk.CMDBodyBlk.u8ExpressFlag = CMD_NONE;
	
	_blk.CMDBodyBlk.u8LimitFlag = CMD_NONE;
	
	_blk.CMDBodyBlk.u8ObstDis0 = dis0 ;
	_blk.CMDBodyBlk.u8ObstDis1 = dis1 ;
	_blk.CMDBodyBlk.u8ObstDis2 = dis2;
	_blk.CMDBodyBlk.u8ObstFlag = obst_id;
	
	_blk.CMDBodyBlk.u8SpeedH = CMD_NONE;
	_blk.CMDBodyBlk.u8SpeedL = CMD_NONE;
	
	_blk.CMDBodyBlk.u8ErrMsg = CMD_NONE;
	
	_blk.CMDBodyBlk.u8ForceMsg = CMD_NONE;
	
	_blk.CMDBodyBlk.u8TailFlag = CMD_TAIL;
	
	u8CheckSum = GetCheckSum(_blk.u8CMDBodyByte, 17) ;
	
	_blk.CMDBodyBlk.u8CheckSum = u8CheckSum;
	
	RingBuffer_InsertMult(&g_tUSART3TxRingBuf, _blk.u8CMDBodyByte, 17);
}

void SetExpress( char express_id)
{
	
	CMDBlkTypeDef _blk;
	uint8_t u8CheckSum = 0;
	memset(&_blk, 0, sizeof(_blk));
	_blk.CMDBodyBlk.u8HeadFlag = CMD_HEAD;
	
	
	_blk.CMDBodyBlk.u8Angel = CMD_NONE;
	
	_blk.CMDBodyBlk.u8ARFlag = CMD_HEAD;
	
	_blk.CMDBodyBlk.u8CMDId = CMD_EXPRESS;
	
	_blk.CMDBodyBlk.u8DirFlag = CMD_NONE;
	
	_blk.CMDBodyBlk.u8ExpressFlag = express_id;
	
	_blk.CMDBodyBlk.u8LimitFlag = CMD_NONE;
	
	_blk.CMDBodyBlk.u8ObstDis0 = CMD_NONE;
	_blk.CMDBodyBlk.u8ObstDis1 = CMD_NONE;
	_blk.CMDBodyBlk.u8ObstDis2 = CMD_NONE;
	
	_blk.CMDBodyBlk.u8ObstFlag = CMD_NONE;
	
	_blk.CMDBodyBlk.u8SpeedH = CMD_NONE;
	_blk.CMDBodyBlk.u8SpeedL = CMD_NONE;
	
	_blk.CMDBodyBlk.u8ErrMsg = CMD_NONE;
	
	_blk.CMDBodyBlk.u8ForceMsg = CMD_NONE;
	
	_blk.CMDBodyBlk.u8TailFlag = CMD_TAIL;
	
	u8CheckSum = GetCheckSum(_blk.u8CMDBodyByte, 17) ;
	
	_blk.CMDBodyBlk.u8CheckSum = u8CheckSum;
	RingBuffer_InsertMult(&g_tUSART3TxRingBuf, _blk.u8CMDBodyByte, 17);
}

void SetLimit( char _cmd)
{
	uint8_t u8CheckSum = 0;
	CMDBlkTypeDef _blk;
	memset(&_blk, 0, sizeof(_blk));
	
	_blk.CMDBodyBlk.u8HeadFlag = CMD_HEAD;
	
	_blk.CMDBodyBlk.u8Angel = CMD_NONE;
	
	_blk.CMDBodyBlk.u8ARFlag = CMD_NONE;
	
	_blk.CMDBodyBlk.u8CMDId = CMD_LIMIT;
	
	_blk.CMDBodyBlk.u8DirFlag = CMD_NONE;
	
	_blk.CMDBodyBlk.u8ExpressFlag = CMD_NONE;
	
	_blk.CMDBodyBlk.u8LimitFlag = _cmd;
	
	_blk.CMDBodyBlk.u8ObstDis0 = CMD_NONE;
	_blk.CMDBodyBlk.u8ObstDis1 = CMD_NONE;
	_blk.CMDBodyBlk.u8ObstFlag = CMD_NONE;
	
	_blk.CMDBodyBlk.u8SpeedH = CMD_NONE;
	_blk.CMDBodyBlk.u8SpeedL = CMD_NONE;
	
	_blk.CMDBodyBlk.u8ErrMsg = CMD_NONE;
	
	_blk.CMDBodyBlk.u8ForceMsg = CMD_NONE;
	
	_blk.CMDBodyBlk.u8TailFlag = CMD_TAIL;
	
	u8CheckSum = GetCheckSum(_blk.u8CMDBodyByte, 17) ;
	
	_blk.CMDBodyBlk.u8CheckSum = u8CheckSum;
	
	RingBuffer_InsertMult(&g_tUSART3TxRingBuf, _blk.u8CMDBodyByte, 17);
}
