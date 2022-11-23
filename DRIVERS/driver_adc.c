#include "stm32f10x.h"
#include "driver_adc.h"

char adc1_buffer_nempty = 0;
short adc1_buffer;
char adc2_buffer_nempty = 0;
short adc2_buffer;

// Initialise l'ADC choisi.
void adc_init(MyADC_Struct_TypeDef* adc_struct)
{
	// Allumage de la clock
	if (adc_struct->ADC == ADC1)
	{
		RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	}
	else if (adc_struct->ADC == ADC2)
	{
		RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;
	}
	
	// Allumage de l'ADC en mode single conversion
	adc_struct->ADC->CR2 &= ~ADC_CR2_CONT;
	adc_struct->ADC->CR2 |= ADC_CR2_ADON;

	// Selection d'un unique channel de conversion
	adc_struct->ADC->SQR1 = 0;	 // 0 = 1 conversion
	adc_struct->ADC->SQR3 = adc_struct->channel;
	
	// Conversion sur un Software trigger
	adc_struct->ADC->CR2 &= ~ADC_CR2_EXTSEL;
	adc_struct->ADC->CR2 |= (0x7 << ADC_CR2_EXTSEL_POS);
	
	// Déclenchement des interruptions
	adc_struct->ADC->CR1 |= ADC_CR1_EOCIE;
	NVIC_EnableIRQ(ADC1_2_IRQn);
	NVIC_SetPriority(ADC1_2_IRQn, 15);
}

// Lance une conversion sur le channel initialisé
void adc_convert_once(ADC_TypeDef* ADC)
{
	// Démarre l'ADC
	ADC_START(ADC);
	// Déclenche la conversion
	ADC->CR2 |= ADC_CR2_SWSTART;
}

// Récupère la donnée convertie
short adc_get_data(ADC_TypeDef* ADC)
{
	short data;
	if (ADC == ADC1 && adc1_buffer_nempty)
	{ 
		data = adc1_buffer;
	}
	else if (ADC == ADC2 && adc2_buffer_nempty)
	{
		data = adc2_buffer;
	}
	else
	{
		data = 0;
	}
	return data;
}

/************
 * HANDLERS *
 ************/

void ADC1_2_IRQHandler (void)
{
	if (ADC1->SR & ADC_SR_EOC)
	{
		// Lecture de la donnée
		adc1_buffer = ADC1->DR;
		adc1_buffer_nempty = 1;
		// Remise à zéro du flag d'interruption
		ADC1->SR &= ~ADC_SR_EOC;
	}
	else if (ADC2->SR & ADC_SR_EOC)
	{
		adc2_buffer = ADC2->DR;
		adc2_buffer_nempty = 1;
		// Remise à zéro du flag d'interruption
		ADC2->SR &= ~ADC_SR_EOC;
	}
}


