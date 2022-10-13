#include "stm32f10x.h"
#include "driver_timer.h"

void (* TIM2_IRQHandler_funct_ptr) (void) ; /* déclaration d’un pointeur de fonction */

/*
*****************************************************************************************
* @brief
* @param -> Paramètre sous forme d’une structure (son adresse) contenant les
	informations de base
* @Note -> Fonction à lancer systématiquement avant d’aller plus en détail dans les
	conf plus fines (PWM, codeur inc...)
*************************************************************************************************
*/
void timer_base_init ( MyTimer_Struct_TypeDef * Timer_ptr )
{
	// Alimentation en clock
	if (Timer_ptr->Timer == TIM1)
	{
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	} 	
	else if (Timer_ptr->Timer == TIM2)
	{
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	}
	else if (Timer_ptr->Timer == TIM3)
	{
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	}
	else
	{
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	}
	
	// Configuration autoreload
	Timer_ptr->Timer->ARR |= Timer_ptr->ARR;
	
	// Configuration prescaler
	Timer_ptr->Timer->PSC |= Timer_ptr->PSC;
}


/*
**************************************************************************************************
* @brief
* @param : - TIM_TypeDef * Timer : Timer concerne
*					 - char Prio : de 0 a 15
* @Note : La fonction MyTimer_Base_Init doit avoir ete lancee au prealable
**************************************************************************************************
*/
void timer_activeIT(TIM_TypeDef* Timer, char prio, void (*IT_funct) (void))
{
	// Enables the interruption on the NVIC's side
	if (Timer == TIM1)
	{
		NVIC_EnableIRQ(TIM1_UP_IRQn);
		NVIC_SetPriority(TIM1_UP_IRQn, prio);
	}
	else if (Timer == TIM2)
	{
		NVIC_EnableIRQ(TIM2_IRQn);
		NVIC_SetPriority(TIM2_IRQn, prio);
	  // Assigns a function to the corresponding timer
	  TIM2_IRQHandler_funct_ptr = IT_funct;
	}
	else if (Timer == TIM3)
	{
		NVIC_EnableIRQ(TIM3_IRQn);
		NVIC_SetPriority(TIM3_IRQn, prio);
	}
	else
	{
		NVIC_EnableIRQ(TIM4_IRQn);
		NVIC_SetPriority(TIM4_IRQn, prio);
	}
	
	// Enables the update interruptions on the timer's side
	Timer->DIER |= TIM_DIER_UIE;

}

void TIM2_IRQHandler(void)
{
	// Clear interruption flag
	TIM2->SR &= ~TIM_SR_UIF;
	
	if (TIM2_IRQHandler_funct_ptr != 0) 
	{
		(*TIM2_IRQHandler_funct_ptr) (); /* appel indirect de la fonction */	
	}
}

void timer_pwm( TIM_TypeDef * Timer , char Channel )
{
	switch (Channel)
	{
		case 1:
			Timer->CCMR1 &= ~TIM_CCMR1_OC1M;
			Timer->CCMR1 |= (0x6 << 4);		
			Timer->CCMR1 |= TIM_CCMR1_OC1PE;
			break;
		case 2:
			Timer->CCMR1 &= ~TIM_CCMR1_OC2M;
			Timer->CCMR1 |= (0x6 << 12);		
			Timer->CCMR1 |= TIM_CCMR1_OC2PE;
			break;
		
		case 3:
			Timer->CCMR2 &= ~TIM_CCMR2_OC3M;
			Timer->CCMR2 |= (0x6 << 4);			
			Timer->CCMR2 |= TIM_CCMR2_OC3PE;
			break;
		
		case 4:
			Timer->CCMR2 &= ~TIM_CCMR2_OC4M;
			Timer->CCMR2 |= (0x6 << 12);		
			Timer->CCMR2 |= TIM_CCMR2_OC4PE;
			break;
		
		default:
			break;
		
	}
	
	Timer->CR1 |= TIM_CR1_ARPE;
	Timer->EGR |= TIM_EGR_UG;
}

void timer_pwm_changecycle( TIM_TypeDef * Timer , char ratio, char channel)
{
	switch (channel)
	{
		case 1:
			Timer->CCR1 &= ~(0xFFFF);
			Timer->CCR1 |= Timer->ARR * ratio / 100;
			break;
		
		case 2:
			Timer->CCR2 &= ~(0xFFFF);
			Timer->CCR2 |= Timer->ARR * ratio / 100;
			break;
		
		case 3:
			Timer->CCR3 &= ~(0xFFFF);
			Timer->CCR3 |= Timer->ARR * ratio / 100;
			break;
		
		case 4:
			Timer->CCR4 &= ~(0xFFFF);
			Timer->CCR4 |= Timer->ARR * ratio / 100;
			break;
		
		default:
			break;
		
	}
}

