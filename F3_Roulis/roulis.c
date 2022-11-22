#include "stm32f10x.h"
#include "roulis.h"
#include "driver_gpio.h" 
#include "driver_timer.h"
#include "MySPI.h"
#include "m_voiles.h"

void roul_init(SPI_TypeDef * SPI) 
{
	MySPI_Init(SPI);		
}

void roul_voiles_relache(MyTimer_Struct_TypeDef timer_voiles)
{
		timer_pwm_changecycle(timer_voiles.Timer, DUTYCYCLE_0, 2);
}

void roul_lancement() //MyTimer_Struct_TypeDef timer_voiles
{
		MySPI_Clear_NSS();
		MySPI_Send(DATA_FORMAT);
		MySPI_Send(0x20);
		MySPI_Set_NSS();
	
		MySPI_Clear_NSS();
		MySPI_Send(POWER_CTL);
		MySPI_Send(0x8);
		MySPI_Set_NSS();
	
		MySPI_Clear_NSS();
		MySPI_Send(BW_RATE);
		MySPI_Send(0x9);
		MySPI_Set_NSS();
		
}

void roul_lecture(My_roulis_struct * ptr){
		int X0;
		int X1;
		int Y0;
		int Y1;
		int Z0;
		int Z1;
	  char Mssg;
	
	 Mssg=DATAX0 |(0x1<<6) | (1<<7);
	
		MySPI_Clear_NSS();
		MySPI_Send(Mssg); 
	
		X0 = MySPI_Read();
		X1 = MySPI_Read();
		Y0 = MySPI_Read();
		Y1 = MySPI_Read();
		Z0 = MySPI_Read();
		Z1 = MySPI_Read();
	
		MySPI_Set_NSS();
	
		ptr->AxeX = ptr->AxeX &~(255);
		ptr->AxeX = ptr->AxeX &~(255<<8);
		ptr->AxeY = ptr->AxeY &~(255);
		ptr->AxeY = ptr->AxeY &~(255<<8);
		ptr->AxeZ = ptr->AxeZ &~(255);
		ptr->AxeZ = ptr->AxeZ &~(255<<8);
	
		ptr->AxeX = ptr->AxeX | X0 | (X1<<8);
		ptr->AxeY = ptr->AxeY | Y0 | (Y1<<8);
		ptr->AxeZ = ptr->AxeZ | Z0 | (Z1<<8);
		
}

