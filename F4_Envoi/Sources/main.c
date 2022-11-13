/*
*	Title       : main.c
* Author      : Arthur Gautheron, Olivier Lagrost, Kenza Bouzergan, Tran Kien Trunh
* Date        : 13/10/2022
* Description : Le programme principal du projet Voilier.
*/

#include "stm32f10x.h"
#include <assert.h>

#include "driver_timer.h"
#include "driver_gpio.h"
#include "driver_usart.h"

int main(void)
{
	// Déclaration des structures
	int att = 0;
	MyUSART_Struct_Typedef usart;
	usart.usart = USART3;
	usart.stop_bits = 2;
	usart.baud_rate_div = 3750;	
	usart_init(&usart);
	
	// Départ du comptage
  TIMER_BASE_START(TIM3);
	att = 0;
	// Boucle infinie
	do
	{
		if (att++ > 10) 
		{
			usart_transmit_string(usart.usart, "caca", 4);
			att = 0;
		}
	}while(1);
}


