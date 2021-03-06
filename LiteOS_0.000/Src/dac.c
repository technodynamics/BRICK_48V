/*
 * dac.c
 *
 *  Created on: Mar 27, 2019
 *      Author: neufe
 */


#include "stm32l4xx_hal.h"
#include <system.h>

int32_t dac_out;


void dac_enable(void)
{
dac_out = 1000U;
/*Enable RCC Clock*/
((RCC)->APB1ENR1) |= RCC_DAC_ENABLE;
/*Set GPIO PA5 to Analog Mode */
((GPIOA)->MODER) |= ((AN_MODE)<<(GPIO_5_DSHIFT));

/*Connect DAC2 to on chip Peripheral*/
((DAC)->MCR) |= ((MODE_1) <<(CH2_SHIFT));

/*Enable to the DAC*/
((DAC)->CR) |= CH2_ENABLE;


((DAC)->DHR12RD) &= 0U;
((DAC)->DHR12RD) |= ((dac_out) << (CH2_SHIFT));

}


void dac_up(uint32_t increment)
{
dac_out += increment;
if(dac_out > 4095U)
{dac_out = 4095U;}

((DAC)->DHR12RD) &= 0U;
((DAC)->DHR12RD) |= ((dac_out) << (CH2_SHIFT));
}
void dac_down(uint32_t decrement)
{
	 dac_out -= decrement;
	 if(dac_out <= 0U)
	 {dac_out = 1U;}


	((DAC)->DHR12RD) &= 0U;
	((DAC)->DHR12RD) |= ((dac_out) << (CH2_SHIFT));
}

void dac_set(uint32_t set_point)
{
if( set_point < 0U)
{set_point = 0U;}

if(set_point > 4095U)
{set_point = 4095U;}

dac_out = set_point;

((DAC)->DHR12RD) &= 0U;
((DAC)->DHR12RD) |= ((dac_out) << (CH2_SHIFT));


}

