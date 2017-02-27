#ifndef __CMDPRASE_H
#define __CMDPRASE_H

#include "sys.h"
#include "ring_buffer.h"

#define CMD_COM COM1

#define CMD_HEAD	0x02
#define CMD_TAIL	0x03
#define CMD_NONE	0x00

enum _cmd_fun
{
	CMD_ACT = 1,
	CMD_LIMIT,
	CMD_OBST,
	CMD_EXPRESS,
};

enum _run_dir
{
	STOP_FLG = 1,
	LEFT_OR_UP,
	RIGHT_OR_DOWN,
};

enum _ar_id
{
	AR_HEAD = 1,			
	AR_WAIST,				
	AR_LARM,				
	AR_RARM, 				
};
enum _cmd_obst_id
{
	EN_ALL = 1,
	DISEN_ALL,
	DISEN_CH1,
	DISEN_CH2,
	DISEN_CH3,
	SET_DIS,
};

enum _act_cmd
{
	CMD_HEAD_TURN_LEFT = 1,
	CMD_HEAD_TURN_RIGHT,
	CMD_HEAD_STOP,
	
	CMD_WAIST_TURN_LEFT,
	CMD_WAIST_TURN_RIGHT,
	CMD_WAIST_STOP,
	
	CMD_LARM_TURN_UP,
	CMD_LARM_TURN_DOWN,
	CMD_LARM_STOP,
	
	CMD_RARM_TURN_UP,
	CMD_RARM_TURN_DOWN,
	CMD_RARM_STOP,
	
	CMD_ALL_BCAK,
};

enum _limit_cmd
{
	CMD_SET_HEAD_DEF = 1,
	CMD_SET_HEAD_L_LIMIT,
	CMD_SET_HEAD_R_LIMIT,
	
	CMD_SET_WAIST_DEF,
	CMD_SET_WAIST_L_LIMIT,
	CMD_SET_WAIST_R_LIMIT,
	
	CMD_SET_LARM_DEF,
	CMD_SET_LARM_U_LIMIT,
	CMD_SET_LARM_D_LIMIT,
	
	CMD_SET_RARM_DEF,
	CMD_SET_RARM_U_LIMIT,
	CMD_SET_RARM_D_LIMIT,
};

enum _express_cmd
{
	CMD_EXPRESS1 = 1,
	CMD_EXPRESS2,
	CMD_EXPRESS3,
	
	CMD_EXPRESS4,
	CMD_EXPRESS5,
	CMD_EXPRESS6,
	
	CMD_EXPRESS7,
	CMD_EXPRESS8,
	CMD_EXPRESS9,
	
	CMD_EXPRESS10,
	CMD_EXPRESS11,
	CMD_EXPRESS12,
};

typedef __packed struct
{
	uint8_t u8HeadFlag;		//֡ͷ		
	uint8_t u8CMDId;		//����ID
	uint8_t u8LimitFlag;	//����ѡ������
	uint8_t u8ObstFlag;		//������������
	uint8_t u8ExpressFlag;	//����ѡ������
	uint8_t u8ARFlag;		//�ϰ���������
	uint8_t u8DirFlag;		//�ϰ���������
	uint8_t u8Angel;		//�Ƕȵ�λ
	uint8_t u8SpeedH;		//�ٶȸ�λ
	uint8_t u8SpeedL;		//�ٶȵ�λ
	uint8_t u8ObstDis0;		//�ϰ�0
	uint8_t u8ObstDis1;		//�ϰ�1
	uint8_t u8ObstDis2;		//�ϰ�2
	uint8_t u8ErrMsg;		//����λ
	uint8_t u8ForceMsg;		//����λ
	uint8_t u8CheckSum;		//У���
	uint8_t u8TailFlag;		//֡β
} CMDTypeDef; 

typedef union
{
	CMDTypeDef CMDBodyBlk;
	uint8_t u8CMDBodyByte[17];
} CMDBlkTypeDef;

extern CMDBlkTypeDef CMDBlk;

uint8_t GetCheckSum(uint8_t *uBuff, uint8_t uBuffLen) ;
void CMDPrase(void);
void SetARAct( char ar, char dir, uint16_t Angel, uint16_t Speed);
void SetExpress( char express_id);
void SetObst(uint8_t dis);
void SetLimit( char _cmd);
#endif
