#include "driver_usart.h"
#include "stm32f10x.h"
#include <stdlib.h>

signed char usart1_buffer = 0;
signed char usart3_buffer = 0;

/********
 * INIT *
 ********/

// Initialize the chosen USART.
// Parameter : MyUSART_Struct_Typedef* usart_struct = usart struct to initialize.
void usart_init(MyUSART_Struct_Typedef* usart_struct)
{
	// Start clock on the USART
	if (usart_struct->usart == USART1)
	{
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	}
	else if (usart_struct->usart == USART3)		
	{
		RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	}

	// Disable Parity
	usart_struct->usart->CR1 &= ~USART_CR1_PCE;
	// Configuration of transmission mode
	// Sending 8 bits each time (M=1 => 9 bits each time)
	usart_struct->usart->CR1 &= ~USART_CR1_M;
	// Number of stop bits	
	usart_struct->usart->CR2 &= ~USART_CR2_STOP;
	usart_struct->usart->CR2 |= usart_struct->stop_bits;
	// Baud rate
	usart_struct->usart->BRR = usart_struct->baud_rate_div;
	
	// Configuration of reception mode
	// Enable interruptions
	usart_struct->usart->CR1 |= USART_CR1_RXNEIE; // Register Not Empty Interruption Enable
	// USART starts searching for a start bit (Read Enable)
	usart_struct->usart->CR1 |= USART_CR1_RE;
	
	// Enable the USART
	usart_struct->usart->CR1 |= USART_CR1_UE;	
	
		// USART starts searching for a start bit (Read Enable)
	usart_struct->usart->CR1 |= USART_CR1_TE;
	
	// Interruptions
	NVIC_EnableIRQ(USART3_IRQn);
	NVIC_SetPriority(USART3_IRQn, 15);
	


}

/***********
 * UTILITY *
 ***********/

// Transmission of a string of char with USART
// Parameters : USART_TypeDef usart  = USART used for transmission
//              char*         data   = transmitted string of char 
// 							char					length = size of data
void usart_transmit_string(USART_TypeDef* usart, char* data, char length)
{
	int i;
	// Transmission of each character
	if(usart == USART1)
	{
		for (i=0; i<length; i++) 
		{
			while (!(USART1->SR & USART_SR_TXE));
			USART1->DR = data[i];
		}
	}
	else if (usart == USART3)
	{
		for (i=0; i<length; i++) 
		{
			while (!(USART3->SR & USART_SR_TXE));
			USART3->DR = data[i];
		}
	}
}

// Reading the character contained in the one byte buffer of the USART. 
// Buffer is updated with interruptions.
// Parameters : USART_TypeDef* usart = USART used for reception
// Returns    : char           data  = information received (1 byte)
char usart_get_data_buffer(USART_TypeDef* usart)
{
	char data;
	if (usart == USART1) data = usart1_buffer;
	else if (usart == USART3) data = usart3_buffer;
	return data;
}


/************
 * HANDLERS *
 ************/

void USART1_IRQHandler(void)
{
	// Clear interruption flag
	USART1->SR &= ~USART_SR_RXNE;	
	// Places the new received character in the buffer
	usart1_buffer = USART1->DR;
}
 
void USART3_IRQHandler(void)
{
	// Clear interruption flag
	USART3->SR &= ~USART_SR_RXNE;
	// Places the new received character in the buffer		
	usart3_buffer = USART3->DR;
}
