#include "driver_gpio.h"
#include "stm32f10x.h"

//RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN;


// Initialise un GPIO avec la configuration souhait�e
void gpio_init ( MyGPIO_Struct_TypeDef* GPIOStructPtr )
{
	// VERIFIER INIT TIMER DU GPIO CONCERNE
	if (GPIOStructPtr->GPIO == GPIOA)
	{
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	} 	
	else if (GPIOStructPtr->GPIO == GPIOB)
	{
		RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	}
	else
	{
		RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	}
	
	// D�termination CRH/CRL en fonction du pin
	if(GPIOStructPtr->pin < 8) 
	{
		GPIOStructPtr->GPIO->CRL &= ~(0xF << GPIOStructPtr->pin*4);
		
		// Configuration
		if (GPIOStructPtr->config == IN_PULLUP)
		{
			// Cas sp�cifique Pullup : on place le bit ODR correspondant � 1
			gpio_set(GPIOStructPtr->GPIO, GPIOStructPtr->pin);
			GPIOStructPtr->GPIO->CRL |= (IN_PULLDOWN << GPIOStructPtr->pin*4);
		} 
		else if (GPIOStructPtr->config == IN_PULLDOWN)
		{
			// Cas sp�cifique Pulldown : on place le bit ODR correspondant � 0
			gpio_reset(GPIOStructPtr->GPIO, GPIOStructPtr->pin);
			GPIOStructPtr->GPIO->CRL |= (IN_PULLDOWN << GPIOStructPtr->pin*4);
		}
		else
		{
			GPIOStructPtr->GPIO->CRL |= (GPIOStructPtr->config << GPIOStructPtr->pin*4);
		}
	}
	else
	{
		GPIOStructPtr->GPIO->CRH &= ~(0xF << ((GPIOStructPtr->pin-8)*4));
		
		// Configuration
		if (GPIOStructPtr->config == IN_PULLUP)
		{
			// Cas sp�cifique Pullup : on place le bit ODR correspondant � 1
			gpio_set(GPIOStructPtr->GPIO, GPIOStructPtr->pin);
			GPIOStructPtr->GPIO->CRH |= (IN_PULLDOWN << (GPIOStructPtr->pin-8)*4);
		} 
		else if (GPIOStructPtr->config == IN_PULLDOWN)
		{
			// Cas sp�cifique Pulldown : on place le bit ODR correspondant � 0
			gpio_reset(GPIOStructPtr->GPIO, GPIOStructPtr->pin);
			GPIOStructPtr->GPIO->CRH |= (IN_PULLDOWN << (GPIOStructPtr->pin-8)*4);
		}
		else
		{
			GPIOStructPtr->GPIO->CRH |= (GPIOStructPtr->config << (GPIOStructPtr->pin-8)*4);
		}
	}
}


// Lit l'�tat du bit d'entr�e correspondant au port 
int gpio_read ( GPIO_TypeDef* GPIO , char pin )
{
	return GPIO->IDR & (1 << pin);
} 

// Place le bit de sortie correspondant au port � 1
void gpio_set ( GPIO_TypeDef* GPIO , char pin )
{
	GPIO->ODR |= (1 << pin);
}


// Place le bit de sortie correspondant au port � 0
void gpio_reset ( GPIO_TypeDef* GPIO , char pin )
{
	GPIO->ODR &= ~(1 << pin);
}


// Echange la valeur du bit de sortie (1 -> 0, 0 -> 1)
void gpio_toggle ( GPIO_TypeDef * GPIO , char pin ) 
{
	GPIO->ODR ^= (1 << pin);
} 
