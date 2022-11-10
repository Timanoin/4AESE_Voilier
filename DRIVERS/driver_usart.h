/*************************************************
 * Name : driver_usart.h                         *
 * Author : Arthur Gautheron                     *
 * Date : 19/10/2022                             *
 * Description :                                 *
 * 		This driver allows the user to initialize  *
 * and use simple communication features with an *
 * USART. This driver does not handle all        *
 * features for the peripheral USART2.           *
 *************************************************/

#ifndef __DRIVER_USART_H__
#define __DRIVER_USART_H__

#include "stm32f10x.h"

#define MAX_SIZE_BUFFER_TR 128

typedef struct {
	USART_TypeDef* usart;
	char           stop_bits;     // 1, 2 or 3 (0.5, 1, 1.5)
	short					 baud_rate_div; /* format : mantissa [15;4], fraction [3;0]
																 * => USARTDIV = mantissa + fraction/16
																 * => baud_rate = f_clk/(16*USARTDIV) */
	
} MyUSART_Struct_Typedef;

/********
 * INIT *
 ********/

// Initialize and return a MyUSART_Struct_Typedef.
MyUSART_Struct_Typedef usart_struct_init(USART_TypeDef* usart, char stop_bits, short baud_rate_div);

// Initialize the chosen USART peripheral.
// Parameter : MyUSART_Struct_Typedef* usart_struct = usart struct to initialize.
void usart_init(MyUSART_Struct_Typedef* usart_struct);

// Initialize the chosen USART in transmission mode.
// Parameter : MyUSART_Struct_Typedef* usart_struct = usart struct to initialize.
static void usart_init_transmission(MyUSART_Struct_Typedef* usart_struct);

// Initialize the chosen USART in reception mode.
// Parameter : MyUSART_Struct_Typedef* usart_struct = usart struct to initialize.
static void usart_init_reception(MyUSART_Struct_Typedef* usart_struct);

/***********
 * UTILITY *
 ***********/
 
// Transmission of a string of char with UART
// Parameters : USART_TypeDef* usart  = USART used for transmission
//              char*          data   = transmitted string of char 
// 							char					 length = size of data
void usart_transmit_string(USART_TypeDef* usart, char* data, char length);

// Transmission of a char with UART
// Parameters : USART_TypeDef* usart     = USART used for transmission
//              char           character = transmitted char
static void usart_transmit_char(USART_TypeDef* usart, char character);

// Reading the character contained in the one byte buffer of the USART. 
// Buffer is updated with interruptions.
// Parameters : USART_TypeDef* usart = USART used for reception
// Returns    : char           data  = information received (1 byte)
char usart_get_data_buffer(USART_TypeDef* usart);


/************
 * HANDLERS *
 ************/
 
void USART1_IRQHandler(void);
void USART3_IRQHandler(void);

#endif
