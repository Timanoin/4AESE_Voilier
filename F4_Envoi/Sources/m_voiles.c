#include "m_voiles.h"
#include "stm32f10x.h"
#include "driver_timer.h"
#include "driver_gpio.h"

// Renvoie une valeur de l'angle de la girouette en valeur absolue 
// Paramètre : int angle_360 [0;359] donné par les données issues du codeur incrémental
// Retour    : int angle_girouette_abs [0;179]
static int angle_girouette_abs(int angle_360)
{
	if (angle_360 >180) angle_360 -= 360;
	return (angle_360>0) ? angle_360 : -angle_360;
}

// Renvoie l'angle souhaité pour les voiles en fonction de l'angle de la girouette
// Paramètre : int angle_girouette_abs [0;179]
// Retour    : int angle_voiles [0;90]
int angle_voiles(int angle_girouette_abs)
{
	return (angle_girouette_abs > 45) ? (angle_girouette_abs-45)*90/135 : 0;
}

// FONCTION DE CALCUL
// Renvoie une valeur de l'angle de la girouette en valeur absolue 
// Paramètre : int angle_360 [0;359]
// Retour    : float [DUTYCYCLE_0; DUTYCYCLE_90]
float voiles_duty_cycle_pwm(int angle_360)
{
	return (float) angle_voiles(angle_girouette_abs(angle_360))*(DUTYCYCLE_90 - DUTYCYCLE_0)/90 + DUTYCYCLE_0;
}

// FONCTION D'INITIALISATION
// Initialise la PWM necessaire au fonctionnement des voiles
void voiles_init_pwm(MyTimer_Struct_TypeDef* timer, MyGPIO_Struct_TypeDef* gpio)
{	
	// Initialisation timer
	timer->Timer = TIM2;
	timer->ARR = 0xFFFF;
	timer->PSC = 22;	
	timer_base_init(timer);
	
	// Initialisation PWM
	timer_pwm(timer->Timer, 2);

	// Configuration pin de sortie
	gpio->GPIO = GPIOA;
	gpio->pin = 1;
	gpio->config = OUT_ALT_PUSHPULL;	
	gpio_init(gpio);
}
