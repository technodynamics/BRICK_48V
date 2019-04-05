/*
 * dac.c
 *
 *  Created on: Mar 27, 2019
 *      Author: neufe
 */


#include "stm32l4xx_hal.h"
#include <system.h>

uint32_t dac_out;


void dac_enable(void)
{
dac_out = 1000U;
driveB(4U,1U);
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

driveB(3U,1U);
driveB(4U,0U);

}


void dac_up(uint32_t increment)
{
dac_out += increment;
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
