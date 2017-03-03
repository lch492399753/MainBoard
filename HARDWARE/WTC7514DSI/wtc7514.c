#include "wtc7514.h"
#include "delay.h"
#include "sys.h"

uint8_t WT7514_ADDR[4] = {0x70,0x74,0x78,0x7c};

//初始化IIC接口

u16 WTC7514_ReadTwoByte(u8 _addr)
{
    u8 temp1=0;
    u8 temp2=0;
    u16 rlt = 0 ;
    IIC_Start();
    IIC_Send_Byte(_addr + 1);       //发送器件地址DEVICE_ADDR,写数据
    IIC_Wait_Ack();
    temp1=IIC_Read_Byte(1);                 //读1个字节，ack=1时，发送ACK，ack=0，发送nACK
    temp2=IIC_Read_Byte(0);                 //读1个字节，ack=1时，发送ACK，ack=0，发送nACK
    IIC_Stop();//产生一个停止条件
    rlt=temp1;
    rlt<<=8;
    rlt+=temp2;
    return rlt;
}


//在WTC7514指定地址写入一个数据
//WriteAddr  :写入数据的目的地址
//DataToWrite:要写入的数据
void WTC7514_SetLevel(uint8_t _addr, uint8_t level)
{
    IIC_Start();
    IIC_Send_Byte(_addr);   //发送器件地址DEVICE_ADDR,写数据
    IIC_Wait_Ack();
    IIC_Send_Byte(level);
    IIC_Wait_Ack();
    IIC_Stop();                       //产生一个停止条件
}

void WTC7514_SetLevel_head(u8 level)
{
    IIC_Start();
    IIC_Send_Byte(DEVICE_ADDR_HEAD);   //发送器件地址DEVICE_ADDR,写数据
    IIC_Wait_Ack();
    IIC_Send_Byte(level);
    IIC_Wait_Ack();
    IIC_Stop();                       //产生一个停止条件
}

u16 WTC7514_ReadTwoByte_head(void)
{
    u8 temp1=0;
    u8 temp2=0;
    u16 rlt;
    IIC_Start();
    IIC_Send_Byte(DEVICE_ADDR_HEAD+1);       //发送器件地址DEVICE_ADDR,写数据
    IIC_Wait_Ack();
    temp1=IIC_Read_Byte(1);                 //读1个字节，ack=1时，发送ACK，ack=0，发送nACK
    temp2=IIC_Read_Byte(0);                 //读1个字节，ack=1时，发送ACK，ack=0，发送nACK
    IIC_Stop();//产生一个停止条件
    rlt=temp1;
    rlt<<=8;
    rlt+=temp2;
    return rlt;
}

uint16_t g_u16TouchVal[WT7514_CH_CNT] = {0};
uint16_t g_u16TouchValPre[WT7514_CH_CNT] = {0};
uint16_t g_u16TouchValue = 0;
void touch_heartbeat()
{
	uint8_t index = 0;
	
	for(index=0; index < WT7514_CH_CNT; index++)
	{
		g_u16TouchVal[index] = WTC7514_ReadTwoByte( WT7514_ADDR[index] );
		
		if(g_u16TouchVal[index] != g_u16TouchValPre[index])
		{
			
			switch( g_u16TouchVal[index] )
			{
				case WTC7514_TOUCHED_NONE:
				{
					g_u16TouchValue = TOUCH_NONE;
					break;
				}

				case WTC7514_TOUCHED_CH(8):
				{
					g_u16TouchValue = TOUCH_MOUTH;
					//walking_flag=0;
					break;
				}

				case WTC7514_TOUCHED_CH(9):
				{
					g_u16TouchValue = TOUCH_MOUTH;
					//walking_flag=0;
					break;
				}

				case WTC7514_TOUCHED_CH(10):
				{
					g_u16TouchValue = TOUCH_HEAD;
					//walking_flag=0;
					break;
				}

				case WTC7514_TOUCHED_CH(11):
				{
					g_u16TouchValue = TOUCH_HEAD;
					//walking_flag=0;
					break;
				}

				default:
					break;
			}
			//usart4Printf("{\"content\":\"00000%d000000000\"}",touch_val);
			g_u16TouchVal[index] = g_u16TouchValPre[index] ;
			g_u8Update2PAD_FLG = 1;
		}
	}
}

