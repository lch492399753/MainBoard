#include "wtc7514.h"
#include "delay.h"



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
    IIC_Send_Byte(_addr + 1);       //����������ַDEVICE_ADDR,д����
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
    IIC_Stop();                       //����һ��ֹͣ����
}

void WTC7514_SetLevel_head(u8 level)
{
    IIC_Start();
    IIC_Send_Byte(DEVICE_ADDR_HEAD);   //����������ַDEVICE_ADDR,д����
    IIC_Wait_Ack();
    IIC_Send_Byte(level);
    IIC_Wait_Ack();
    IIC_Stop();                       //����һ��ֹͣ����
}

u16 WTC7514_ReadTwoByte_head(void)
{
    u8 temp1=0;
    u8 temp2=0;
    u16 rlt;
    IIC_Start();
    IIC_Send_Byte(DEVICE_ADDR_HEAD+1);       //����������ַDEVICE_ADDR,д����
    IIC_Wait_Ack();
    temp1=IIC_Read_Byte(1);                 //��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
    temp2=IIC_Read_Byte(0);                 //��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
    IIC_Stop();//����һ��ֹͣ����
    rlt=temp1;
    rlt<<=8;
    rlt+=temp2;
    return rlt;
}
