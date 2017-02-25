#include "iFlytek.h"
#include "delay.h"
#include "usart.h"
#include "PoseWithVoice_Fun.h"

#if IFLYTEK_WORK_MODE

#define IFLY_READ_ADDR 0x8F
#define IFLY_WRITE_ADDR 0x8E

#define READ_WAKE_ANGEL_CMD 0x10

uint16_t g_u16iFlyTekAngel = 0xffff;
uint16_t g_u16iFlyTekAngelPre = 0xffff;
uint16_t g_u16iFlyTekStart = 0x00;

void IFlyTekReset()
{
	IIC_Start();
    IIC_Send_Byte(IFLY_WRITE_ADDR);       //发送器件地址DEVICE_ADDR,写数据
    //IIC_Wait_Ack();
	IIC_Send_Byte(0x00);       //发送器件地址DEVICE_ADDR,写数据
	//IIC_Wait_Ack();
	IIC_Send_Byte(0x00);       //发送器件地址DEVICE_ADDR,写数据
	//IIC_Wait_Ack();
	IIC_Send_Byte(0x11);       //发送器件地址DEVICE_ADDR,写数据
	//IIC_Wait_Ack();
	IIC_Send_Byte(0x00);       //发送器件地址DEVICE_ADDR,写数据
	//IIC_Wait_Ack();
	IIC_Send_Byte(0x00);       //发送器件地址DEVICE_ADDR,写数据
	//IIC_Wait_Ack();
	IIC_Stop();
}



uint8_t GetIFlyTekWorkSta()
{
	uint8_t tmp1=0,tmp2=0,tmp3=0,tmp4=0;
	IIC_Start();
    IIC_Send_Byte(IFLY_WRITE_ADDR);       //发送器件地址DEVICE_ADDR,写数据
    IIC_Wait_Ack();
	IIC_Send_Byte(0x00);       //发送器件地址DEVICE_ADDR,写数据
	IIC_Start();
	IIC_Send_Byte(IFLY_READ_ADDR ); 
	tmp1 = IIC_Read_Byte(1);
	tmp2 = IIC_Read_Byte(1);
	tmp3 = IIC_Read_Byte(1);
	tmp4 = IIC_Read_Byte(0);
	IIC_Stop();
	return tmp1;
}

void IFlyTekInit()
{

}

void CheckWakeUpSta()
{
	IIC_Start();
    IIC_Send_Byte(IFLY_WRITE_ADDR);       //发送器件地址DEVICE_ADDR,写数据
    IIC_Wait_Ack();
	IIC_Send_Byte(0x00);       //发送器件地址DEVICE_ADDR,写数据
	IIC_Wait_Ack();
	IIC_Send_Byte(0x00);       //发送器件地址DEVICE_ADDR,写数据
	IIC_Wait_Ack();
	IIC_Send_Byte(READ_WAKE_ANGEL_CMD);       //发送器件地址DEVICE_ADDR,写数据
	IIC_Wait_Ack();
	IIC_Send_Byte(0x00);       //发送器件地址DEVICE_ADDR,写数据
	IIC_Wait_Ack();
	IIC_Send_Byte(0x00);  
	IIC_Wait_Ack();	
	IIC_Stop();
}
	
uint8_t ReadWakeUpSta()
{
	uint8_t tmp1 =0 ,tmp2 = 0, tmp3=0,tmp4=0;
	IIC_Start();
	IIC_Send_Byte(IFLY_WRITE_ADDR);       //发送器件地址DEVICE_ADDR,写数据
	IIC_Wait_Ack();
	IIC_Send_Byte(0x00);       //发送器件地址DEVICE_ADDR,写数据
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(IFLY_READ_ADDR ); 
	IIC_Wait_Ack();
	tmp1 = IIC_Read_Byte(1);
	tmp2 = IIC_Read_Byte(1);
	tmp3 = IIC_Read_Byte(1);
	tmp4 = IIC_Read_Byte(0);
	IIC_Stop();
	
	return tmp3;
}

uint16_t ReadWakeUpAngel()
{
	uint8_t tmp1 =0 ,tmp2 = 0, tmp3=0,tmp4=0;
	uint16_t angel = 0;
	IIC_Start();
	IIC_Send_Byte(IFLY_WRITE_ADDR);       //发送器件地址DEVICE_ADDR,写数据
	IIC_Wait_Ack();
	IIC_Send_Byte(0x01);       //发送器件地址DEVICE_ADDR,写数据
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(IFLY_READ_ADDR ); 
	IIC_Wait_Ack();
	tmp1 = IIC_Read_Byte(1);
	tmp2 = IIC_Read_Byte(1);
	tmp3 = IIC_Read_Byte(1);
	tmp4 = IIC_Read_Byte(0);
	IIC_Stop();
	
	angel = ((tmp2<<8) & 0xff00)| tmp1;
	return angel;
}

#else
void GetIFlyTekAngel()
{
	int16_t tmp =0;
	if( (g_u8Usart2RecFlg & 0x02) )
	{
		g_u8Usart2RecFlg = 0;
			
		switch(g_u8Usart2RecLen)
		{
			case 16:
			{
				tmp = (USART2_RX_BUF[14] - 0x30);
				g_u8Usart2RecLen = 0;
				break;
			}
			case 17:
			{
				tmp = (USART2_RX_BUF[14] - 0x30) *10  +
										 (USART2_RX_BUF[15] - 0x30) ;
				g_u8Usart2RecLen = 0;
				break;
			}
			case 18:
			{
				tmp = (USART2_RX_BUF[14] - 0x30) *100 +
										 (USART2_RX_BUF[15] - 0x30) *10  +
										 (USART2_RX_BUF[16] - 0x30) ;
				g_u8Usart2RecLen = 0;
				break;
			}
			default:
				break;
		}

		if((tmp <0 ) || (tmp>360))
		{
			return;
			g_tVoiceCtl.u8IsWakeUp = 0 ;
		}
			
		g_tVoiceCtl.u16WakeAng = tmp;
		g_tVoiceCtl.u8IsWakeUp = 1;
		memset(USART2_RX_BUF,0,USART2_RX_STA);
			//USART2_RX_STA = 0x0000;
	}
}
	
#endif

