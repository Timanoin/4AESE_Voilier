
#ifndef INC_MYSPI_H_
#define INC_MYSPI_H_

#include "stm32f10x.h"

/*************************************************************************************
=====================    By Periph team 	INSA GEI	2022	 ===========================
*************************************************************************************/

/*
*************************************************************************************
=====================     I2C les IO STM32F103      =================================
*************************************************************************************
Les IO sont pris en charge par la lib, pas besoin de faire les configurations


Sur la Nucléo , le SPI1 est perturbé par la LED2 (PA5), mais doit pouvoir subir les front SCK qd même (LED clignote vite..)
le SPI2 n'est pas utilisable car pin non connectées par défaut (sauf à modifier les SB). En fait la Nucléo fait un choix entre SPI1 
et SPI2 par soudage jumper (SB).

-> Utiliser SPI1 avec la carte Nucléo
  
 * 	**IO SPI 1**
	SPI1_NSS PA4
	SPI1_SCK PA5
	SPI1_MISO  PA6  
	SPI1_MOSI  PA7  

	**IO SPI 2**
	SPI2_NSS PB12
	SPI2_SCK PB13
	SPI2_MISO  PB14  
	SPI2_MOSI  PB15  





*************************************************************************************
==================== Fondamentaux SPI      ==========================================
*************************************************************************************
- Bus Synchrone, 4 fils (même si on peut l'utiliser en 3 fils)
- Transfert à l'octet
- Protocole entre un Master (contrôle SCK) et un Slave
- SCK permet de synchroniser les bits de chaque octet. Il se configure par :
	* son niveau de repos : ici niveau '1'
	* le front actif de synchronisation pour chaque bit : ici front montant (front up durant bit stable)
- /CS ou /NSS active le slave sur l'état bas
- MOSI : Master Out Slave In (donc data circulant du Master vers le Slave, donc écriture dans le Slave)
- MISO : Master In Slave Out (donc data circulant du Slave vers le Master, donc lecture du Slave)

Bien que la lib propose une fonction d'écriture et de lecture :
	* une écriture s'accompagne obligatoirement d'une lecture (bidon)
	* une lecture s'accompagne obligatoirement d'une écriture (bidon)
La gestion /CS = /NSS se fait "à la main". On peut alors lire toute une série d'octets
en laissant /CS à l'état bas pendant toute la durée de circulation des octets.


*************************************************************************************
==================== La lib SPI            ==========================================
*************************************************************************************

fonctions essentielles :

MySPI_Init 
MySPI_Send 
MySPI_Read
MySPI_Set_NSS
MySPI_Clear_NSS


==========================================================================================*/




/*========================================================================================= 
														INITIALISATION SPI
========================================================================================= */

/**
	* @brief Configure le SPI spécifié : FSCK = 281kHz, Repos SCK = '1', Front actif = up
					 Gestion /CS logicielle à part, configure les 4 IO
					 - SCK, MOSI : Out Alt push pull
					 - MISO : floating input
					 - /NSS (/CS) : Out push pull
	* @param SPI_TypeDef * SPI : SPI1 ou SPI2 
  */
void MySPI_Init(SPI_TypeDef * SPI);



/**
  * @brief Envoie un octet (/CS non géré, à faire logiciellement)
					 Plus en détail, émission de l'octet souhaité sur MOSI
					 Lecture en même temps d'un octet poubelle sur MISO (non exploité)
  * @param : char ByteToSend : l'octet à envoyer
  */
void MySPI_Send(char ByteToSend);


/**
  * @brief Reçoit un octet (/CS non géré, à faire logiciellement)
					 Plus en détail, émission d'un octet bidon sur MOSI (0x00)
					 pour élaborer les 8 fronts sur SCK et donc piloter le slave en lecture 
					 qui répond sur MISO
	* @param : none
	* @retval : l'octet lu.
  */
char MySPI_Read(void);



/**
  * @brief Positionne /CS = /NSS à '1'. A utiliser pour borner les octets à transmettre/recevoir
	* @param : none
  */
void MySPI_Set_NSS(void);



/**
  * @brief Positionne /CS = /NSS à '0'.  A utiliser pour borner les octets à transmettre/recevoir
	* @param :none
  */
void MySPI_Clear_NSS(void);

#endif 
