#include "m_orientation.h"
#include "stm32f10x.h"

#include "driver_timer.h"
#include "driver_gpio.h"
#include "driver_usart.h"

#define ORIENTATION_PWM_CH 1

// FONCTION D'INITIALISATION
// Initialise la PWM necessaire a l'orientation du plateau
void orientation_init(MyTimer_Struct_TypeDef* timer, 
											MyGPIO_Struct_TypeDef*  gpio_pwm,
											MyGPIO_Struct_TypeDef*  gpio_sens,
											MyUSART_Struct_Typedef* usart)
{	
	// Initialisation timer
	timer->Timer = TIM3;
	timer->ARR = 0x0E10; // 0d3600
	timer->PSC = 0;	
	timer_base_init(timer);
	
	// Initialisation PWM
	timer_pwm(timer->Timer, ORIENTATION_PWM_CH);

	// Configuration pin de sortie PWM
	gpio_pwm->GPIO = GPIOA;
	gpio_pwm->pin = 6;
	gpio_pwm->config = OUT_ALT_PUSHPULL;	
	gpio_init(gpio_pwm);
	
	// Configuration pin de sortie sens
	gpio_sens->GPIO = GPIOA;
	gpio_sens->pin = 7;
	gpio_sens->config = OUT_PUSHPULL;	
	gpio_init(gpio_sens);
	
	// Configuration USART
	*usart = usart_struct_init(USART3, 2, 0xEA6); //0xEA6 : baud rate = 9600
	usart_init(usart);
}

// Renvoie le signe d'un char signé
static char orientation_get_signe(signed char data)
{
	return data < 0;
}

// Renvoie la valeur absolue d'un char signé
static char orientation_val_abs(signed char data)
{
	return (data < 0) ? -data : data;
}

// Récupère l'information dans le buffer de l'USART choisi, 
// et modifie le duty cycle de la PWM envoyée au servomoteur du plateau, 
// ainsi que le bit de signe
void orientation_gestion_plateau(MyTimer_Struct_TypeDef* timer_pwm, 
																 MyGPIO_Struct_TypeDef*  gpio_sens,
																 MyUSART_Struct_Typedef* usart)
{
	char data_buffer = usart_get_data_buffer(usart->usart);
	char duty_cycle = orientation_val_abs(data_buffer);
	// Bit de signe
	if (orientation_get_signe(data_buffer))
	{
		gpio_set(gpio_sens->GPIO, gpio_sens->pin);
	}
	else
	{
		gpio_reset(gpio_sens->GPIO, gpio_sens->pin);
	}
	// Changement dutycycle PWM
	timer_pwm_changecycle(timer_pwm->Timer, (float)duty_cycle, ORIENTATION_PWM_CH);
}

