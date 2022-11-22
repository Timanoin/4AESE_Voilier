#ifndef __M_VOILES_H__
#define __M_VOILES_H__

#include "driver_timer.h"
#include "driver_gpio.h"

#define DUTYCYCLE_0 10.0
#define DUTYCYCLE_90 5.0

// Renvoie une valeur de l'angle de la girouette en valeur absolue 
// Paramètre : int angle_360 [0;359] donné par les données issues du codeur incrémental
// Retour    : int angle_girouette_abs [0;179]
static int angle_voiles(int angle_girouette);

// Renvoie une valeur de l'angle de la girouette en valeur absolue 
// Paramètre : int angle_360 [0;359]
// Retour    : int angle_girouette_abs [0;179]
static int angle_girouette_abs(int angle_360);

// FONCTION DE CALCUL
// Renvoie une valeur de l'angle de la girouette en valeur absolue 
// Paramètre : int angle_360 [0;359]
// Retour    : float [DUTYCYCLE_0; DUTYCYCLE_90]
float voiles_duty_cycle_pwm(int angle_360);

// FONCTION D'INITIALISATION
// Initialise la PWM necessaire au fonctionnement des voiles
void voiles_init_pwm(MyTimer_Struct_TypeDef* timer, MyGPIO_Struct_TypeDef* gpio);

#endif
