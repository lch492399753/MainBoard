#include "remote.h"
#include "delay.h"
#include "usart.h"
#include "PID.h"
#include "jiexi.h"

static  u8 t1=0,t2=0,wtj1=0,wtj2=0,pos_flag_j=0,w_ok=1;
extern u32 delay_time;
extern u8 Table_Number,wifi;

void Remote(void);
void Remote_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  // T8OC1 ????,T8OC2 ????,
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC,GPIO_Pin_6);


//TIM_OCInitTypeDef TIM_OCInitStructure;



    TIM_TimeBaseStructure.TIM_Period = 10000;                                     //??65.536ms
    TIM_TimeBaseStructure.TIM_Prescaler = 72-1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);


    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_ICInit(TIM8, &TIM_ICInitStructure);

    TIM_Cmd(TIM8, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(TIM8, TIM_IT_Update|TIM_IT_CC1, ENABLE);

}

u8  RmtSta=0;
u16 Dval;
u32 RmtRec=0;
u8  RmtCnt=0;

void  TIM8_CC_IRQHandler (void)
{

    if(TIM_GetITStatus(TIM8,TIM_IT_Update)!=RESET)
    {
        //  printf("8\r\n");
        if(RmtSta&0x80)
        {
            RmtSta&=~0X10;                          //ȡ���������Ѿ���������
            if((RmtSta&0X0F)==0X00)RmtSta|=1<<6;    //����Ѿ����һ�ΰ����ļ�ֵ��Ϣ�ɼ�
            if((RmtSta&0X0F)<14)RmtSta++;
            else
            {
                RmtSta&=~(1<<7);                    //���������ʶ
                RmtSta&=0XF0;                       //��ռ�����
            }
        }
    }
    if(TIM_GetITStatus(TIM8,TIM_IT_CC1)!=RESET)
    {


        if(RDATA)
        {

            //printf("%d\r\n",RDATA);
            TIM_OC1PolarityConfig(TIM8,TIM_ICPolarity_Falling);                     //CC4P=1    ����Ϊ�½��ز���
            TIM_SetCounter(TIM8,0);                         //��ն�ʱ��ֵ
            //  printf("d\r\n");
            RmtSta|=0X10;                           //����������Ѿ�������
        }
        else  //�½��ز���
        {
            //printf("a\r\n");
            Dval=TIM_GetCapture1(TIM8);
            //  printf("%d\r\n",Dval);         //��ȡCCR4Ҳ������CC4IF��־λ
            TIM_OC1PolarityConfig(TIM8,TIM_ICPolarity_Rising);              //CC4P=0    ����Ϊ�����ز���
            if(RmtSta&0X10)                         //���һ�θߵ�ƽ����
            {
                if(RmtSta&0X80)//���յ���������
                {

                    if(Dval>300&&Dval<800)          //560Ϊ��׼ֵ,560us
                    {
                        RmtRec<<=1;                 //����һλ.
                        RmtRec|=0;                  //���յ�0
                    }
                    else if(Dval>1400&&Dval<1800)   //1680Ϊ��׼ֵ,1680us
                    {
                        RmtRec<<=1;                 //����һλ.
                        RmtRec|=1;                  //���յ�1
                    }
                    else if(Dval>2200&&Dval<2600)   //�õ�������ֵ���ӵ���Ϣ 2500Ϊ��׼ֵ2.5ms
                    {
                        RmtCnt++;                   //������������1��
                        RmtSta&=0XF0;               //��ռ�ʱ��
                    }
                }
                else if(Dval>4200&&Dval<4700)       //4500Ϊ��׼ֵ4.5ms
                {
                    RmtSta|=1<<7;                   //��ǳɹ����յ���������
                    RmtCnt=0;                       //�����������������
                }
            }
            RmtSta&=~(1<<4);
        }
    }

    TIM_ClearITPendingBit(TIM8,TIM_IT_Update|TIM_IT_CC1);

    Remote();
}





//void Waist_Turn_jiaoyan(u8 aa,u8 bb)
//{
//
//      u8 position;
//
//  static  u32 a,a1,b=0,b1=0;
//  position=GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9);
////if(w_ok==1)
//  if(wtj2==0) {
//    b=delay_time;
//      wtj2=1;
//  }
//  if(wtj2==1){
//  b1=delay_time;
//  }
//  if(b1-b>=3000){
//      pos_flag_j=1;
//      wtj2=0;
//  }
//      if((position==1)&&(pos_flag_j==1))
//  {
//    Stop();
//  //  while(1);
//      //ii--;
//      w_ok=0;
//  pos_flag_j=0;
//   //ifposition==0
//  }
//  else
//  {
//      //if(position==0)wei=1;
//      Waist_In1=aa;
//      Waist_In2=bb;
//      TIM_SetCompare3(TIM3,400);
//      TIM_SetCompare4(TIM3,400);
//  }
//}

u8 Remote_Scan(void)
{
    u8 sta=0;
    u8 t1,t2;
    if(RmtSta&(1<<6))
    {
        t1=RmtRec>>24;
        t2=(RmtRec>>16);
        if((t1==0X33)&&t2==0Xb8)
        {
            t1=RmtRec>>8;
            t2=RmtRec;
            if(t1==(u8)~t2)sta=t1;
        }
        if((sta==0)||((RmtSta&0X80)==0))
        {
            RmtSta&=~(1<<6);
            RmtCnt=0;
        }
    }
    return sta;
}

void Remote(void)
{
    static u16 key_value,pre_key=0;
    key_value=Remote_Scan();
    if(key_value)
    {
        switch(key_value)
        {
            case 8:
                ;
                break;
            case 136:
                ;
                break;
            case 72:
                usart3Printf("{3}");
                break;
            case 200:
                usart3Printf("{4}");
                break;
            case 40:
                usart3Printf("{5}");
                break;
            case 168:
                usart3Printf("{6}");
                break;
            case 104:
                usart3Printf("{7}");
                break;
            case 232:
                usart3Printf("{8}");
                break;
            case 24:
                usart3Printf("{9}");
                break;
            case 160:
                usart3Printf("{1}");
                break;
            case 96:
                usart3Printf("{2}");
                break;
            case 16:
                usart3Printf("{3}");
                break;
            case 224:
                usart3Printf("{4}");
                break;


        }
        pre_key=key_value;
    }


}






















