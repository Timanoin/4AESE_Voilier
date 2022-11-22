#ifndef __M_ENVOI_H__
#define __M_ENVOI_H__

#include "stm32f10x.h"
#include "driver_timer.h"
#include "driver_gpio.h"
#include "driver_usart.h"
#include "driver_adc.h"

// Ne pas toucher...
#define MAX_BATTERIE 0x0479

// Zone éditable

#define POURCENTAGE_BATTERIE_FAIBLE 20
#define MESSAGE_BATTERIE_FAIBLE "* Batterie faible.\n"
#define TAILLE_MESSAGE_BATTERIE_FAIBLE 19

#define MESSAGE_CHAVIREMENT "* Le voilier chavire. \n* Les voiles sont relachees.\n"
#define TAILLE_MESSAGE_CHAVIREMENT 52

#define DEBUT_MESSAGE_ANGLE_VOILES "* Les voiles ont un angle de"
#define FIN_MESSAGE_ANGLE_VOILES "degres.\n"
// fin de zone éditable

// FONCTION D'INITIALISATION
// Initialise les peripheriques necessaires a l'envoi d'informations
void envoi_init(MyADC_Struct_TypeDef* adc_batterie,
								MyGPIO_Struct_TypeDef* gpio_adc_batterie);

// Envoie l'état de la batterie à la télécommande
void envoi_info_batterie(MyUSART_Struct_Typedef* usart, MyADC_Struct_TypeDef* adc_batterie);

// Envoie l'information du charivement si jamais il a lieu
void envoi_info_chavirement(MyUSART_Struct_Typedef* usart);

#endif
