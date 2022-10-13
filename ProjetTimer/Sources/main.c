/*
*	Title       : main.c
* Author      : Arthur Gautheron
* Date        : 28/09/2022
* Description : Le programme principal du TP Timers.
*/

#include "stm32f10x.h"
#include "driver_timer.h"
#include "driver_gpio.h"

void ma_fonction(void);

int main(void)
{
	// Structure configuration
	MyTimer_Struct_TypeDef timer;
	MyGPIO_Struct_TypeDef gpio;
	
	timer.Timer = TIM2;
	timer.ARR = 0x2D0;
	timer.PSC = 0x0;
	
	// Initialisation timer
	timer_base_init(&timer);
	timer_pwm(timer.Timer, 2);
	timer_pwm_changecycle(timer.Timer, 20, 2);
	
	gpio.GPIO = GPIOB;
	gpio.pin = 3;
	gpio.config = OUT_ALT_PUSHPULL;
	
	gpio_init(&gpio);
	

	
	// Départ du comptage
	TIMER_BASE_START(timer.Timer);
	
	// Boucle infinie
	do
	{
		
	}while(1);
}

void ma_fonction(void)
{
	gpio_toggle(GPIOC, 10);
}
