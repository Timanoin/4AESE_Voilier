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
	MyTimer_Struct_TypeDef timer_voiles;
	MyGPIO_Struct_TypeDef gpio_pwm_voiles;	
	
	// Initialisation des périphériques
	voiles_init_pwm(&timer_voiles, &gpio_pwm_voiles);
	timer_pwm_changecycle(timer_voiles.Timer, 5.0, 2);
	
	// Départ du comptage
	TIMER_BASE_START(timer_voiles.Timer);
	
	// Boucle infinie
	do
	{

	}while(1);
}


