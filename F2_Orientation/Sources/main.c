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

#include "m_orientation.h"

#include "driver_timer.h"
#include "driver_gpio.h"
#include "driver_usart.h"

int main(void)
{
	// Déclaration des structures
	
	// Rotation du plateau
	// Timer de fréquence 20kHz : ARR = 3600, PSC = 0
	MyTimer_Struct_TypeDef timer_pwm_plateau;
	MyGPIO_Struct_TypeDef  gpio_pwm_plateau;
	MyGPIO_Struct_TypeDef  gpio_pwm_sens;
	MyUSART_Struct_Typedef usart_telecommande;
	
	
	// Initialisation des périphériques
	orientation_init(&timer_pwm_plateau, &gpio_pwm_plateau, &gpio_pwm_sens, &usart_telecommande);

	
	// Départ du comptage
  TIMER_BASE_START(TIM3);
	
	// Boucle infinie
	do
	{
		orientation_gestion_plateau(&timer_pwm_plateau, &gpio_pwm_sens, &usart_telecommande);
	}while(1);
}


