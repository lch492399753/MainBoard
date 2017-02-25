#ifndef WCT7514_H
#define WCT7514_H
#include "myiic.h"

#define WTC7514_ADDR 0x70
#define DEVICE_ADDR_HEAD 0x78

#define WTC7514_TOUCHED_NONE 0
#define WTC7514_TOUCHED_CH(n) 1<<(n)

#define TOUCH_NONE  0
#define TOUCH_HEAD  2
#define TOUCH_MOUTH 1

u8 WTC7514_ReadOneByte(u16 ReadAddr);
void WTC7514_SetLevel(u8 _addr, u8 level);
void WTC7514_Init(void); //³õÊ¼»¯IIC

void WTC7514_SetLevel_head(u8 level);
u16 WTC7514_ReadTwoByte(u8 _addr);
u16 WTC7514_ReadTwoByte_head(void);


#endif
