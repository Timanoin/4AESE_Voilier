/*
*	Title       : main.c
* Author      : Arthur Gautheron, Olivier Lagrost, Kenza Bouzergan, Tran Kien Trunh
* Date        : 13/10/2022
* Description : Le programme principal du projet Voilier.
*/

#include "stm32f10x.h"
#include <assert.h>

#include "m_voiles.h"

void test_angle_voiles(void);
void ok(void);

int main(void)
{
	// Déclaration des structures
	
	// Initialisation des périphériques
	test_angle_voiles();
	
	// Boucle infinie
	do
	{
		
	}while(1);
}

void test_angle_voiles(void)
{
	if (angle_voiles(0) == 0) ok();
	if (angle_voiles(30) == 0) ok();
	if (angle_voiles(-45) == 0) ok();
	if (angle_voiles(180) == 90) ok();
	if (angle_voiles(-180) == -90) ok();
}

void ok(void)
{

}

