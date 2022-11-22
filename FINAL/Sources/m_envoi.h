#ifndef __M_ENVOI_H__
#define __M_ENVOI_H__

#include "stm32f10x.h"
#include "driver_timer.h"
#include "driver_gpio.h"
#include "driver_usart.h"
#include "driver_adc.h"

#define MAX_BATTERIE 0x0479

// FONCTION D'INITIALISATION
// Initialise les peripheriques necessaires a l'envoi d'informations
void envoi_init(MyADC_Struct_TypeDef* adc_batterie,
								MyGPIO_Struct_TypeDef* gpio_adc_batterie);

// Envoie l'état de la batterie à la télécommande
void envoi_info_batterie(MyUSART_Struct_Typedef* usart, MyADC_Struct_TypeDef* adc_batterie);

#endif
