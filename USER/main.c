#include "delay.h"
#include "sys.h"
#include "core_cm3.h"
#include "usart.h"
#include "remote.h"
#include "PWM.h"
#include "gpio.h"
#include "exti.h"
#include "jiexi.h"
#include "string.h"
#include "stdio.h"
#include "stmflash.h"
#include "PID.h"
#include "stdlib.h"
#include "Tracking.h"
#include "wtc7514.h"
#include "user_config.h"
#include "PoseWithVoice_Fun.h"
#include "motor.h"
#include "inv_mpu.h"
#include "obst.h"
#include "rfid.h"

u16 ID0_start,ID0_length,IDN_length,
    ID1_start,ID1_length,ID2_start,ID2_length,ID3_start,ID3_length,ID4_start,ID4_length,ID5_start,ID5_length,
    ID6_start,ID6_length,ID7_start,ID7_length,ID8_start,ID8_length,ID9_start,ID9_length,ID10_start,ID10_length,
    ID11_start,ID11_length,ID12_start,ID12_length,ID13_start,ID13_length,ID14_start,ID14_length,ID15_start,ID15_length,
    ID16_start,ID16_length,ID17_start,ID17_length,ID18_start,ID18_length,ID19_start,ID19_length,ID20_start,ID20_length,
    IDY_length;
extern unsigned char TxBuffer5[100] ;
extern unsigned int i;
unsigned int flag=0;
extern u16 flag_ten;
extern u8 flag_RecFul,flag_alive,flag_ID;
u8 flag_new=0;
u8 Room=0,starting=0,waiting=0,Table_Number,Card,take_out=1,agv=0,Obstacte;
//u8  key0=0,key1=0,key2=0,key3=0,key4=0,key5=0,key6=0,key7=0,key8=0,key9=0,key10=0,key11=0,key12=0,key13=0,key14=0,key15=0;
__IO char key[16] = {0};
uint16_t g_u16RunSpeed = 0;
u16 t;
u16 len;
u16 times=0;
u8 left1,right1,flag_wifi=0,wifi=0;
volatile u32 time=0,time_alive=0;
u32 stop=0,stop_flag=0;

char pianyi=0;

//u8  data[100];
//u8 redata[]={"\{\"001\":\"0000\",\"002\":\"00161111160445503000\",\"003\":\"00161111160445502000\"\}"};
char  redata[20000];
char  IDNdata[10000];
char  IDYdata[10000];
u8 ID_finished=0;
u16 agv_time,wifi_time,zhanhui_time=0;
u8 id0_flag=0,agv_flag=1,wifi_ok=0,diaot_flag=0,diaotou_flag=0,wifi_flag=0,work_flag=0,IDfinish_flag=0,IDY_flag=0,mshi=0,moshi=1,turn_flag=0,zidao_flag=0,walking_flag=0,idka_falg=0,
   yihangmoshi,pre_walking_flag=0;
//char CMD_Buffer[20][40];
#define SIZE 1000       //数组长度
#define FLASH_SAVE_ADDR  0X08070000     //设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)
NVIC_InitTypeDef NVIC_InitStructure;
u8  agv_update,ID_update,obstacle_update;
u8  pre_card=255,pre_agv=2,pre_Obstacte=2;
u8 changjing=0;


u32 curr_speed_left = 0;
u32 curr_speed_right = 0;
short speed_l=600,speed_r=600;

EXTI_InitTypeDef EXTI_InitStructure;
u8 KA=250;
u16 GetVal=0,pre_GetVal=0,GetValhead=0,pre_GetValhead=0;
u8 touch1=0,pre_touch1=0;
void sensor_update(void);
void jiexi(u8 id);
void Wifi_init()
{
    while(USART_ReceiveData(UART4)!='P');
    atk_8266_send_cmd("AT+CIPMODE=1\r\n","OK",20);
    atk_8266_send_cmd("AT+CIPSTART=\"TCP\",\"192.168.5.107\",8015\r\n","OK",20);
//  usart4Printf("%s\r\n","AT+CIPSEND");
    //atk_8266_send_cmd("AT+CIPSEND","OK",10);
    //usart4Printf("ok");
}





void AGV_Sensor()
{
    key[15]=GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15);
    key[14]=GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_14);
    key[13]=GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13);
    key[12]=GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12);

    key[11]=GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11);
    key[10]=GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10);
    key[9]=GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9);
    key[8]=GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8);


    key[7]=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15);
    key[6]=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14);
    key[5]=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13);
    key[4]=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12);

    key[3]=GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_15);
    key[2]=GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14);
    key[1]=GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13);
    key[0]=GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12);

    left1=key[8]+key[9]+key[10]+key[11]+key[12]+key[13]+key[14];
    right1 =key[1]+key[2]+key[3]+key[4]+key[5]+key[6]+key[7];
    //usart2Printf("%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d\r\n",key0,key1,key2,key3,key4,key5,key6,key7,key8,key9,key10,key11,key12,key13,key14,key15);
}

void Keep_Walking (u16 speed)
{
    Forward();
    AGV_Sensor();
    // Speed(speed);
    Tracking(70);
}
u8 ASC_To_Valu(u8 asc)
{
    u8 valu;
    switch(asc)
    {
        case 0x30:
            valu=0;
            break;
        case 0x31:
            valu=1;
            break;
        case 0x32:
            valu=2;
            break;
        case 0x33:
            valu=3;
            break;
        case 0x34:
            valu=4;
            break;
        case 0x35:
            valu=5;
            break;
        case 0x36:
            valu=6;
            break;
        case 0x37:
            valu=7;
            break;
        case 0x38:
            valu=8;
            break;
        case 0x39:
            valu=9;
            break;
        //case 0x40:valu=10;break;
        case 0x41:
            valu=10;
            break;
        case 0x42:
            valu=11;
            break;
        case 0x43:
            valu=12;
            break;
        case 0x44:
            valu=13;
            break;
        case 0x45:
            valu=14;
            break;
        case 0x46:
            valu=15;
            break;
//      case 0x37:valu=7;break;
//      case 0x38:valu=8;break;
//      case 0x39:valu=9;break;
    }
    return valu;
}
void jixi_IDYdate(u16 IDY_leng)
{
    u8 tt=0,ID,m=13;
    vs16 length=0,l,ID_start,ID_length=0;

//while(l>12)
    for( l=IDY_leng; l>12;)
    {
//  usart4Printf("IDY_leng%d\r\n",IDY_leng);
        ID=ASC_To_Valu(IDYdata[1+length+tt*m])*100+ASC_To_Valu(IDYdata[2+length+tt*m])*10+ASC_To_Valu(IDYdata[3+length+tt*m]);
        ID_length=ASC_To_Valu(IDYdata[7+length+tt*m])*1000+ASC_To_Valu(IDYdata[8+length+tt*m])*100+ASC_To_Valu(IDYdata[9+length+tt*m])*10+ASC_To_Valu(IDYdata[10+length+tt*m]);
        ID_start=(11+length+tt*m);
        length=length+ID_length;
        tt++;
//  usart4Printf("L %d\r\n",l);
//  usart4Printf("ID%d\r\n",ID);
//  usart4Printf("ID_start%d\r\n",ID_start);
//  usart4Printf("ID_length%d\r\n",ID_length);
//  usart4Printf("tt%d\r\n",tt);
        switch(ID)
        {
            case 0:
            {
                ID0_start=ID_start;
                ID0_length=ID_length;
                l=l-(ID1_length+13);
            }
            break;
            case 1:
            {
                ID1_start=ID_start;
                ID1_length=ID_length;
                l=l-(ID1_length+13);
                KA=1;
            }
            break;
            case 2:
            {
                ID2_start=ID_start;
                ID2_length=ID_length;
                l=l-(ID2_length+13);
                KA=2;
            }
            break;
            case 3:
            {
                ID3_start=ID_start;
                ID3_length=ID_length;
                l=l-(ID3_length+13);
                KA=3;
            }
            break;
            case 4:
            {
                ID4_start=ID_start;
                ID4_length=ID_length;
                l=l-(ID4_length+13);
                KA=4;
            }
            break;
            case 5:
            {
                ID5_start=ID_start;
                ID5_length=ID_length;
                l=l-(ID5_length+13);
                KA=5;
            }
            break;
            case 6:
            {
                ID6_start=ID_start;
                ID6_length=ID_length;
                l=l-(ID6_length+13);
                KA=6;
            }
            break;
            case 7:
            {
                ID7_start=ID_start;
                ID7_length=ID_length;
                l=l-(ID7_length+13);
                KA=7;
            }
            break;
            case 8:
            {
                ID8_start=ID_start;
                ID8_length=ID_length;
                l=l-(ID8_length+13);
                KA=8;
            }
            break;
            case 9:
            {
                ID9_start=ID_start;
                ID9_length=ID_length;
                l=l-(ID9_length+13);
                KA=9;
            }
            break;
            case 10:
            {
                ID10_start=ID_start;
                ID10_length=ID_length;
                l=l-(ID10_length+10);
                KA=10;
            }
            break;
            case 11:
            {
                ID11_start=ID_start;
                ID11_length=ID_length;
                l=l-(ID11_length+10);
                KA=11;
            }
            break;
            case 12:
            {
                ID12_start=ID_start;
                ID12_length=ID_length;
                l=l-(ID12_length+10);
                KA=12;
            }
            break;
            case 13:
            {
                ID13_start=ID_start;
                ID13_length=ID_length;
                l=l-(ID13_length+10);
                KA=13;
            }
            break;
            case 14:
            {
                ID14_start=ID_start;
                ID14_length=ID_length;
                l=l-(ID14_length+10);
                KA=14;
            }
            break;
            case 15:
            {
                ID15_start=ID_start;
                ID15_length=ID_length;
                l=l-(ID15_length+10);
                KA=15;
            }
            break;
            case 16:
            {
                ID16_start=ID_start;
                ID16_length=ID_length;
                l=l-(ID16_length+10);
                KA=16;
            }
            break;
            case 17:
            {
                ID17_start=ID_start;
                ID17_length=ID_length;
                l=l-(ID17_length+10);
                KA=17;
            }
            break;
            case 18:
            {
                ID18_start=ID_start;
                ID18_length=ID_length;
                l=l-(ID18_length+10);
                KA=18;
            }
            break;
            case 19:
            {
                ID19_start=ID_start;
                ID19_length=ID_length;
                l=l-(ID19_length+10);
                KA=19;
            }
            break;
            case 20:
            {
                ID20_start=ID_start;
                ID20_length=ID_length;
                l=l-(ID20_length+10);
                KA=20;
            }
            break;
        }
    }



}
void OneLine(char *str)
{
    u16  ii,n=0,IDY_length,nn=0;
    u8 ID=250,IID;

    ID=ASC_To_Valu(str[2])*100+ASC_To_Valu(str[3])*10+ASC_To_Valu(str[4]);
//usart4Printf("%d",*p);
    if(ID==0)
    {
        id0_flag=1;
        IDN_length=ASC_To_Valu(str[8])*1000+ASC_To_Valu(str[9])*100+ASC_To_Valu(str[10])*10+ASC_To_Valu(str[11]);
        for(ii=IDN_length+1; ii>0; ii--)
        {
            IDNdata[n]=redata[n+12];
            n++;
        }
        n=0;
//                   for(t=0;t<(n-1);t++)
//          {
//              USART_SendData(UART4, IDNdata[t]);//向串口1发送数据
//              while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//          }
        n=0;
        if(str[13+IDN_length]==',')
        {
            while(str[IDN_length+14+n]!='}')
            {
                IDYdata[n]=redata[n+14+IDN_length];
                n++;
                IDY_length=n;
            }
            n=0;
            //usart4Printf("\r\n%d\r\n",IDN_length);
//       for(t=0;t<(n);t++)
//          {
//              USART_SendData(UART4, IDYdata[t]);//向串口1发送数据
//              while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//          }
            jixi_IDYdate(IDY_length);
            work_flag=1;
            //  usart4Printf("\r\nOK\r\n");
        }
    }
    else
    {
        IDY_length=ASC_To_Valu(str[8])*1000+ASC_To_Valu(str[9])*100+ASC_To_Valu(str[10])*10+ASC_To_Valu(str[11]);
        n=0;

        while(str[1+n]!='}')
        {
            if(str[1+n]==',')
            {
                IID=ASC_To_Valu(str[3+n])*100+ASC_To_Valu(str[4+n])*10+ASC_To_Valu(str[5+n]);
                if(IID==0)
                {
                    id0_flag=1;
                    IDN_length=ASC_To_Valu(str[9])*1000+ASC_To_Valu(str[10])*100+ASC_To_Valu(str[11])*10+ASC_To_Valu(str[12]);
                    for(ii=IDN_length+1; ii>0; ii--)
                    {
                        IDNdata[nn]=redata[n+13];
                        n++;
                        nn++;
                    }
                    nn=0;
                }
                break;
            }
            else
            {
                IDYdata[n]=redata[n+1];
                n++;
                IDY_length=n;
            }
        }
        n=0;
//               for(t=0;t<(n);t++)
//          {
//              USART_SendData(UART4, IDYdata[t]);//向串口1发送数据
//              while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
//          }
        jixi_IDYdate(IDY_length);
        work_flag=1;
        //  usart4Printf("\r\nKO\r\n");
    }
    // l=IDY_length;
}

void keep_alive()
{
//usart4Printf("{content:%sABCDEFGHIJKLMNO}","fanweijin");
}
void DealRec()
{
    //STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)redata,flag_ten);
}

void Wifi_Ok()
{
    static  u16 a=0;
    while(!wifi_ok)
    {
        //usart3Printf("{2}\r\n");
        if(wifi_flag==1)
        {
            a++;
            //usart4Printf("{replay:ok?}");
            wifi_flag=0;
        }
//  if(a==1500)
//  {usart4Printf("{\"request\":\"keepalive\"}");a=0;}
    }
}




void Diao_tou()
{
    AGV_Sensor();
    {
        Walk_Right_Zhuan;
        SPEEDR=20;
        SPEEDL=20;
        //if((key0+left1+right1+key15==16))walking_flag=6;
        if((key[0]+left1+right1+key[15]==16))           diaotou_flag=1;
        if(((key[6]+key[7]+key[8])<=1)&&(diaotou_flag==1))
        {
            diaotou_flag=0;
            if(diaot_flag==1)
            {
                walking_flag=0;
            }
            else if  (diaot_flag==0)
            {
                walking_flag=6;
            }
        }
    }

}
void AGV_Sensor(void);

__IO uint8_t g_u8AutoRunFlg =0;
uint8_t g_u8StopFlg = 0;
//void zidao(short Speed)
//{
//	uint16_t u16Speed=0;
//    turn_flag=2;
//    
//	
//	//AGV_Sensor();
//    agv_cnt = key[0]+left1+right1+key[15];
//    if( key[0]+left1+right1+key[15]==16 )           //出轨
//    {
//        if( (zuo_chu==0)&&(you_chu==1) )
//        {
//            //Walk_Stop
////            Walk_Forward
////            //Walk_Left_Zhuan;
////            SPEEDL=0;
////            SPEEDR=Speed;
//			g_S8SpeedRTarget=30;
//			g_S8SpeedLTarget=0;
//        }
//        if( (zuo_chu==1)&&(you_chu==0) )
//        {

//			g_S8SpeedRTarget=0;
//			g_S8SpeedLTarget=30;
//        }
//        if( (zuo_chu==0)&(you_chu==0) )
//        {
//            //Walk_Left_Zhuan;
////            Walk_Forward;
////            SPEEDL=Speed;
////            SPEEDR=0;
//			g_S8SpeedRTarget=0;
//			g_S8SpeedLTarget=30;
//        }
//		SPEEDR=g_S8SpeedRCurr;
//		SPEEDL=g_S8SpeedLCurr;
//		g_S8SpeedAutoCurr = 0;
//    }
//    else
//    {
////		if(g_u8AutoRunFlg == 0)
////		{
////			//g_u8AutoRunFlg =1;
////		}
////        else
//		{
//			//Walk_Forward;
//			WALK_LEFT_FRONT();
//			WALK_RIGHT_FRONT();
//		}
//		g_S8SpeedAutoTarget=60;
//		//SpeedCover(AutoMotorID, g_S8SpeedAutoCurr,&u16Speed );
//        Tracking(u16Speed);
//    }
//}


void jiexi_data()
{
    if(id0_flag==1)
    {
        ID_0(IDNdata,0,IDN_length);
    }

}

void GuideWalking(uint8_t Speed);
uint32_t g_u16ObstCnt = 0;
uint8_t g_u8speed = 60;


uint16_t Angel = 0;
uint16_t AngelDir = 0;
int main(void)
{
	
	uint8_t cmd[5] = {0};
	
    delay_init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    uart_init(115200);
    uart2_init(115200);
    uart3_init(115200);
    uart4_init(115200 );
    //UART5_Configuration(9600);
	RfidInit();
	RobotGpioInit();
	IIC_Init();
//	while(mpu_dmp_init())
// 	{
//		delay_ms(200);
//	}  
	TIM3_PWM_Init(719,19);
    TIM2_Init(99,719);			//定时器周期为1ms

	//init_walkingmotor();                                                //?????????,?????????-1000 ~ 1000
    init_walkingmotor_odometer();                                       //??????????????,????
    //set_walkingmotor_speed(0, 0);                                       //???? ?:0mm/s ?:0mm/s
    START_TIME;

    Deng=1;

	Beep = 1;
    WTC7514_SetLevel(WTC7514_ADDR,29);
    delay_ms(100);
  
    delay_ms(100);
	g_u16RunSpeed = LOW_SPEED;
	memset(&g_tVoiceCtl,0,sizeof(g_tVoiceCtl));
	Beep = 0;

    while(1)
    {

		SendMsg2PAD();
		Rfid_heartbeat();
		Uart3_heartbeat();
		
        if( flag_RecFul==1)
        {
           OneLine(redata);
            // work_flag=1;
            flag_RecFul=0;
        }
        if(id0_flag==1)
        {
            jiexi_data();
        }
 
        if(flag_ID==1)
        {
            // if(work_flag==1)
            {
                flag_ID=0;
                //sensor_update();
                {
                    //while(flag_ID){
                    if(Card==KA)
                        jiexi_ID(Card);
                    //  work_flag=0;
                    //usart4Printf("{card%d}\r\n",Card);
                    //if(ID_finished) {ID_finished=0;flag_ID=0;}
                    //if(Card==3)usart3Printf("00160000022105500000\r\n");
                    //if(Card==2)usart3Printf("00160000021105500000\r\n");
                }
            }
        }

        switch(walking_flag)
        {
            case 0:
            {
				g_u8AutoRunFlg = 0;
            }
            break;
            case 1:
            {
				g_u8AutoRunFlg = 0;
            }
            break;
            case 2:
            {
				g_u8AutoRunFlg = 0;
            }
            break;
            case 3:
            {
				g_u8AutoRunFlg = 0;
            }
            break;
            case 4:
            {
				g_u8AutoRunFlg = 0; 
            }
            break;
            case 5:
            {
                Diao_tou();
            }
            break;
            case 6:
            {
                if((g_u8ObstCount>0) && (IsObstOpen()))
                {
					g_S8SpeedLTarget = 0;
					g_S8SpeedRTarget = 0;
					g_S8SpeedAutoTarget=0;
					g_u8SpeedDelta = 10;
////					SPEEDR= g_S8SpeedRCurr;
////					SPEEDL= g_S8SpeedRCurr;
//					if(SPEEDR > 0)
//						SPEEDR -= 50;
//					else
//						SPEEDR=0;
//					if(SPEEDL > 0)
//						SPEEDL-= 50;
//					else
//						SPEEDL = 0;
//					
//					SPEEDL =0;
//					SPEEDR = 0; 
					set_walkingmotor_speed(0, 0);                                       //???? ?:0mm/s ?:0mm/s
                    if( (g_u8ObstCount%10) == 0)
                    {
						
                    }
					g_u8ObstFlg = 1;
					if((g_u16ObstVoice == 5) || ((g_u16ObstVoice % 300) == 0))
					{
						//usart4Printf("{\"content\":\"00%d%d%d%d00000000\"}",g_u8CardH,g_u8CardL,g_u8ObstFlg,0);
					}
					g_u8AutoRunFlg = 0;
                }
                else
                {
					g_u8AutoRunFlg = 1;
					
                }

            }
            break;
			
			default:
				break;

        }

    }
}

