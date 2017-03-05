#include "rfid.h"


uint8_t g_u8Usart5RecFlg = 0;
uint8_t g_u8Usart5RecLen = 0;
uint8_t UART5_RX_BUF[USART5_BUFF_SIZE];

void UART5_Configuration(u32 bound)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO |RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);

    //USART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //USART1_RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate =  bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(UART5, &USART_InitStructure);
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
    USART_Cmd(UART5, ENABLE);
    //USART_ClearFlag(UART5, USART_FLAG_TC);
}

void RfidInit()
{
	UART5_Configuration(9600);
}

void UART5_IRQHandler(void)
{
//	u8 Res,Ka,ka_biaozhi;
	uint8_t Res =0;
	
    if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
    {
        Res =USART_ReceiveData(UART5);  //读取接收到的数据
        if((g_u8Usart5RecFlg&0x02)==0)//接收未完成
        {
			if(g_u8Usart5RecFlg&0x01)
            {
                if(Res == 0x03)
				{
					g_u8Usart5RecFlg|=0x02;
					UART5_RX_BUF[g_u8Usart5RecLen++]=Res ;
				}
					
                else 
				{
					UART5_RX_BUF[g_u8Usart5RecLen]=Res ;
                    g_u8Usart5RecLen++;
                    if(g_u8Usart5RecLen>(USART5_BUFF_SIZE - 1))
                        g_u8Usart5RecLen=0;
				}
            }
            else
            {
                if(Res == 0x02)
				{
					g_u8Usart5RecFlg |= 0x01;
					g_u8Usart5RecLen = 0;
					UART5_RX_BUF[g_u8Usart5RecLen++]= Res ;
				}	
                else
                {
                    g_u8Usart5RecFlg = 0;
                }
            }
//            if(g_u8Usart5RecFlg&0x4000)//接收到了0x0d
//            {
//                if(Res!=0x0a)USART5_RX_STA=0;//接收错误,重新开始
//                else USART5_RX_STA|=0x8000; //接收完成了
//            }
//            else //还没收到0X0D
//            {
//                if(Res==0x0d)
//					g_u8Usart5RecFlg|=0x4000;
//                else
//                {
//                    //  USART5_RX_BUF[USART5_RX_STA&0X3FFF]=Res ;
//                    if(Res==0x02)
//                    {
//                        biaozhi=0;
//                    }
//                    USART5_RX_BUF[biaozhi]=Res ;
//                    biaozhi++;
//                    USART5_RX_STA++;
//                    if(biaozhi==11)
//                    {
//                        USART_SendData(USART1, USART5_RX_BUF[9]);
//                        while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//                        USART_SendData(USART1, USART5_RX_BUF[10]);//向串口1发送数据
//                        while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//                    }
////                      if( USART5_RX_STA )
//                    if(USART5_RX_STA>(USART_REC_LEN-1))USART5_RX_STA=0;//接收数据错误,重新开始接收
//                }
//            }
        }
    }
//   if(USART5_RX_BUF[9]==0x30&&USART5_RX_BUF[10]==0x30) //0
//      {
//          //Card=0;
//
//      //  flag_ID=1;
//   // printf("a\r\n");
//      }
    // Card=(ASC_To_Valu(USART5_RX_BUF[9])*16+ASC_To_Valu(USART5_RX_BUF[10]));
    // ifka_biaozhi
//    ka=(ASC_To_Valu(USART5_RX_BUF[9])*16+ASC_To_Valu(USART5_RX_BUF[10]));
//    if( ka1==ka)
//    {
//        Card=ka;
//    }
//    ka1=ka;
//    //ka2=ka1;
//    //if()
//    if(precard!=Card)
//    {
//        precard=Card;
//        flag_ID=1;
//    }


}

void Rfid_heartbeat()
{
	uint8_t card = 0;
	if( g_u8Usart5RecFlg&0x02 )
	{
		card = ( (UART5_RX_BUF[9] - 0x30)*16 + (UART5_RX_BUF[10] - 0x30) );
		
		SetCardVal(card);
		SetMsgUpdateFlg();
		g_u8Usart5RecFlg = 0;
		g_u8Usart5RecLen = 0;
		
	}
	
}
