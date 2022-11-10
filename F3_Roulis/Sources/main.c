/*
*	Title       : main.c
* Author      : Arthur Gautheron, Olivier Lagrost, Kenza Bouzergan, Tran Kien Trunh
* Date        : 13/10/2022
* Description : Le programme principal du projet Voilier.
*/

#include "stm32f10x.h"
#include "driver_timer.h"
#include "driver_gpio.h"
#include "roulis.h"
#include "MySPI.h"
#include "m_voiles.h"

int main(void)
{
	int x =0;
	int y =0;
	int z =0;
	// Déclaration des structures
	My_roulis_struct MyRoulis ; 
	MyTimer_Struct_TypeDef TimerVoiles;
	
	// Initialisation des périphériques
	roul_init(SPI1);
	roul_lancement();
	// Boucle infinie
	do
	{
		roul_lecture(MyRoulis);
		x=MyRoulis.AxeX1 * 0.0078;
		y=MyRoulis.AxeY1 * 0.0078;
		z=MyRoulis.AxeZ1 * 0.0078;
		roul_voiles_relache(TimerVoiles, z);
	}while(1);
}

