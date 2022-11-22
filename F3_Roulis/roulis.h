#ifndef __MYGIROUETTE_H__
#define __MYGIROUETTE_H__

#include "stm32f10x.h"
#include "driver_gpio.h" 
#include "driver_timer.h"
#include "MySPI.h"
#include "m_voiles.h"

#define POWER_CTL 0x2d
#define BW_RATE 0x2c
#define DATA_FORMAT 0x31
#define DATAX0 0x32

#define Multibyte |(1<<7)
#define Singlebyte & ~(1<<7)
#define Lecture		|(1<<6)
#define Ecriture & ~(1<<6)

typedef struct {
	short int  AxeX;
	short int  AxeY;
	short int  AxeZ;
}My_roulis_struct;   

void roul_init(SPI_TypeDef * SPI) ;
void roul_voiles_relache(MyTimer_Struct_TypeDef timer_voiles);
void roul_lancement(void);
void roul_lecture(My_roulis_struct * ptr);
#endif
