#ifndef __M_ORIENTATION_H__
#define __M_ORIENTATION_H__

#include "stm32f10x.h"
#include "driver_timer.h"
#include "driver_gpio.h"
#include "driver_usart.h"

// FONCTION D'INITIALISATION
// Initialise les peripheriques necessaires a l'orientation du plateau
void orientation_init(MyTimer_Struct_TypeDef* timer_pwm, 
											MyGPIO_Struct_TypeDef*  gpio_pwm,
											MyGPIO_Struct_TypeDef*  gpio_sens,
											MyUSART_Struct_Typedef* usart);

// Renvoie le signe d'un char signé
static char orientation_get_signe(signed char data);

// Renvoie la valeur absolue d'un char signé
static char orientation_val_abs(signed char data);

// Récupère l'information dans le buffer de l'USART choisi, 
// et modifie le duty cycle de la PWM envoyée au servomoteur du plateau, 
// ainsi que le bit de signe
void orientation_gestion_plateau(MyTimer_Struct_TypeDef* timer_pwm, 
																 MyGPIO_Struct_TypeDef*  gpio_sens,
																 MyUSART_Struct_Typedef* usart);



#endif