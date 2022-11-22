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
- bit "normal" = SDA stable lors du pulse SCL (ie durant l'�tat haut de SCL, SDA est stable)
- bit Start/Stop/Restart = SDA non stable lorsque SCL vaut '1' (violation r�gle pr�c�dente)
			* Start : front descendant de SDA lorsque SCL vaut '1'
			* Stop  : front montant de SDA lorsque SCL = '1'
			* Restart = Start en cours de trame (typiquement pour changer Write/read).
- uC en Mode Master uniquement (c'est notre choix) : c'est le uC qui est ma�tre de l'horloge SCL.
- Le Slave a une @ 7 bits. On ajoute un bit LSB qui est /WR (donc 0 pour �criture, 1 pour lecture)
- Une adresse s'�crit donc |a6 a5 a4 a3 a2 a1 a0 /WR| ce qui donne 8 bits. Elle indique une future
lecture ou �criture. 
On parle d'@ 7 bits en regroupant |a6 a5 a4 a3 a2 a1 a0|
On parle d'@ 8 bits en regroupant |a6 a5 a4 a3 a2 a1 a0 /WR| (donc une @ �criture, une @ lecture)
NB : pr�f�rer le concept @7bits...c'est plus clair.

- On peut lire ou �crire une ou plusieurs donn�es � la suite. C'est lors de l'envoie de l'adresse Slave
par le Master que le sens � venir pour les datas est indiqu�.
- En �criture, 
			* les Ack sont faits par le slave apr�s chaque octet envoy� par le master (Ack = mise � 0 le bit 9).
- En lecture, 
			* d�s que le l@ slave est transmise (/RW = 1), et le Ack r�alis�, le slave positionne le bit 7 
			du prochain octet � lire sur SDA par anticipation ,			
			* le master encha�ne ses pulses (9), lors du pulse 9 (le dernier) c'est le master qui acquite.
			* Apr�s chaque acquitement, le Slave amorce le prochain octet en positionnant son bit 7 sur SDA 
			* Apr�s le dernier octet, le Master g�n�re un stop.
					* Pour pouvoir g�n�rer le stop, le Master doit piloter SDA, or ce n'est pas possible puisque
					le Slave positionne le futur bit 7 ... Pour r�gler ce probl�me :
					lors du dernier transfert, le Master N'acquitte PAS (NACK). Ainsi le Slave ne
					propose plus le bit 7 du prochain octet sur SDA et lib�re SDA. Le Master peut alors cl�turer la 
					communication avec un Stop.




======= Echange typique avec un Slave  ================================================================
- Une lecture ou �criture se fait vers un Slave et � partir d'une adresse m�moire donn�e (pointeur interne).
Ce pointeur est automatiquement incr�ment� dans le slave lors des acc�s �criture ou lecture.

- Ecriture de N octets , trame compl�te (@ = adresse slave, pt = valeur de chargement du pointeur interne)
|Start Cond   |@6|@5|@4|@3|@2|@1|@0| Wr =0 |Slave ACK|
|pt7|pt6|pt5|pt4|pt3|pt2|pt1|pt0|Slave ACK| 
|d7|d6|d5|d4|d3|d2|d1|d0|Slave ACK| (data 1)
.....
|d7|d6|d5|d4|d3|d2|d1|d0|Salve ACK|Stop Cond| (data N)

- Lecture de N octets � partir d'une adresse de pointeur donn�e
|Start Cond   |@6|@5|@4|@3|@2|@1|@0| Wr =0 |Slave ACK|
|pt7|pt6|pt5|pt4|pt3|pt2|pt1|pt0|Slave ACK| 
|ReStart Cond   |@6|@5|@4|@3|@2|@1|@0| Wr =1 |Slave ACK|    (NB: restart n�cessaire pour changer �criture / lecture)
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

1 fonction sp�ciale : MyI2C_Get_Error

Les fonctions Put/Get string fonctionnent sur le principe classique d�crit pr�c�demment
(Slave@, Pter @, Data...).
La fonction init prend parmi ses param�tres le nom d'une fct callback d'erreur.
En fait, le driver g�re en IT les erreurs possibles I2C. La fonction MyI2C_Get_Error permet de 
recevoir un code erreur.
Il est conseill� d'utiliser ce callback. Si on tombe dedans, c'est qu'une erreur s'est produite.
Si le code erreur est "inconnu", souvent c'est qu'il y  a un soucis � l'adressage slave:
V�rifier alors la connectique physique SDA/SCL ainsi que l'alimentation du slave ou tout simplement
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
	TimeOut,			// SCL est rest� plus de 25ms � l'�tat bas
	UnknownError 	// IT erreur d�clench�e mais pas de flag explicite ...
} MyI2C_Err_Enum;




/**
  * @brief  Retourne les erreurs I2C
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @retval Type d'erreur rencontr�e , voir MyI2C_Err_Enum
  */

MyI2C_Err_Enum MyI2C_Get_Error(I2C_TypeDef * I2Cx);



/*========================================================================================= 
														INITIALISATION I2C
========================================================================================= */


/**
  * @brief  Initialise l'interface I2C (1 ou 2) 
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param char IT_Prio_I2CErr 0 � 15 (utilis� en cas d'erreur, IT courte et non bloquante
	* @param  *ITErr_function : callback � utiliser pour sortir d'un plantage transmission
  * @retval None 
  * @Example  MyI2C_Init(I2C1, 2,My_CallbackErr);

	

  */
void MyI2C_Init(I2C_TypeDef * I2Cx, char IT_Prio_I2CErr, void (*ITErr_function) (void));



/*========================================================================================= 
														EMISSION I2C : PutString
========================================================================================= */


// Structure de donn�es pour l'�mission ou la r�ception I2C :
typedef struct
{
	char SlaveAdress7bits;  // l'adresse I2C du slave device
	char * Ptr_Data;				// l'adresse du d�but de tableau char � recevoir/�mettre (tableau en RAM uC) 
	char Nb_Data;						// le nbre d'octets � envoyer / recevoir  
}
MyI2C_RecSendData_Typedef;



/**
	* @brief|Start Cond   |@6|@5|@4|@3|@2|@1|@0| Wr =0 |Slave ACK|
					|pt7|pt6|pt5|pt4|pt3|pt2|pt1|pt0|Slave ACK| 
					|d7|d6|d5|d4|d3|d2|d1|d0|Slave ACK| (data 1)
					.....
					|d7|d6|d5|d4|d3|d2|d1|d0|Salve ACK|Stop Cond| (data N) 

  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  PteurAdress = adresse de d�marrage �criture � l'interieur du slave I2C
  * @param  DataToSend, adresse de la structure qui contient les informations � transmettre 
             voir d�finition	MyI2C_RecSendData_Typedef				
  * @retval None
	* @Example MyI2C_PutString(I2C1,0xAA, &MyI2C_SendTimeData);
  *					 Ecrit dans le slave c�bl� sur I2C1 � partir de l'@ m�moire interne Slave 0xAA

  */
void MyI2C_PutString(I2C_TypeDef * I2Cx, char PteurAdress, MyI2C_RecSendData_Typedef * DataToSend);










/*========================================================================================= 
														R�ception I2C : GetString 
========================================================================================= */

/**
	* @brief  |Start Cond   |@6|@5|@4|@3|@2|@1|@0| Wr =0 |Slave ACK|
						|pt7|pt6|pt5|pt4|pt3|pt2|pt1|pt0|Slave ACK| 
						|ReStart Cond   |@6|@5|@4|@3|@2|@1|@0| Wr =1 |Slave ACK|
						|d7|d6|d5|d4|d3|d2|d1|d0|Master ACK| (data 1)
						.....
						|d7|d6|d5|d4|d3|d2|d1|d0|Master NACK|Stop Cond| (data N)

  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  PteurAdress = adresse de d�marrage lecture � l'interieur du slave I2C
  * @param  DataToSend, adresse de la structure qui contient les informations n�cessaires � la
						r�ception des donn�es voir d�finition	MyI2C_RecSendData_Typedef				
  * @retval None
	* @Example	MyI2C_GetString(I2C1,0xAA, &MyI2C_RecevievedTimeData);
							Lit dans le slave c�bl� sur I2C1 � partir de l'@ m�moire interne Slave 0xAA
  */
void	MyI2C_GetString(I2C_TypeDef * I2Cx, char PteurAdress, MyI2C_RecSendData_Typedef * DataToReceive);




#endif
