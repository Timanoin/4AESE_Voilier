#ifndef __MYGIROUETTE_H__
#define __MYGIROUETTE_H__

#include "stm32f10x.h"
#include "driver_gpio.h" 
#include "driver_timer.h"

void Gir_Config(TIM_TypeDef* Timer);
int Gir_Angle(void);
int Gir_Reset(void);


#endif 
