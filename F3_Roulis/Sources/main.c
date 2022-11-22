/*
*	Title       : main.c
* Author      : Arthur Gautheron, Olivier Lagrost, Kenza Bouzergan, Tran Kien Trunh
* Date        : 13/10/2022
* Description : Le programme principal du projet Voilier.
*/

#include "stm32f10x.h"
#include "driver_timer.h"
#include "driver_gpio.h"
#include "roulis.h"
#include "MySPI.h"
#include "m_voiles.h"
#include "math.h" 
#include "girouette.h"

#define pi 3.1415926535898

float Angle;
float X;
float Y;
float Z;
int chavire = 0;

int main(void)
{
	// Déclaration des structures
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
	My_roulis_struct MyRoulis ; 
	
	
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
	
	// Roulis
	roul_init(SPI1);
	roul_lancement();
	
	// Start timers
	TIMER_BASE_START(gir_compt_inc.Timer);
	TIMER_BASE_START(timer_voiles.Timer);

	
	Angle = 0;
	MyRoulis.AxeX = 0;
	MyRoulis.AxeY = 0;
	MyRoulis.AxeZ = 0;

	
	// Initialisation des périphériques

	// Boucle infinie
	do
	{
		gir_angle=Gir_Angle();
		if (gpio_read(gir_GPIO_I.GPIO,gir_GPIO_I.pin))
		{
			Gir_Reset();
		}
		// 
		
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
			timer_pwm_changecycle(timer_voiles.Timer, voiles_duty_cycle_pwm(gir_angle), 2);
	}while(1);
}

