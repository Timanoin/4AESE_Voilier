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
#include "driver_adc.h"

#include "m_envoi.h"

int main(void)
{
	// Déclaration des structures
	MyUSART_Struct_Typedef usart;
	MyGPIO_Struct_TypeDef gpio_tx;
	MyADC_Struct_TypeDef adc;
	MyGPIO_Struct_TypeDef gpio_adc;
	
	// USART en transmission
	usart.usart = USART3;
	usart.stop_bits = 2;
	usart.baud_rate_div = 3750;	
	usart_init(&usart);	
	// Configuration pin tx usart
	gpio_tx.GPIO = GPIOB;
	gpio_tx.pin = 10;
	gpio_tx.config = OUT_PUSHPULL;	
	gpio_init(&gpio_tx);
	
	envoi_init(&adc, &gpio_adc);
	
	// Départ du comptage
  TIMER_BASE_START(TIM3);
	// Boucle infinie
	do
	{
		envoi_info_batterie(&usart, &adc);
	}while(1);
}


