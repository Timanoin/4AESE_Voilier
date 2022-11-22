/*
*	Title       : main.c
* Author      : Arthur Gautheron, Olivier Lagrost, Kenza Bouzergan, Tran Kien Trunh
* Date        : 13/10/2022
* Description : Le programme principal du projet Voilier.
*/

#include "stm32f10x.h"
#include "math.h" 
#define pi 3.1415926535898

#include "girouette.h"
#include "m_voiles.h"
#include "m_orientation.h"
#include "roulis.h"

#include "driver_gpio.h"
#include "driver_timer.h"
#include "driver_usart.h"
#include "MySPI.h"

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
	MyUSART_Struct_Typedef usart_telecommande;
	MyGPIO_Struct_TypeDef  gpio_rx;
	
	/*********************************/
	/* F3 - Détection du chavirement */
	/*********************************/
	float Angle;
	float X;
	float Y;
	float Z;
	int chavire = 0;
	My_roulis_struct MyRoulis ; 

	/***********************************************/
	/* F4 - Envoi d'informations à la télécommande */
	/***********************************************/
	MyGPIO_Struct_TypeDef gpio_tx;
	MyADC_Struct_TypeDef adc_batterie;
	MyGPIO_Struct_TypeDef gpio_adc_batterie;
	
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
	gpio_tx.config = OUT_PUSHPULL;	
	gpio_init(&gpio_tx);
	// Configuration ADC pour la mesure de la batterie	
	envoi_init(&adc, &gpio_adc);
	
	// Start timers
	TIMER_BASE_START(gir_compt_inc.Timer);
	TIMER_BASE_START(timer_voiles.Timer);
	TIMER_BASE_START(timer_pwm_plateau.Timer);
	
	Angle = 0;
	MyRoulis.AxeX = 0;
	MyRoulis.AxeY = 0;
	MyRoulis.AxeZ = 0;
	
	///////////////////////////////////////////
	// Boucle infinie du programme principal //
	///////////////////////////////////////////
	do {
		/***************************/
		/* F1 - bordage des voiles */
		/***************************/
		// Récupération de l'angle de la girouette
		gir_angle=Gir_Angle();
		if (gpio_read(gir_GPIO_I.GPIO,gir_GPIO_I.pin))
		{
			Gir_Reset();
		}
		// Actionnement des voiles en fonction de l'angle recu
		timer_pwm_changecycle(timer_voiles.Timer, voiles_duty_cycle_pwm(gir_angle), 2);
		
		/****************************/
		/* F2 - Rotation du plateau */
		/****************************/
		//Gestion de la vitesse de rotation du plateau
		orientation_gestion_plateau(&timer_pwm_plateau, &gpio_pwm_sens, &usart_telecommande);
		
		/*********************************/
		/* F3 - Détection du chavirement */
		/*********************************/
		// Fonctionnement du roulis
		roul_lecture(&MyRoulis);
		X=MyRoulis.AxeX;
		Y=MyRoulis.AxeY;
		Z=MyRoulis.AxeZ;
		if (Y*Z>0){
			Angle = atan(Y/Z);
		}
		else 
			Angle = atan(-(Y/Z));
		
		if ((Y >= 150) || (Y <= -150)){
				roul_voiles_relache(timer_voiles);
				chavire = 1;
		}
		else 
		{
			timer_pwm_changecycle(timer_voiles.Timer, voiles_duty_cycle_pwm(gir_angle), 2);
		}

		/***********************************************/
		/* F4 - Envoi d'informations à la télécommande */
		/***********************************************/
		
		// Gérer le temps + les autres infos
		envoi_info_batterie(&usart, &adc);
		
	}while(1);
}
