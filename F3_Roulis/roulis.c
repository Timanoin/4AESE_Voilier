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

void roul_voiles_relache(MyTimer_Struct_TypeDef timer_voiles, int z)
{
	if (z > 40){
		timer_pwm_changecycle(timer_voiles.Timer, DUTYCYCLE_0, 2);
	}
}

void roul_lancement() //MyTimer_Struct_TypeDef timer_voiles
{
		MySPI_Clear_NSS();
		MySPI_Send(DATA_FORMAT Ecriture Multibyte);
		MySPI_Set_NSS();
	
		MySPI_Clear_NSS();
		MySPI_Send(POWER_CTL Ecriture Multibyte );
		MySPI_Set_NSS();
	
		MySPI_Clear_NSS();
		MySPI_Send(BW_RATE Ecriture Multibyte);
		MySPI_Set_NSS();
		
}

void roul_lecture(My_roulis_struct ptr){
			
		MySPI_Clear_NSS();
		MySPI_Send(DATAX0 Lecture Multibyte);
		ptr.AxeX1 = MySPI_Read();
		ptr.AxeX2 = MySPI_Read();
		
		ptr.AxeY1 = MySPI_Read();
		ptr.AxeY2 = MySPI_Read();
	
		ptr.AxeZ1 = MySPI_Read();
		ptr.AxeZ2 = MySPI_Read();
	
		MySPI_Set_NSS();
		
}

