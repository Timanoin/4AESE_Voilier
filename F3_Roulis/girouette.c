#include "stm32f10x.h"
#include "girouette.h"
#include "driver_gpio.h" 
#include "driver_timer.h"



void Gir_Config(TIM_TypeDef* Timer) {

		Timer_Incremental(Timer);
		TIMER_BASE_START(Timer);
}

int Gir_Angle(void) {
	return TIM1->CNT / 4;
}
	
int Gir_Reset(void) {
	return TIM1->CNT =0;
}
