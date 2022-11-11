/*
*	Title       : main.c
* Author      : Arthur Gautheron, Olivier Lagrost, Kenza Bouzergan, Tran Kien Trunh
* Date        : 13/10/2022
* Description : Le programme principal du projet Voilier.
*/

#include "stm32f10x.h"

#include "girouette.h"
#include "m_voiles.h"
#include "m_orientation.h"

#include "driver_gpio.h"
#include "driver_timer.h"
#include "driver_usart.h"

int main(void)
{
	// Déclaration des variables et structures
	// Girouette
	MyGPIO_Struct_TypeDef gir_GPIO_A;
	MyGPIO_Struct_TypeDef gir_GPIO_B;
	MyGPIO_Struct_TypeDef gir_GPIO_I;
	MyTimer_Struct_TypeDef gir_compt_inc;
	int gir_angle;
	// Voiles
	MyTimer_Struct_TypeDef timer_voiles;
	MyGPIO_Struct_TypeDef gpio_pwm_voiles;
	
  // Rotation du plateau
	MyTimer_Struct_TypeDef timer_pwm_plateau;	// Timer de fréquence 20kHz : ARR = 3600, PSC = 0
	MyGPIO_Struct_TypeDef  gpio_pwm_plateau;
	MyGPIO_Struct_TypeDef  gpio_pwm_sens;
	MyUSART_Struct_Typedef usart_telecommande;
	MyGPIO_Struct_TypeDef  gpio_usart_rx_telecommande;	
	
	
	// Initialisation des périphériques
	// Girouette
	gir_GPIO_A.GPIO=GPIOA;
	gir_GPIO_A.pin=9;
	gir_GPIO_A.config=IN_PULLDOWN;
	gpio_init(&gir_GPIO_A);
	
	gir_GPIO_B.GPIO=GPIOA;
	gir_GPIO_B.pin=8;
	gir_GPIO_B.config=IN_PULLDOWN;
	gpio_init(&gir_GPIO_B);
	
	gir_GPIO_I.GPIO=GPIOA;
	gir_GPIO_I.pin=12;
	gir_GPIO_I.config=IN_FLOATING;
	gpio_init(&gir_GPIO_I);

	gir_compt_inc.Timer = TIM1;
	gir_compt_inc.ARR = 1439;
	gir_compt_inc.PSC = 0x0;
	
	timer_base_init(&gir_compt_inc);
	Gir_Config(gir_compt_inc.Timer);
	
	// Voiles
	voiles_init_pwm(&timer_voiles, &gpio_pwm_voiles);
	
	// Plateau
	orientation_init(&timer_pwm_plateau, &gpio_pwm_plateau, &gpio_pwm_sens, &usart_telecommande, &gpio_usart_rx_telecommande);
	
	// Start timers
	TIMER_BASE_START(gir_compt_inc.Timer);
	TIMER_BASE_START(timer_voiles.Timer);
	TIMER_BASE_START(timer_pwm_plateau.Timer);
	// Boucle infinie
	do
	{
		// Récupération de l'angle de la girouette
		gir_angle=Gir_Angle();
		if (gpio_read(gir_GPIO_I.GPIO,gir_GPIO_I.pin))
		{
			Gir_Reset();
		}
		// 
		timer_pwm_changecycle(timer_voiles.Timer, voiles_duty_cycle_pwm(gir_angle), 2);
		// Gestion de la vitesse de rotation du plateau
		orientation_gestion_plateau(&timer_pwm_plateau, &gpio_pwm_sens, &usart_telecommande);
	}while(1);
}
