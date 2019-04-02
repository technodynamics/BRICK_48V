/*
 * opamp.c
 *
 *  Created on: Jan 29, 2019
 *      Author: neufe
 */

#include "stm32l4xx_hal.h"
#include <system.h>


void opamp_enable(void)
{
/*Enable Op Amp CLK in RCC*/
((RCC)->APB1ENR1) |= RCC_OPAMP_ENABLE;
/*Enable GPIOA CLK */
((RCC)->AHB2ENR) |= RCC_GPIOA_ENABLE;

/*Set PA0 PA1 PA3 to Analog mode*/

/*Set to High Voltage Range and enable PGA*/
((OPAMP1)->CSR) |= (HV_RANGE|PGA_ENABLE);
/*Set Gain to 16*/
((OPAMP1)->CSR) |= ((PGA_GAIN_2)<<(PGA_SHIFT));
/*Enable the OPAMP*/
((OPAMP1)->CSR) |= OPAMP_ENABLE;

}
