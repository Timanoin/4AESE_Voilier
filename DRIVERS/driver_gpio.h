#ifndef __MYGPIO_H__
#define __MYGPIO_H__

#include "stm32f10x.h"

typedef struct
{
	GPIO_TypeDef* GPIO ;
	char 					pin ; //numero de 0 a 15
	char 					config ; // voir ci dessous
} MyGPIO_Struct_TypeDef ;

#define IN_FLOATING 0x04 //0100
#define IN_PULLDOWN 0x08 //01000 + ODR 0
#define IN_PULLUP   0x18//11000 + ODR 1
#define IN_ANALOG   0x00 //0000

#define OUT_PUSHPULL     0x02 //0010
#define OUT_OD           0x06 //0110
#define OUT_ALT_PUSHPULL 0x0A //1010
#define OUT_ALT_OD       0x0E //1110

// Initialise un GPIO avec la configuration souhaitée
void gpio_init ( MyGPIO_Struct_TypeDef* GPIOStructPtr ) ;

// Lit l'état du bit d'entrée correspondant au port 
int gpio_read( GPIO_TypeDef* GPIO , char GPIO_Pin ) ;

// Place le bit de sortie correspondant au port à 1
void gpio_set ( GPIO_TypeDef* GPIO , char GPIO_Pin ) ;

// Place le bit de sortie correspondant au port à 0
void gpio_reset ( GPIO_TypeDef* GPIO , char GPIO_Pin ) ;

// Echange la valeur du bit (1 -> 0, 0 -> 1)
void gpio_toggle ( GPIO_TypeDef * GPIO , char GPIO_Pin ) ;
#endif
