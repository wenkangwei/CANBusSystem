#ifndef SYSTICK_H
#define SYSTICK_H
#include "stm32f10x_conf.h"

__IO uint32_t TimingDelay;

void SysTick_Init(void);
void Delay_us(__IO u32 nTime);
void Delay_ms(uint32_t nTime);
void TimingDelay_Decrement(void);
#endif // SYSTICK_H

