#ifndef __JIEXI_H
#define __JIEXI_H	 
#include "sys.h"


void Motor_Head(char B2,u16 CC,u16 DD); 
void Forward (void);
void Backward (u16 speed);
void Turn_Left (u16 speed);
void Turn_Right (u16 speed);
void Stop (void);
void jiexi_ID (u8 id);

void ID_0(char *str,u16 start,u16 length);
void ID_1(char *str,u16 start,u16 length);
void ID_2(char *str,u16 start,u16 length);
void ID_3(char *str,u16 start,u16 length);
void ID_4(char *str,u16 start,u16 length);
void ID_5(char *str,u16 start,u16 length);
void ID_6(char *str,u16 start,u16 length);
void ID_7(char *str,u16 start,u16 length);

void Waist_Turn_Right(void);  // a=1 b=0 youzhuang 90
void Waist_Turn_Left(void);		 				    
#endif
