#include "m_orientation.h"
#include "stm32f10x.h"

#include "driver_timer.h"
#include "driver_gpio.h"
#include "driver_usart.h"

// FONCTION D'INITIALISATION
// Initialise la PWM necessaire a l'orientation du plateau
void orientation_init(MyTimer_Struct_TypeDef* timer, 
											MyGPIO_Struct_TypeDef*  gpio_pwm,
											MyGPIO_Struct_TypeDef*  gpio_sens,
											MyUSART_Struct_Typedef* usart,
											MyGPIO_Struct_TypeDef*  gpio_usart_rx)
{	
	// Initialisation timer
	timer->Timer = TIM3;
	timer->ARR = 0x0E10; // 0d3600
	timer->PSC = 0;	
	timer_base_init(timer);
	
	// Initialisation PWM
	timer_pwm(timer->Timer, 3);

	// Configuration pin de sortie PWM
	gpio_pwm->GPIO = GPIOB;
	gpio_pwm->pin = 0;
	gpio_pwm->config = OUT_ALT_PUSHPULL;	
	gpio_init(gpio_pwm);
	
	// Configuration pin de sortie sens
	gpio_sens->GPIO = GPIOB;
	gpio_sens->pin = 1;
	gpio_sens->config = OUT_PUSHPULL;	
	gpio_init(gpio_sens);
	
	// Configuration pin de sortie sens
	gpio_usart_rx->GPIO = GPIOB;
	gpio_usart_rx->pin = 11;
	gpio_usart_rx->config = IN_FLOATING;	
	gpio_init(gpio_usart_rx);
	
	
	// Configuration USART
		// Initialization of each component of the structure
	usart->usart = USART3;
	usart->stop_bits = 2;
	usart->baud_rate_div = 3750;	
	usart_init(usart);
	

}

// Renvoie le signe d'un char sign?
static char orientation_get_signe(signed char data)
{
	return data < 0;
}

// Renvoie la valeur absolue d'un char sign?
static char orientation_val_abs(signed char data)
{
	return (data < 0) ? -data : data;
}

// R?cup?re l'information dans le buffer de l'USART choisi, 
// et modifie le duty cycle de la PWM envoy?e au servomoteur du plateau, 
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
		gpio_reset(gpio_sens->GPIO, gpio_sens->pin);
	}
	else
	{
		gpio_set(gpio_sens->GPIO, gpio_sens->pin);
	}
	// Changement dutycycle PWM
	timer_pwm_changecycle(timer_pwm->Timer, (float)duty_cycle, 3);
}

