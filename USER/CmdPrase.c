#include "CmdPrase.h"
#include "usart.h"
#include "stmflash.h"
#include "usart.h"
#include "string.h"
#include "obst.h"
#include "gpio.h"

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

//void SetObst( uint8_t dis)
//{
//	if(dis == 0)
//	{	
//		
//	}
//	else
//	{
//		
//	}
////	CMDBlkTypeDef _blk;
////	uint8_t u8CheckSum = 0;
////	memset(&_blk, 0, sizeof(_blk));
////	
////	_blk.CMDBodyBlk.u8HeadFlag = CMD_HEAD;
////	
////	_blk.CMDBodyBlk.u8Angel = CMD_NONE;
////	
////	_blk.CMDBodyBlk.u8ARFlag = CMD_NONE;
////	
////	_blk.CMDBodyBlk.u8CMDId = CMD_OBST;
////	
////	_blk.CMDBodyBlk.u8DirFlag = CMD_NONE;
////	
////	_blk.CMDBodyBlk.u8ExpressFlag = CMD_NONE;
////	
////	_blk.CMDBodyBlk.u8LimitFlag = CMD_NONE;
////	
////	_blk.CMDBodyBlk.u8ObstDis0 = dis0 ;
////	_blk.CMDBodyBlk.u8ObstDis1 = dis1 ;
////	_blk.CMDBodyBlk.u8ObstDis2 = dis2;
////	_blk.CMDBodyBlk.u8ObstFlag = obst_id;
////	
////	_blk.CMDBodyBlk.u8SpeedH = CMD_NONE;
////	_blk.CMDBodyBlk.u8SpeedL = CMD_NONE;
////	
////	_blk.CMDBodyBlk.u8ErrMsg = CMD_NONE;
////	
////	_blk.CMDBodyBlk.u8ForceMsg = CMD_NONE;
////	
////	_blk.CMDBodyBlk.u8TailFlag = CMD_TAIL;
////	
////	u8CheckSum = GetCheckSum(_blk.u8CMDBodyByte, 17) ;
////	
////	_blk.CMDBodyBlk.u8CheckSum = u8CheckSum;
////	
////	RingBuffer_InsertMult(&g_tUSART3TxRingBuf, _blk.u8CMDBodyByte, 17);
//}

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

void Uart3_heartbeat(void)
{
	uint8_t data[17] = {0};
	uint8_t _dis[3] = {0};
	CMDBlkTypeDef cmd_blk;
	uint8_t index = 0;
	if( RingBuffer_GetCount(&g_tUSART3TxRingBuf) > 0 )
	{
		memset(&data,0,17);
		if( RingBuffer_PopMult( &g_tUSART3TxRingBuf, data, 17) )
		{
			USART3_SendStrings(data,17);
		}
	}

	if( RingBuffer_GetCount(&g_tUSART3RxRingBuf) > 0 )
	{
		memset(&cmd_blk,0,sizeof(cmd_blk));
		if( RingBuffer_PopMult( &g_tUSART3RxRingBuf, cmd_blk.u8CMDBodyByte, 17) )
		{
			if( (cmd_blk.CMDBodyBlk.u8HeadFlag == 0x02) &&(cmd_blk.CMDBodyBlk.u8TailFlag == 0x03) )
			{
				memset(g_u8SonarDis,0,3);
				g_u8ObstFlg &= 0x80;
				g_u8SonarDis[0] = cmd_blk.CMDBodyBlk.u8ObstDis0;
				g_u8SonarDis[1] = cmd_blk.CMDBodyBlk.u8ObstDis1;
				g_u8SonarDis[2] = cmd_blk.CMDBodyBlk.u8ObstDis2;
				
				for(index=0; index<SONAR_CH_CONUT; index++)
				{
					if(g_u8SonarDis[index] < g_u8ObstDis)
					{
						g_u8ObstFlg |= 1 <<(index);
					}
				}
				
				
			}
			else
			{
				RingBuffer_Flush(&g_tUSART3RxRingBuf);
			}
			
			
		}
		else
		{
			RingBuffer_Flush(&g_tUSART3RxRingBuf);
		}
	}
	
	if( (g_u8ObstFlg &0x07)> 0)
	{
		TEST_LED = 0;
	}
	else
	{
		TEST_LED = 1;
	}
	
	
	
}



