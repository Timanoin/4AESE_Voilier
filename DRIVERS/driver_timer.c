#include "stm32f10x.h"
#include "driver_timer.h"

void (* TIM2_IRQHandler_funct_ptr) (void) ; /* d�claration d�un pointeur de fonction */
void (* TIM1_IRQHandler_funct_ptr) (void) ;
/*
*****************************************************************************************
* @brief
* @param -> Param�tre sous forme d�une structure (son adresse) contenant les
	informations de base
* @Note -> Fonction � lancer syst�matiquement avant d�aller plus en d�tail dans les
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
	Timer_ptr->Timer->ARR = Timer_ptr->ARR;
	
	// Configuration prescaler
	Timer_ptr->Timer->PSC = Timer_ptr->PSC;
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
		TIM1_IRQHandler_funct_ptr = IT_funct;
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

// Active la PWM de Timer sur le Channel choisi
void timer_pwm( TIM_TypeDef * Timer , char Channel )
{
	switch (Channel)
	{
		case 1:
			// Configuration du mode de PWM
			Timer->CCMR1 &= ~TIM_CCMR1_OC1M;
			Timer->CCMR1 |= (0x6 << 4);		
			// Activation sortie OCx
			Timer->CCER |= TIM_CCER_CC1E;
			break;
		case 2:
			// Configuration du mode de PWM
			Timer->CCMR1 &= ~TIM_CCMR1_OC2M;
			Timer->CCMR1 |= (0x6 << 12);
			// Activation sortie OCx
			Timer->CCER |= TIM_CCER_CC2E;
			break;	
		case 3:
			// Configuration du mode de PWM
			Timer->CCMR2 &= ~TIM_CCMR2_OC3M;
			Timer->CCMR2 |= (0x6 << 4);	
			// Activation sortie OCx		
			Timer->CCER |= TIM_CCER_CC3E;
			break;	
		case 4:
			// Configuration du mode de PWM
			Timer->CCMR2 &= ~TIM_CCMR2_OC4M;
			Timer->CCMR2 |= (0x6 << 12);
			// Activation sortie OCx		
			Timer->CCER |= TIM_CCER_CC4E;
			break;	
		default:
			break;		
	}
	if (Timer == TIM1) Timer->BDTR |= TIM_BDTR_MOE;
	// Activation auto-reload preload register
	Timer->CR1 |= TIM_CR1_ARPE;
	// Mise � 0 du bit DIR
	Timer->CR1 &= ~TIM_CR1_DIR;
	// Activation Shadow registers
	Timer->EGR |= TIM_EGR_UG;
}

// Change le duty cycle de la PWM de Timer sur le channel
void timer_pwm_changecycle( TIM_TypeDef * Timer , char ratio, char channel)
{
	switch (channel)
	{
		case 1:
			Timer->CCR1 = Timer->ARR * ratio / 100;
			break;		
		case 2:
			Timer->CCR2 = Timer->ARR * ratio / 100;
			break;	
		case 3:
			Timer->CCR3 = Timer->ARR * ratio / 100;
			break;	
		case 4:
			Timer->CCR4 |= Timer->ARR * ratio / 100;
			break;		
		default:
			break;
	
	}
}

void Timer_Incremental(TIM_TypeDef* Timer)
{
	Timer->ARR = 1439;
	if (Timer == TIM1)
	{			
	
	TIM1->CCMR1 = (TIM1->CCMR1 & ~(0x3 << 0)) | (0x1 << 0);		
	TIM1->CCMR1 = (TIM1->CCMR1 & ~(0x3 << 8)) | (0x1 << 8);		
	
	TIM1->CCER &= ~(0x1 << 1);		
	TIM1->CCMR1 &= ~(0xF << 4);
	TIM1->CCER |= (0x1 << 0);
	
	TIM1->CCER &= ~(0x1 << 5);		
	TIM1->CCMR1 &= ~(0xF << 12);
	TIM1->CCER |= (0x1 << 4);
	
	TIM1->SMCR = (TIM1->SMCR & ~(0x7 << 0)) | (0x3 << 0);				
	
	TIM1->CR1 |= (0x1 << 0);
		
	
	}
	if (Timer == TIM2)
	{				
	
	TIM2->CCMR1 = (TIM2->CCMR1 & ~(0x3 << 0)) | (0x1 << 0);		
	TIM2->CCMR1 = (TIM2->CCMR1 & ~(0x3 << 8)) | (0x1 << 8);		
	
	TIM2->CCER &= ~(0x1 << 1);		
	TIM2->CCMR1 &= ~(0xF << 4);
	TIM2->CCER |= (0x1 << 0);
	
	TIM2->CCER &= ~(0x1 << 5);		
	TIM2->CCMR1 &= ~(0xF << 12);
	TIM2->CCER |= (0x1 << 4);
	
	TIM2->SMCR = (TIM2->SMCR & ~(0x7 << 0)) | (0x3 << 0);				
	
	TIM2->CR1 |= (0x1 << 0);
	}
	if (Timer == TIM3)
	{		
	
	TIM3->CCMR1 = (TIM3->CCMR1 & ~(0x3 << 0)) | (0x1 << 0);		
	TIM3->CCMR1 = (TIM3->CCMR1 & ~(0x3 << 8)) | (0x1 << 8);		
	
	TIM3->CCER &= ~(0x1 << 1);		
	TIM3->CCMR1 &= ~(0xF << 4);
	TIM3->CCER |= (0x1 << 0);
	
	TIM3->CCER &= ~(0x1 << 5);		
	TIM3->CCMR1 &= ~(0xF << 12);
	TIM3->CCER |= (0x1 << 4);
	
	TIM3->SMCR = (TIM3->SMCR & ~(0x7 << 0)) | (0x3 << 0);				
	
	TIM3->CR1 |= (0x1 << 0);
	}
	if (Timer == TIM4)
	{			
	
	TIM4->CCMR1 = (TIM4->CCMR1 & ~(0x3 << 0)) | (0x1 << 0);		
	TIM4->CCMR1 = (TIM4->CCMR1 & ~(0x3 << 8)) | (0x1 << 8);		
	
	TIM4->CCER &= ~(0x1 << 1);		
	TIM4->CCMR1 &= ~(0xF << 4);
	TIM4->CCER |= (0x1 << 0);
	
	TIM4->CCER &= ~(0x1 << 5);		
	TIM4->CCMR1 &= ~(0xF << 12);
	TIM4->CCER |= (0x1 << 4);
	
	TIM4->SMCR = (TIM4->SMCR & ~(0x7 << 0)) | (0x3 << 0);				
	
	TIM4->CR1 |= (0x1 << 0);
	}
}


