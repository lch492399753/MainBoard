#include "wtc7514.h"
#include "delay.h"
#include "sys.h"

uint8_t WT7514_ADDR[4] = {0x70,0x74,0x78,0x7c};

//��ʼ��IIC�ӿ�
void WTC7514_Init(void)
{
    IIC_Init();
    IIC_Stop();//����һ��ֹͣ����
}

u16 WTC7514_ReadTwoByte(u8 _addr)
{
    u8 temp1=0;
    u8 temp2=0;
    u16 rlt = 0 ;
    IIC_Start();
    IIC_Send_Byte(_addr+1);       //����������ַDEVICE_ADDR,д����
    IIC_Wait_Ack();
    temp1=IIC_Read_Byte(1);                 //��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
    temp2=IIC_Read_Byte(0);                 //��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
    IIC_Stop();//����һ��ֹͣ����
    rlt=temp1;
    rlt<<=8;
    rlt+=temp2;
    return rlt;
}


//��WTC7514ָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ
//DataToWrite:Ҫд�������
void WTC7514_SetLevel(uint8_t _addr, uint8_t level)
{
    IIC_Start();
    IIC_Send_Byte(_addr);   //����������ַDEVICE_ADDR,д����
    IIC_Wait_Ack();
    IIC_Send_Byte(level);
    IIC_Wait_Ack();
    IIC_Stop();                      //����һ��ֹͣ����
}


uint16_t g_u16TouchVal[WT7514_CH_CNT] = {0};
uint16_t g_u16TouchValPre[WT7514_CH_CNT] = {0};

void touch_heartbeat()
{
	uint8_t index = 0;
	uint16_t touchval = 0;
	
	for(index=0; index < WT7514_CH_CNT; index++)
	{
		
		g_u16TouchVal[index] = WTC7514_ReadTwoByte( WTC7514_ADDR );
		if(g_u16TouchVal[index] != g_u16TouchValPre[index])
		{
			TEST_LED_ON();
			switch( g_u16TouchVal[index] )
			{
				case WTC7514_TOUCHED_NONE:
				{
					touchval = TOUCH_NONE;
					break;
				}

				case WTC7514_TOUCHED_CH(4):
				{
					touchval = TOUCH_MOUTH;
					//walking_flag=0;
					break;
				}

				case WTC7514_TOUCHED_CH(5):
				{
					touchval = TOUCH_MOUTH;
					//walking_flag=0;
					break;
				}

				case WTC7514_TOUCHED_CH(6):
				{
					touchval = TOUCH_HEAD;
					//walking_flag=0;
					break;
				}

				case WTC7514_TOUCHED_CH(7):
				{
					touchval = TOUCH_HEAD;
					//walking_flag=0;
					break;
				}
				
				case WTC7514_TOUCHED_CH(8):
				{
					touchval = TOUCH_HEAD;
					//walking_flag=0;
					break;
				}
				
				case WTC7514_TOUCHED_CH(9):
				{
					touchval = TOUCH_HEAD;
					//walking_flag=0;
					break;
				}
				
				case WTC7514_TOUCHED_CH(10):
				{
					touchval = TOUCH_HEAD;
					//walking_flag=0;
					break;
				}
				
				case WTC7514_TOUCHED_CH(11):
				{
					touchval = TOUCH_HEAD;
					//walking_flag=0;
					break;
				}
				
				default:
					break;
			}
			//usart4Printf("{\"content\":\"00000%d000000000\"}",touch_val);
			touchval = TOUCH_HEAD;
			g_u16TouchValPre[index] = g_u16TouchVal[index] ;
			SetTouchVal(touchval);
			SetMsgUpdateFlg();
		}
		else
		{
			TEST_LED_OFF();
		}
	}
}

