/*
*	Title       : main.c
* Author      : Arthur Gautheron, Olivier Lagrost, Kenza Bouzergan, Tran Kien Trunh
* Date        : 13/10/2022
* Description : Le programme principal du projet Voilier.
*/

#include "stm32f10x.h"
#include "math.h" 
#include <string.h>
#include <stdio.h>
#define pi 3.1415926535898

#include "girouette.h"
#include "m_voiles.h"
#include "m_orientation.h"
#include "roulis.h"
#include "m_envoi.h"

#include "driver_gpio.h"
#include "driver_timer.h"
#include "driver_usart.h"
#include "driver_adc.h"
#include "MySPI.h"

MyUSART_Struct_Typedef usart_telecommande;
MyADC_Struct_TypeDef adc_batterie;
int angle_voile = 0;

char msg_chavire_envoye = 0;
char message_sent = 0;

int stkhandler_n = 0;

void it_func_message_voiles(void);
void init_systick(void);

// Fonction utilisée pour envoyée périodiquement un message
// sur l'angle des voiles à 

int main(void)
{
	/////////////////////////////////////////////
	// Déclaration des variables et structures //
	/////////////////////////////////////////////
	
	/***************************/
	/* F1 - bordage des voiles */
	/***************************/
	// Girouette
	MyGPIO_Struct_TypeDef gir_GPIO_A;
	MyGPIO_Struct_TypeDef gir_GPIO_B;
	MyGPIO_Struct_TypeDef gir_GPIO_I;
	MyTimer_Struct_TypeDef gir_compt_inc;
	int gir_angle;
	// Voiles
	MyTimer_Struct_TypeDef timer_voiles;
	MyGPIO_Struct_TypeDef gpio_pwm_voiles;
	
	/****************************/
	/* F2 - Rotation du plateau */
	/****************************/
	MyTimer_Struct_TypeDef timer_pwm_plateau;	// Timer de fréquence 20kHz : ARR = 3600, PSC = 0
	MyGPIO_Struct_TypeDef  gpio_pwm_plateau;
	MyGPIO_Struct_TypeDef  gpio_pwm_sens;
	MyGPIO_Struct_TypeDef  gpio_rx;
	
	/*********************************/
	/* F3 - Détection du chavirement */
	/*********************************/
	float X;
	float Y;
	float Z;
	char chavire = 0;
	My_roulis_struct MyRoulis ; 

	/***********************************************/
	/* F4 - Envoi d'informations à la télécommande */
	/***********************************************/
	MyGPIO_Struct_TypeDef gpio_tx;
	MyGPIO_Struct_TypeDef gpio_adc_batterie;
	
	char message_voiles[100];
	char message_angle[2];

	
	//////////////////////////////////////
	// Initialisation des périphériques //
	//////////////////////////////////////
	
	/***************************/
	/* F1 - bordage des voiles */
	/***************************/
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
	
	/****************************/
	/* F2 - Rotation du plateau */
	/****************************/
	// Plateau
	orientation_init(&timer_pwm_plateau, &gpio_pwm_plateau, &gpio_pwm_sens, &usart_telecommande, &gpio_rx);

	/*********************************/
	/* F3 - Détection du chavirement */
	/*********************************/	
	roul_init(SPI1);
	roul_lancement();
	
	/***********************************************/
	/* F4 - Envoi d'informations à la télécommande */
	/***********************************************/
	
	// Configuration pin tx usart
	gpio_tx.GPIO = GPIOB;
	gpio_tx.pin = 10;
	gpio_tx.config = OUT_ALT_PUSHPULL;	
	gpio_init(&gpio_tx);
	// Configuration ADC pour la mesure de la batterie	
	envoi_init(&adc_batterie, &gpio_adc_batterie);
	
	// Start timers
	TIMER_BASE_START(gir_compt_inc.Timer);
	TIMER_BASE_START(timer_voiles.Timer);
	TIMER_BASE_START(timer_pwm_plateau.Timer);
	
	init_systick();
	
	MyRoulis.AxeX = 0.0;
	MyRoulis.AxeY = 0.0;
	MyRoulis.AxeZ = 0.0;
	
	///////////////////////////////////////////
	// Boucle infinie du programme principal //
	///////////////////////////////////////////
	do {
		/****************************/
		/* F2 - Rotation du plateau */
		/****************************/
		// Gestion de la vitesse de rotation du plateau
		orientation_gestion_plateau(&timer_pwm_plateau, &gpio_pwm_sens, &usart_telecommande);
		
		/************************************************************/
		/* F1 - bordage des voiles et F3 - Détection du chavirement */
		/************************************************************/
		// Récupération de l'angle de la girouette
		gir_angle=Gir_Angle();
		if (gpio_read(gir_GPIO_I.GPIO,gir_GPIO_I.pin))
		{
			Gir_Reset();
		}
		// Fonctionnement du roulis
		roul_lecture(&MyRoulis);
		X=MyRoulis.AxeX;
		Y=MyRoulis.AxeY;
		Z=MyRoulis.AxeZ;
		// A intégrer dns voiles.c
		if ((Y >= 150.0) || (Y <= -150.0)){
			// chavirement !! 
			roul_voiles_relache(timer_voiles);
			chavire = 1;
			angle_voile = 0;
		}
		else 
		{
			// bordage normal
			chavire = 0;
			angle_voile = angle_voiles(angle_girouette_abs(gir_angle));
			timer_pwm_changecycle(timer_voiles.Timer, voiles_duty_cycle_pwm(gir_angle), 2);
		}

		/***********************************************/
		/* F4 - Envoi d'informations à la télécommande */
		/***********************************************/
	
		// Gestion de l'envoi du message 1 fois par chavirement
		if (!chavire) msg_chavire_envoye = 0;
		// Envoi d'un message si le bateau chavire
		if (!msg_chavire_envoye && chavire) 
		{
			envoi_info_chavirement(&usart_telecommande);
			msg_chavire_envoye = 1;
		}			
		if (!(stkhandler_n % 4) && !message_sent)
		{
			message_sent = 1;
			message_voiles[0] = 0;
			strcat(message_voiles, DEBUT_MESSAGE_ANGLE_VOILES);
			sprintf(message_angle, "%d", angle_voile);
			strcat(message_voiles, message_angle);
			strcat(message_voiles, FIN_MESSAGE_ANGLE_VOILES);
			usart_transmit_string(usart_telecommande.usart, message_voiles, strlen(message_voiles));
			// Gérer le temps + les autres infos
			envoi_info_batterie(&usart_telecommande, &adc_batterie);
		}

	}while(1);
}

void init_systick(void)
{
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk;
	// Pour une seconde : Systick module 4
	SysTick->LOAD = 0x00FFFFFF;
	NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn, 15);
}

void SysTick_Handler(void)
{
	SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk;
	stkhandler_n ++;
	message_sent = 0;
}
