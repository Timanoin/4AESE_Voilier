#ifndef __DRIVER_ADC_H__
#define __DRIVER_ADC_H__

#define ADC_SQR1_L_POS 20
#define ADC_CR2_EXTSEL_POS 17

#include "stm32f10x.h"

typedef struct {
    ADC_TypeDef* ADC;
		char 				 channel;
} MyADC_Struct_TypeDef;

// Démarre l'ADC
#define ADC_START(ADC) ((ADC)->CR2|=ADC_CR2_ADON)

// Initialise l'ADC choisi.
void adc_init(MyADC_Struct_TypeDef* adc_struct);

// Lance une conversion sur le channel initialisé
void adc_convert_once(ADC_TypeDef* ADC);

// Récupère la donnée convertie
short adc_get_data(ADC_TypeDef* ADC);




/************
 * HANDLERS *
 ************/

void ADC1_2_IRQHandler (void);

#endif
