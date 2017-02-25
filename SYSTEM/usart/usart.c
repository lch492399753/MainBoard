#include "sys.h"
#include "usart.h"
#include "PID.h"
#include <stdarg.h>


extern  u8 atk_8266_send_date(u8 *cmd,u8 *ack,u16 waittime);
extern u8 Table_Number;
extern u16 speed_l,speed_r;
u8   flag_RecFul=0,flag_alive=0,flag_ID;
u16 flag_ten=0;
u8 precard=255;
u8 ka=0,ka1=0,ka2=0;
//////////////////////////////////////////////////////////////////////////////////
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"                   //ucos 使用   
#endif
static u8 tou=0,wei=0;

unsigned char TxBuffer5[100] ;
unsigned int i;
extern unsigned int flag;
extern u8 Card,wifi_ok,moshi,turn_flag;
u16 Actual=800;
u16 Actual_Pwm=800;
u16 Actual_Pwm1=0;
u16 Actual_Pwm2=0;
u16 Preset_Pwm=800;
u8 biaozhi=0;

u8 ASC_To_Valu(u8 asc);
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;

};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
_sys_exit(int x)
{
    x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
    while((USART1->SR&0X40)==0);//循环发送,直到发送完毕
    USART1->DR = (u8) ch;
    return ch;
}
#endif

/*使用microLib的方法*/
/*
int fputc(int ch, FILE *f)
{
USART_SendData(USART1, (uint8_t) ch);

while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}

   return ch;
}
int GetKey (void)  {

   while (!(USART1->SR & USART_FLAG_RXNE));

   return ((int)(USART1->DR & 0x1FF));
}

*/

void usart1Printf(char *fmt, ...)
{
    char buffer[USART_REC_LEN - 1];
    u8 i = 0;
    u8 len;

    va_list arg_ptr; //Define convert parameters variable
    va_start(arg_ptr, fmt); //Init variable
    len = vsnprintf(buffer, USART_REC_LEN+1, fmt, arg_ptr); //parameters list format to buffer

    while ((i < USART_REC_LEN) && (i < len) && (len > 0))
    {
        USART_SendData(USART1, (u8) buffer[i++]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    }
    va_end(arg_ptr);
}

void usart2Printf(char *fmt, ...)
{
    char buffer[USART_REC_LEN - 1];
    u8 i = 0;
    u8 len;

    va_list arg_ptr; //Define convert parameters variable
    va_start(arg_ptr, fmt); //Init variable
    len = vsnprintf(buffer, USART_REC_LEN+1, fmt, arg_ptr); //parameters list format to buffer

    while ((i < USART_REC_LEN) && (i < len) && (len > 0))
    {
        USART_SendData(USART2, (u8) buffer[i++]);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    }
    va_end(arg_ptr);
}
void usart3Printf(char *fmt, ...)
{
    char buffer[USART_REC_LEN - 1];
    u8 i = 0;
    u8 len;

    va_list arg_ptr; //Define convert parameters variable
    va_start(arg_ptr, fmt); //Init variable
    len = vsnprintf(buffer, USART_REC_LEN+1, fmt, arg_ptr); //parameters list format to buffer

    while ((i < USART_REC_LEN) && (i < len) && (len > 0))
    {
        USART_SendData(USART3, (u8) buffer[i++]);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
    }
    va_end(arg_ptr);
}
void usart4Printf(char *fmt, ...)
{
    char buffer[USART_REC_LEN - 1];
    u8 i = 0;
    u8 len;

    va_list arg_ptr; //Define convert parameters variable
    va_start(arg_ptr, fmt); //Init variable

    len = vsnprintf(buffer, USART_REC_LEN+1, fmt, arg_ptr); //parameters list format to buffer

    while ((i < USART_REC_LEN) && (i < len) && (len > 0))
    {

        USART_SendData(UART4, (u8) buffer[i++]);
        while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
    }
    va_end(arg_ptr);
}



u8 USART_RX_BUF[USART_REC_LEN];
u16 USART_RX_STA=0;

#if EN_USART1_RX
u8 USART1_RX_BUF[USART1_REC_LEN];
u16 USART1_RX_STA=0;

void uart_init(u32 bound)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);

    //USART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //USART1_RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    NVIC_InitStructure.NVIC_IRQChannel =USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);

}
void USART1_IRQHandler(void)
{
    u8 Res;
#if SYSTEM_SUPPORT_OS
    OSIntEnter();
#endif
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        Res =USART_ReceiveData(USART1);
        if((USART1_RX_STA&0x8000)==0)
        {
            if(USART1_RX_STA&0x4000)
            {
                if(Res!=0x0a)USART1_RX_STA=0;
                else USART1_RX_STA|=0x8000;
            }
            else
            {
                if(Res==0x0d)USART1_RX_STA|=0x4000;
                else
                {
                    USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
                    USART1_RX_STA++;
                    if(USART1_RX_STA>(USART1_REC_LEN-1))
                        USART1_RX_STA=0;
                }
            }
        }
    }
#if SYSTEM_SUPPORT_OS
    OSIntExit();
#endif
}
#endif


#if EN_USART2_RX
u8 USART2_RX_BUF[USART2_BUFF_SIZE];
u16 USART2_RX_STA=0;
uint8_t g_u8Usart2RecLen = 0;
RINGBUFF_T g_tUSART2RxRingBuf;
void uart2_init(u32 bound)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    //  RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO |RCC_APB2Periph_GPIOA , ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    //USART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //USART1_RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    NVIC_InitStructure.NVIC_IRQChannel =USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);
	
	USART_GetFlagStatus(USART2, USART_FLAG_TC);
	
	//RingBuffer_Init ( &g_tUSART2RxRingBuf, g_cUSART2RxBuffer, 1, USART2_BUFF_SIZE );

}
uint8_t g_u8Usart2RecFlg = 0;
void USART2_IRQHandler(void)
{
    u8 Res;
#if SYSTEM_SUPPORT_OS
    OSIntEnter();
#endif
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        Res =USART_ReceiveData(USART2);
		//RingBuffer_Insert ( &g_tUSART2RxRingBuf, &Res );
        if((g_u8Usart2RecFlg&0x02)==0)
        {
            if(g_u8Usart2RecFlg&0x01)
            {
                if(Res == 0x0A)
				{
					g_u8Usart2RecFlg|=0x02;
					USART2_RX_BUF[g_u8Usart2RecLen++]=Res ;
				}
					
                else 
				{
					USART2_RX_BUF[g_u8Usart2RecLen]=Res ;
                    g_u8Usart2RecLen++;
                    if(g_u8Usart2RecLen>(USART_REC_LEN-1))
                        g_u8Usart2RecLen=0;
				}
            }
            else
            {
                if(Res =='W')
				{
					g_u8Usart2RecFlg |= 0x01;
					g_u8Usart2RecLen = 0;
					USART2_RX_BUF[g_u8Usart2RecLen++]= Res ;
				}	
                else
                {
                    g_u8Usart2RecFlg = 0;
					//g_u8Usart2RecLen = 0;
                }
            }
        }
    }
#if SYSTEM_SUPPORT_OS
    OSIntExit();
#endif
}
#endif


#if EN_USART3_RX
u8 USART3_RX_BUF[USART_REC_LEN];
u16 USART3_RX_STA=0;

char g_cUSART3RxBuffer[USART3_BUFF_SIZE];
char g_cUSART3TxBuffer[USART3_BUFF_SIZE];
RINGBUFF_T g_tUSART3RxRingBuf;
RINGBUFF_T g_tUSART3TxRingBuf;

void uart3_init(u32 bound)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    // RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART3|RCC_APB2Periph_GPIOB, ENABLE);

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO |RCC_APB2Periph_GPIOB , ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    //USART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //USART1_RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    NVIC_InitStructure.NVIC_IRQChannel =USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART3, ENABLE);
	
	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	//USART_ClearFlag(USART1, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
	USART_GetFlagStatus(USART3, USART_FLAG_TC);
	
	RingBuffer_Init ( &g_tUSART3RxRingBuf, g_cUSART3RxBuffer, 1, USART3_BUFF_SIZE );
	RingBuffer_Init ( &g_tUSART3TxRingBuf, g_cUSART3TxBuffer, 1, USART3_BUFF_SIZE );
}
void USART3_IRQHandler(void)
{
    u8 Res;
#if SYSTEM_SUPPORT_OS
    OSIntEnter();
#endif
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        Res = USART_ReceiveData(USART3);
		RingBuffer_Insert ( &g_tUSART3RxRingBuf, &Res );	
        if((USART3_RX_STA&0x8000)==0)
        {
            if(USART3_RX_STA&0x4000)
            {
                if(Res!=0x0a)
					USART3_RX_STA=0;
                else 
					USART3_RX_STA|=0x8000;
            }
            else
            {
                if(Res==0x0d)USART3_RX_STA|=0x4000;
                else
                {
                    USART3_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
                    USART3_RX_STA++;
                    if(USART3_RX_STA>(USART_REC_LEN-1))
                        USART3_RX_STA=0;
                }
            }
        }
    }
#if SYSTEM_SUPPORT_OS
    OSIntExit();
#endif
}

void USART3_SendStrings(unsigned char* str,uint8_t len)
{
	uint8_t  i = 0;
    while( i < len )
    {
		//while( USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
		USART_SendData(USART3, str[i++]);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)  ;
    }
}

#endif


#if EN_USART4_RX


u8 USART4_RX_BUF[USART_REC_LEN];
u16 USART4_RX_STA=0;


uint8_t g_u8USART1RxCnt ;


void uart4_init(u32 bound)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
//   RCC_APB2PeriphClockCmd(RCC_APB1Periph_UART4|RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO |RCC_APB2Periph_GPIOC , ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);

    //USART1_TX   GPIOC.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //USART1_RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);


    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(UART4, &USART_InitStructure);
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
    USART_Cmd(UART4, ENABLE);
	
	
}

void UART4_IRQHandler(void)
{
    static u16  n=0;

    u8 Res;
    //  NVIC_InitTypeDef NVIC_InitStructure;
#if SYSTEM_SUPPORT_OS
    OSIntEnter();
#endif
    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {
		 
        if(USART_ReceiveData(UART4)=='{')
        {
            tou=1;
            n=0;
        }
        if(tou==1)
        {
            redata[n] =USART_ReceiveData(UART4);
            n++;
        }
        if(redata[n-1]=='}'&&tou==1)
        {
            //  redata[n]='}';
            flag_RecFul=1;
            flag_ten=n;
            tou=0;
            wei=1;

        }

    }
#if SYSTEM_SUPPORT_OS
    OSIntExit();
#endif
}
#endif






#if EN_USART5_RX
u8 USART5_RX_BUF[USART_REC_LEN];
u16 USART5_RX_STA=0;

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
void UART5_IRQHandler(void)
{
	u8 Res,Ka,ka_biaozhi;

#if SYSTEM_SUPPORT_OS
    OSIntEnter();
#endif
    //printf("a\r\n");
    //LED0=1;
    if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
    {
        Res =USART_ReceiveData(UART5);  //读取接收到的数据
        if((USART5_RX_STA&0x8000)==0)//接收未完成
        {
            if(USART5_RX_STA&0x4000)//接收到了0x0d
            {
                if(Res!=0x0a)USART5_RX_STA=0;//接收错误,重新开始
                else USART5_RX_STA|=0x8000; //接收完成了
            }
            else //还没收到0X0D
            {
                if(Res==0x0d)
					USART5_RX_STA|=0x4000;
                else
                {
                    //  USART5_RX_BUF[USART5_RX_STA&0X3FFF]=Res ;
                    if(Res==0x02)
                    {
                        biaozhi=0;
                    }
                    USART5_RX_BUF[biaozhi]=Res ;
                    biaozhi++;
                    USART5_RX_STA++;
                    if(biaozhi==11)
                    {
                        USART_SendData(USART1, USART5_RX_BUF[9]);
                        while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
                        USART_SendData(USART1, USART5_RX_BUF[10]);//向串口1发送数据
                        while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
                    }
//                      if( USART5_RX_STA )
                    if(USART5_RX_STA>(USART_REC_LEN-1))USART5_RX_STA=0;//接收数据错误,重新开始接收
                }
            }
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
    ka=(ASC_To_Valu(USART5_RX_BUF[9])*16+ASC_To_Valu(USART5_RX_BUF[10]));
    if( ka1==ka)
    {
        Card=ka;
    }
    ka1=ka;
    //ka2=ka1;
    //if()
    if(precard!=Card)
    {
        precard=Card;
        flag_ID=1;
    }

    //  usart4Printf("Card : %d \r\n",Card);
//          switch(Ka)
//  {
//
//      case 1: Card=1;flag_ID=1;break;
//      case 2:Card=2;flag_ID=1;break;
//      case 3:Card=3;flag_ID=1;break;
//      case 4:Card=4;flag_ID=1;break;
//      case 5:Card=5;flag_ID=1;break;
//      case 6:Card=6;flag_ID=1;break;
//      case 7:Card=7;flag_ID=1;break;
//      case 8:Card=8;flag_ID=1;break;
//      case 9:Card=9;flag_ID=1;break;
//      case 10:Card=10;flag_ID=1;break;
//      case 11:Card=11;flag_ID=1;break;
//      case 12:Card=12;flag_ID=1;break;
//      case 13:Card=13;flag_ID=1;break;
//      case 14:Card=14;flag_ID=1;break;
//      case 15:Card=15;flag_ID=1;break;
//      case 16:Card=16;flag_ID=1;break;
//      case 17:Card=17;flag_ID=1;break;
//      case 18:Card=18;flag_ID=1;break;
//      case 19:Card=19;flag_ID=1;break;
//      case 20:Card=20;flag_ID=1;break;
//  }
    // usart4Printf("Card%d\r\n",ASC_To_Valu(USART5_RX_BUF[9])*10);
    //usart4Printf("Card%d\r\n",Ka);
//   if(USART5_RX_BUF[9]==0x30&&USART5_RX_BUF[10]==0x31) //1
//      {
//      Card=1;
//
//          flag_ID=1;
//   //     printf("b\r\n");
//      }
//           if(USART5_RX_BUF[9]==0x30&&USART5_RX_BUF[10]==0x32) //0
//      {
//          Card=2;
//
//          flag_ID=1;
//      //  printf("a\r\n");
//      }
//   if(USART5_RX_BUF[9]==0x30&&USART5_RX_BUF[10]==0x33) //1
//      {
//     Card=3;
//
//     flag_ID=1;
//      }

    // usart4Printf("%i%i\r\n",USART5_RX_BUF[9],USART5_RX_BUF[10]);
#if SYSTEM_SUPPORT_OS
    OSIntExit();
#endif

}


#endif

