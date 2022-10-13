#ifndef __MYTIMER_H__
#define __MYTIMER_H__

#include "stm32f10x.h"

typedef struct
{
	TIM_TypeDef* Timer ; // TIM1 à TIM4
	unsigned short ARR;
	unsigned short PSC;
} MyTimer_Struct_TypeDef ;

/*
*****************************************************************************************
* @brief
* @param -> Paramètre sous forme d’une structure (son adresse) contenant les
	informations de base
* @Note -> Fonction à lancer systématiquement avant d’aller plus en détail dans les
	conf plus fines (PWM, codeur inc...)
*************************************************************************************************
*/
void timer_base_init(MyTimer_Struct_TypeDef* Timer);
/*
**************************************************************************************************
* @brief
* @param : - TIM_TypeDef * Timer : Timer concerne
*					 - char Prio : de 0 a 15
* @Note : La fonction MyTimer_Base_Init doit avoir ete lancee au prealable
**************************************************************************************************
*/
void timer_activeIT(TIM_TypeDef* Timer, char prio, void (*IT_funct) (void));
//void TIM1_UP_IRQHandler(void);
void TIM2_IRQHandler(void);
//void TIM3_IRQHandler(void);
//void TIM4_IRQHandler(void);

#define TIMER_BASE_START(Timer) ((Timer)->CR1|=TIM_CR1_CEN)
#define TIMER_BASE_STOP(Timer) ((Timer)->CR1&=~TIM_CR1_CEN)

void timer_pwm( TIM_TypeDef * Timer , char channel ) ;
void timer_pwm_changecycle( TIM_TypeDef * Timer , char ratio, char channel);

#endif
