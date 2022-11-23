#include "m_envoi.h"
#include "stm32f10x.h"
#include <stdlib.h>
#include <string.h>

#include "driver_timer.h"
#include "driver_gpio.h"
#include "driver_usart.h"
#include "driver_adc.h"


// FONCTION D'INITIALISATION
// Initialise les peripheriques necessaires a l'envoi d'informations
void envoi_init(MyADC_Struct_TypeDef* adc_batterie,
								MyGPIO_Struct_TypeDef* gpio_adc_batterie)
{
	adc_batterie->ADC = ADC1;
	adc_batterie->channel = 12;
	adc_init(adc_batterie);
	
	gpio_adc_batterie->GPIO = GPIOC;
	gpio_adc_batterie->pin = 2;
	gpio_adc_batterie->config = IN_ANALOG;
}

// Envoie l'état de la batterie à la télécommande
void envoi_info_batterie(MyUSART_Struct_Typedef* usart, MyADC_Struct_TypeDef* adc_batterie)
{
	short data;
	int etat_batterie;
	
	adc_convert_once(adc_batterie->ADC);
	data = adc_get_data(adc_batterie->ADC);
	
	etat_batterie = (int)((data*100)/MAX_BATTERIE);
	if (etat_batterie < POURCENTAGE_BATTERIE_FAIBLE)
	{
			usart_transmit_string(usart->usart, MESSAGE_BATTERIE_FAIBLE, TAILLE_MESSAGE_BATTERIE_FAIBLE);
	}
}

// Envoie l'information du charivement si jamais il a lieu
void envoi_info_chavirement(MyUSART_Struct_Typedef* usart)
{
		usart_transmit_string(usart->usart, MESSAGE_CHAVIREMENT, strlen(MESSAGE_CHAVIREMENT));
}


