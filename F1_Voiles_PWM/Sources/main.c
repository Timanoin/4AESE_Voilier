/*
*	Title       : main.c
* Author      : Arthur Gautheron, Olivier Lagrost, Kenza Bouzergan, Tran Kien Trunh
* Date        : 13/10/2022
* Description : Le programme principal du projet Voilier.
*/

#define DUTYCYCLE_MIN 5
#define DUTYCYCLE_MAX 10

#include "stm32f10x.h"
#include <assert.h>

#include "m_voiles.h"
#include "driver_timer.h"
#include "driver_gpio.h"

int main(void)
{
	// Déclaration des structures
	
	MyTimer_Struct_TypeDef timer;
	MyGPIO_Struct_TypeDef gpio;
	int cycle = DUTYCYCLE_MIN;
	
	// Initialisation des périphériques
	
	// Initialisation timer
	timer.Timer = TIM2;
	timer.ARR = 0xFFFF;
	timer.PSC = 22;	
	timer_base_init(&timer);
	
	//Initialisation PWM
	timer_pwm(timer.Timer, 2);

	// Configuration pin de sortie
	gpio.GPIO = GPIOA;
	gpio.pin = 1;
	gpio.config = OUT_ALT_PUSHPULL;	
	gpio_init(&gpio);
	
	// Départ du comptage
	TIMER_BASE_START(timer.Timer);
	
	// Boucle infinie
	do
	{
		if (cycle < DUTYCYCLE_MAX)
		{
			timer_pwm_changecycle(timer.Timer, cycle, 2);
			//Point d'arrêt
			cycle++;
		}
	}while(1);
}


