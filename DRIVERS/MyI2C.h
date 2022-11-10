#ifndef _I2C_
#define _I2C_

#include "stm32f10x.h"

/*************************************************************************************
=====================    By Periph team 	INSA GEI	2022	 ===========================
*************************************************************************************/

/*
*************************************************************************************
=====================     I2C les IO STM32F103      =================================
*************************************************************************************

Les IO sont pris en charge par la lib, pas besoin de faire les configurations Alt OD.

**I2C1**
SCL PB6
SDA PB7

**I2C2**
SCL PB10
SDA PB11


*************************************************************************************
==================== Fondamentaux I2C      ==========================================
*************************************************************************************
- Bus synchrone Low speed (<100kHz) ou high speed (=400kHz), Ici Low speed 100kHz.
- Transfert octet par octet, poids fort en premier, avec aquittement pour chaque octet
- Deux lignes SDA et SCL (horloge) en open drain, repos '1'
- bit "normal" = SDA stable lors du pulse SCL (ie durant l'état haut de SCL, SDA est stable)
- bit Start/Stop/Restart = SDA non stable lorsque SCL vaut '1' (violation règle précédente)
			* Start : front descendant de SDA lorsque SCL vaut '1'
			* Stop  : front montant de SDA lorsque SCL = '1'
			* Restart = Start en cours de trame (typiquement pour changer Write/read).
- uC en Mode Master uniquement (c'est notre choix) : c'est le uC qui est maître de l'horloge SCL.
- Le Slave a une @ 7 bits. On ajoute un bit LSB qui est /WR (donc 0 pour écriture, 1 pour lecture)
- Une adresse s'écrit donc |a6 a5 a4 a3 a2 a1 a0 /WR| ce qui donne 8 bits. Elle indique une future
lecture ou écriture. 
On parle d'@ 7 bits en regroupant |a6 a5 a4 a3 a2 a1 a0|
On parle d'@ 8 bits en regroupant |a6 a5 a4 a3 a2 a1 a0 /WR| (donc une @ écriture, une @ lecture)
NB : préférer le concept @7bits...c'est plus clair.

- On peut lire ou écrire une ou plusieurs données à la suite. C'est lors de l'envoie de l'adresse Slave
par le Master que le sens à venir pour les datas est indiqué.
- En écriture, 
			* les Ack sont faits par le slave après chaque octet envoyé par le master (Ack = mise à 0 le bit 9).
- En lecture, 
			* dès que le l@ slave est transmise (/RW = 1), et le Ack réalisé, le slave positionne le bit 7 
			du prochain octet à lire sur SDA par anticipation ,			
			* le master enchaîne ses pulses (9), lors du pulse 9 (le dernier) c'est le master qui acquite.
			* Après chaque acquitement, le Slave amorce le prochain octet en positionnant son bit 7 sur SDA 
			* Après le dernier octet, le Master génère un stop.
					* Pour pouvoir générer le stop, le Master doit piloter SDA, or ce n'est pas possible puisque
					le Slave positionne le futur bit 7 ... Pour régler ce problème :
					lors du dernier transfert, le Master N'acquitte PAS (NACK). Ainsi le Slave ne
					propose plus le bit 7 du prochain octet sur SDA et libère SDA. Le Master peut alors clôturer la 
					communication avec un Stop.




======= Echange typique avec un Slave  ================================================================
- Une lecture ou écriture se fait vers un Slave et à partir d'une adresse mémoire donnée (pointeur interne).
Ce pointeur est automatiquement incrémenté dans le slave lors des accès écriture ou lecture.

- Ecriture de N octets , trame complète (@ = adresse slave, pt = valeur de chargement du pointeur interne)
|Start Cond   |@6|@5|@4|@3|@2|@1|@0| Wr =0 |Slave ACK|
|pt7|pt6|pt5|pt4|pt3|pt2|pt1|pt0|Slave ACK| 
|d7|d6|d5|d4|d3|d2|d1|d0|Slave ACK| (data 1)
.....
|d7|d6|d5|d4|d3|d2|d1|d0|Salve ACK|Stop Cond| (data N)

- Lecture de N octets à partir d'une adresse de pointeur donnée
|Start Cond   |@6|@5|@4|@3|@2|@1|@0| Wr =0 |Slave ACK|
|pt7|pt6|pt5|pt4|pt3|pt2|pt1|pt0|Slave ACK| 
|ReStart Cond   |@6|@5|@4|@3|@2|@1|@0| Wr =1 |Slave ACK|    (NB: restart nécessaire pour changer écriture / lecture)
|d7|d6|d5|d4|d3|d2|d1|d0|Master ACK| (data 1)
.....
|d7|d6|d5|d4|d3|d2|d1|d0|Master ACK| (data N-1)
|d7|d6|d5|d4|d3|d2|d1|d0|Master NACK|Stop Cond| (data N)





*************************************************************************************
==================== La lib I2C            ==========================================
*************************************************************************************

3 fonctions essentielles :
MyI2C_Init
MyI2C_PutString
MyI2C_GetString

1 fonction spéciale : MyI2C_Get_Error

Les fonctions Put/Get string fonctionnent sur le principe classique décrit précédemment
(Slave@, Pter @, Data...).
La fonction init prend parmi ses paramètres le nom d'une fct callback d'erreur.
En fait, le driver gère en IT les erreurs possibles I2C. La fonction MyI2C_Get_Error permet de 
recevoir un code erreur.
Il est conseillé d'utiliser ce callback. Si on tombe dedans, c'est qu'une erreur s'est produite.
Si le code erreur est "inconnu", souvent c'est qu'il y  a un soucis à l'adressage slave:
Vérifier alors la connectique physique SDA/SCL ainsi que l'alimentation du slave ou tout simplement
l'@ slave !


==========================================================================================*/





/*========================================================================================= 
														GESTION ERREURS 
========================================================================================= */
typedef enum 
{
	OK,
	BusError, 		// 
	AckFail,  		// Pas,d'ack
	TimeOut,			// SCL est resté plus de 25ms à l'état bas
	UnknownError 	// IT erreur déclenchée mais pas de flag explicite ...
} MyI2C_Err_Enum;




/**
  * @brief  Retourne les erreurs I2C
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @retval Type d'erreur rencontrée , voir MyI2C_Err_Enum
  */

MyI2C_Err_Enum MyI2C_Get_Error(I2C_TypeDef * I2Cx);



/*========================================================================================= 
														INITIALISATION I2C
========================================================================================= */


/**
  * @brief  Initialise l'interface I2C (1 ou 2) 
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param char IT_Prio_I2CErr 0 à 15 (utilisé en cas d'erreur, IT courte et non bloquante
	* @param  *ITErr_function : callback à utiliser pour sortir d'un plantage transmission
  * @retval None 
  * @Example  MyI2C_Init(I2C1, 2,My_CallbackErr);

	

  */
void MyI2C_Init(I2C_TypeDef * I2Cx, char IT_Prio_I2CErr, void (*ITErr_function) (void));



/*========================================================================================= 
														EMISSION I2C : PutString
========================================================================================= */


// Structure de données pour l'émission ou la réception I2C :
typedef struct
{
	char SlaveAdress7bits;  // l'adresse I2C du slave device
	char * Ptr_Data;				// l'adresse du début de tableau char à recevoir/émettre (tableau en RAM uC) 
	char Nb_Data;						// le nbre d'octets à envoyer / recevoir  
}
MyI2C_RecSendData_Typedef;



/**
	* @brief|Start Cond   |@6|@5|@4|@3|@2|@1|@0| Wr =0 |Slave ACK|
					|pt7|pt6|pt5|pt4|pt3|pt2|pt1|pt0|Slave ACK| 
					|d7|d6|d5|d4|d3|d2|d1|d0|Slave ACK| (data 1)
					.....
					|d7|d6|d5|d4|d3|d2|d1|d0|Salve ACK|Stop Cond| (data N) 

  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  PteurAdress = adresse de démarrage écriture à l'interieur du slave I2C
  * @param  DataToSend, adresse de la structure qui contient les informations à transmettre 
             voir définition	MyI2C_RecSendData_Typedef				
  * @retval None
	* @Example MyI2C_PutString(I2C1,0xAA, &MyI2C_SendTimeData);
  *					 Ecrit dans le slave câblé sur I2C1 à partir de l'@ mémoire interne Slave 0xAA

  */
void MyI2C_PutString(I2C_TypeDef * I2Cx, char PteurAdress, MyI2C_RecSendData_Typedef * DataToSend);










/*========================================================================================= 
														Réception I2C : GetString 
========================================================================================= */

/**
	* @brief  |Start Cond   |@6|@5|@4|@3|@2|@1|@0| Wr =0 |Slave ACK|
						|pt7|pt6|pt5|pt4|pt3|pt2|pt1|pt0|Slave ACK| 
						|ReStart Cond   |@6|@5|@4|@3|@2|@1|@0| Wr =1 |Slave ACK|
						|d7|d6|d5|d4|d3|d2|d1|d0|Master ACK| (data 1)
						.....
						|d7|d6|d5|d4|d3|d2|d1|d0|Master NACK|Stop Cond| (data N)

  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  PteurAdress = adresse de démarrage lecture à l'interieur du slave I2C
  * @param  DataToSend, adresse de la structure qui contient les informations nécessaires à la
						réception des données voir définition	MyI2C_RecSendData_Typedef				
  * @retval None
	* @Example	MyI2C_GetString(I2C1,0xAA, &MyI2C_RecevievedTimeData);
							Lit dans le slave câblé sur I2C1 à partir de l'@ mémoire interne Slave 0xAA
  */
void	MyI2C_GetString(I2C_TypeDef * I2Cx, char PteurAdress, MyI2C_RecSendData_Typedef * DataToReceive);




#endif
