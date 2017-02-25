#ifndef __COMMON_H
#define __COMMON_H

#include "sys.h"
#include "stdlib.h"
#include "string.h"

typedef int8_t         _s8;
typedef uint8_t        _u8;

typedef int16_t        _s16;
typedef uint16_t       _u16;

typedef int32_t        _s32;
typedef uint32_t       _u32;

typedef int64_t        _s64;
typedef uint64_t       _u64;


#define HIGH 1
#define LOW  0

#define _PIN_SET_1(port, pin) port->BSRR = pin
#define _PIN_SET_0(port, pin) port->BRR = pin
#define _PIN_SET(port, pin, val) _PIN_SET_##val(port, pin)

#define PIN_SET(port, pin, val) EXPAND_WRAPPER(_PIN_SET, port, pin, val)
#define PIN_READ(port, pin) ((port)->IDR & pin)

#define pinSet   GPIO_WriteBit
#define pinRead  GPIO_ReadInputDataBit


#define _INIT_OC_NOPRELOAD_FOR(channel, timer)  \
  TIM_OC##channel##Init(TIM##timer, &TIM_OCInitStructure); \
  TIM_OC##channel##PreloadConfig(TIM##timer, TIM_OCPreload_Disable)

#define INIT_OC_NOPRELOAD_FOR( channel, timer) \
  EXPAND_WRAPPER(_INIT_OC_NOPRELOAD_FOR, channel, timer)


#define _INIT_OC_FOR(channel, timer)  \
  TIM_OC##channel##Init(TIM##timer, &TIM_OCInitStructure); \
  TIM_OC##channel##PreloadConfig(TIM##timer, TIM_OCPreload_Enable)

#define INIT_OC_FOR( channel, timer) \
  EXPAND_WRAPPER(_INIT_OC_FOR, channel, timer)


#define _RAW_PWM_SET(channel, timer, val) \
  TIM##timer->CCR##channel = val

#define RAW_PWM_SET(channel, timer, val) \
  EXPAND_WRAPPER(_RAW_PWM_SET, channel, timer, val)


#define _RAW_PWM_GET(channel, timer) \
  TIM##timer->CCR##channel

#define RAW_PWM_GET(channel, timer) \
  EXPAND_WRAPPER(_RAW_PWM_GET, channel, timer)

uint8_t IsInRange(uint16_t a , uint16_t b, uint16_t delta);
void pinMode(GPIO_TypeDef* GPIOx, uint16_t pin,
                           GPIOMode_TypeDef mode, GPIOSpeed_TypeDef GPIO_Speed);
_u32 enter_critical_section(void);
void leave_critical_section(_u32 context);
int abs(int i);
#endif
