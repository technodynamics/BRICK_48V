/*
 * dac.c
 *
 *  Created on: Mar 27, 2019
 *      Author: neufe
 */


#include "stm32l4xx_hal.h"
#include <system.h>

int32_t dac_out_2;
int32_t dac_out_1;


void dac_enable(void)
{
dac_out_2 = 1000U;
/*Enable RCC Clock*/
((RCC)->APB1ENR1) |= RCC_DAC_ENABLE;
/*Set GPIO PA5 to Analog Mode */
((GPIOA)->MODER) |= ((AN_MODE)<<(GPIO_5_DSHIFT));

/*Connect DAC2 to on chip Peripheral*/
((DAC)->MCR) |= ((MODE_1) <<(CH2_SHIFT));

/*Enable to the DAC*/
((DAC)->CR) |= CH2_ENABLE;


((DAC)->DHR12RD) &= 0U;
((DAC)->DHR12RD) |= ((dac_out_2) << (CH2_SHIFT));
}

void master_mode_enable(void)
{

/*Connect DAC1 to on chip Peripheral*/
((DAC)->MCR) |= ((MODE_1));

/*Enable to the DAC channel 1*/
((DAC)->CR) |= CH1_ENABLE;

dac_out_1 = 7U;

((DAC)->DHR12RD) &= CH1_BM;
((DAC)->DHR12RD) |= ((dac_out_1));
}


void master_pull_down(void)
{

dac_out_1 =7U;

((DAC)->DHR12RD) &= CH1_BM;
((DAC)->DHR12RD) |= ((dac_out_1));

}

void master_reset(void)
{

dac_out_1 = MASTER_BASE_OUT;

((DAC)->DHR12RD) &= CH1_BM;
((DAC)->DHR12RD) |= ((dac_out_1));

}


void master_down(void)
{
if(dac_out_1 < 25U)
{return;}
dac_out_1 -= AF_THERMAL_STEP;

((DAC)->DHR12RD) &= CH1_BM;
((DAC)->DHR12RD) |= ((dac_out_1));

}

void master_up(void)
{
	if(dac_out_1 > 4065U)
	{return;}
	dac_out_1 += AF_THERMAL_STEP;

	((DAC)->DHR12RD) &= CH1_BM;
	((DAC)->DHR12RD) |= ((dac_out_1));
}


void dac_up(uint32_t increment)
{
dac_out_2 += increment;
if(dac_out_2 > 4095U)
{dac_out_2 = 4095U;}

((DAC)->DHR12RD) &= CH2_BM;
((DAC)->DHR12RD) |= ((dac_out_2) << (CH2_SHIFT));
}
void dac_down(uint32_t decrement)
{
	 dac_out_2 -= decrement;
	 if(dac_out_2 <= 0U)
	 {dac_out_2 = 1U;}


	((DAC)->DHR12RD) &= CH2_BM;
	((DAC)->DHR12RD) |= ((dac_out_2) << (CH2_SHIFT));
}

void dac_set(uint32_t set_point)
{
if( set_point < 0U)
{set_point = 0U;}

if(set_point > 4095U)
{set_point = 4095U;}

dac_out_2 = set_point;

((DAC)->DHR12RD) &= CH2_BM;
((DAC)->DHR12RD) |= ((dac_out_2) << (CH2_SHIFT));


}

