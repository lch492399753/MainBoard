#include "common.h"

/*
*�ж�a�Ƿ���(b-delta,b+delta)֮�ڣ��ڷ�Χ֮�ڷ���1�����򷵻�0.
*/
uint8_t IsInRange(uint16_t a , uint16_t b, uint16_t delta)
{
	if( (a>(b-delta)) && (a<(b+delta)) )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void pinMode(GPIO_TypeDef* GPIOx, uint16_t pin, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef GPIO_Speed)
{
  GPIO_InitTypeDef init ;
  init.GPIO_Mode = mode;
  init.GPIO_Pin = pin;
  init.GPIO_Speed = GPIO_Speed;
  
  GPIO_Init(GPIOx, &init);
}

#define cli __disable_irq
#define sei __enable_irq

_u32 enter_critical_section(void)
{
    _u32 context=__get_PRIMASK();
    cli();
    return context;
}

void leave_critical_section(_u32 context)
{
    __set_PRIMASK(context);
}

int abs(int i)
{      /* compute absolute value of int argument */
	return (i < 0 ? -i : i);
}

