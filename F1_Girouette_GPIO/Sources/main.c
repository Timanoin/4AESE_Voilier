/*
*	Title       : main.c
* Author      : Arthur Gautheron, Olivier Lagrost, Kenza Bouzergan, Tran Kien Trunh
* Date        : 13/10/2022
* Description : Le programme principal du projet Voilier.
*/

#include "stm32f10x.h"
#include "driver_timer.h"
#include "driver_gpio.h"
#include "girouette.h"


//void ma_fonction(void)
//	{
//		TIM1->SR &= ~TIM_SR_UIF;
		
//	}
	
int main(void)
{
	// Déclaration des structures
	MyGPIO_Struct_TypeDef GPIO_1;
	MyGPIO_Struct_TypeDef GPIO_2;
	MyGPIO_Struct_TypeDef GPIO_3;
	MyTimer_Struct_TypeDef timer;
	int Angle;
	

	// Structure configuration
	GPIO_1.GPIO=GPIOA;
	GPIO_1.pin=9;
	GPIO_1.config=IN_PULLDOWN;
	gpio_init(&GPIO_1);
	
	GPIO_2.GPIO=GPIOA;
	GPIO_2.pin=8;
	GPIO_2.config=IN_PULLDOWN;
	gpio_init(&GPIO_2);
	
	GPIO_3.GPIO=GPIOA;
	GPIO_3.pin=12;
	GPIO_3.config=IN_FLOATING;
	gpio_init(&GPIO_3);

	timer.Timer = TIM1;
	timer.ARR = 1439;
	timer.PSC = 0x0;
	
	// Initialisation du timer et départ du comptage
	timer_base_init(&timer);
	Gir_Config(timer.Timer);
	
	//Récupération de l'angle
	Angle=Gir_Angle();

	// Boucle infinie
	do
	{
		if (gpio_read(GPIO_3.GPIO,GPIO_3.pin))
		{
			Gir_Reset();
		}
	}while(1);
}

