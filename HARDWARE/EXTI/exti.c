#include "exti.h"
#include "PID.h"

#include "delay.h"
#include "usart.h"
#include "jiexi.h"
#include "gpio.h"
#include "pwm.h"
//#include "beep.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEKս��STM32������
//�ⲿ�ж� ��������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
//�ⲿ�ж�0�������

extern u8 Obstacte;
u8 bizhang_flag=0;
short p_speed;
void EXTIX_Init(void)
{

    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);



    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource11);

    EXTI_InitStructure.EXTI_Line=EXTI_Line11;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);


    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


}

//�ⲿ�ж�0�������
void EXTI15_10_IRQHandler(void)
{
    u16 i=0;

    while(Obstacles==1)      //WK_UP����
    {
        Speed_Right(0,speed_zhuanhuan(curr_left[0]));
        Speed_Left(0,speed_zhuanhuan(curr_right[0]));
        Obstacte=1;
        bizhang_flag=1;
    }
    EXTI_ClearITPendingBit(EXTI_Line11); //���LINE0�ϵ��жϱ�־λ
    SPEEDL=20;
    SPEEDR=20;
    // usart2Printf("exti :  \r\n");
    Obstacte=0;
}
void EXTI_Set_Int(u8 en)
{

    EXTI_InitTypeDef EXTI_InitStructure;

    EXTI_ClearITPendingBit(EXTI_Line11);  //���EXTI11��·����λ

    if(en)
    {
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    }
    else
    {
        EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    }
    EXTI_Init(&EXTI_InitStructure);        //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

}
////�ⲿ�ж�2�������
//void EXTI2_IRQHandler(void)
//{
//  delay_ms(10);//����
//  if(KEY2==0)   //����KEY2
//  {
//      LED0=!LED0;
//  }
//  EXTI_ClearITPendingBit(EXTI_Line2);  //���LINE2�ϵ��жϱ�־λ
//}
////�ⲿ�ж�3�������
//void EXTI3_IRQHandler(void)
//{
//  delay_ms(10);//����
//  if(KEY1==0)  //����KEY1
//  {
//  //  LED1=!LED1;
//  }
//  EXTI_ClearITPendingBit(EXTI_Line3);  //���LINE3�ϵ��жϱ�־λ
//}

//void EXTI4_IRQHandler(void)
//{
////    delay_ms(10);//����
//  //if(KEY0==0)    //����KEY0
//  {
//      //LED0=!LED0;
//      //LED1=!LED1;
//  }
//  //EXTI_ClearITPendingBit(EXTI_Line4);  //���LINE4�ϵ��жϱ�־λ
//}
