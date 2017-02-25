#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#include "ring_buffer.h"
#include "CmdPrase.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define USART1_REC_LEN  		64  	//�����������ֽ��� 200
//#define USART2_REC_LEN  			200  	//�����������ֽ��� 200
//#define USART4_REC_LEN         200
//#define USART5_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART2_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART3_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART4_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART5_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����	  	

#define USART2_BUFF_SIZE 32
#define USART3_BUFF_SIZE 256
extern uint8_t g_u8Usart2RecFlg ;
extern uint8_t g_u8Usart2RecLen ;
extern u8 g_cUSART2RxBuffer[USART2_BUFF_SIZE];
extern RINGBUFF_T g_tUSART2RxRingBuf;

extern char g_cUSART3RxBuffer[USART3_BUFF_SIZE];
extern char g_cUSART3TxBuffer[USART3_BUFF_SIZE];
extern RINGBUFF_T g_tUSART3RxRingBuf;
extern RINGBUFF_T g_tUSART3TxRingBuf;
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u8 USART2_RX_BUF[USART2_BUFF_SIZE];
extern u8  USART1_RX_BUF[USART1_REC_LEN];
extern u16 USART1_RX_STA;         		//����״̬���
extern u16 USART2_RX_STA;
extern u16 USART_RX_STA;         		//����״̬���	
extern char  redata[20000];
extern char  IDNdata[10000];
extern char  IDYdata[10000];
extern u16 USART4_RX_STA;
extern __align(8) u8 USART4_TX_BUF[1024];
extern u8  USART4_RX_BUF[USART_REC_LEN]; 
extern u8  USART5_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART5_RX_STA;         		//����״̬���	
extern  u16 Actual_Pwm;
extern  u16 Actual_Pwm1;
extern  u16 Actual_Pwm2;
extern  u16 Preset_Pwm;

#define UART5_REC_LEN 256
extern uint16_t UART5_RX_STA;
extern uint8_t UART5_RX_BUF[UART5_REC_LEN];

//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
void uart2_init(u32 bound);
 
void uart3_init(u32 bound);
void uart4_init(u32 bound);

void usart1Printf(char *fmt, ...);
void usart2Printf(char *fmt, ...);
void usart3Printf(char *fmt, ...);
void usart4Printf(char *fmt, ...);

void DealRec(void);
void UART5_Configuration(u32 bound);

u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8 atk_8266_send_date(u8 *cmd,u8 *ack,u16 waittime);
void USART3_SendStrings(unsigned char* str,uint8_t len);
extern const u8* portnum;
#endif


