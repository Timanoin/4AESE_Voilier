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
	MyUSART_Struct_Typedef usart;
	MyGPIO_Struct_TypeDef gpio;
	usart.usart = USART3;
	usart.stop_bits = 2;
	usart.baud_rate_div = 3750;	
	usart_init(&usart);
	
	// Configuration pin d'entrée GPIO
	gpio.GPIO = GPIOB;
	gpio.pin = 10;
	gpio.config = OUT_PUSHPULL;	
	gpio_init(&gpio);
	
	// Départ du comptage
  TIMER_BASE_START(TIM3);
	// Boucle infinie
	do
	{
		usart_transmit_string(usart.usart, "haha \n", 6);
	}while(1);
}


