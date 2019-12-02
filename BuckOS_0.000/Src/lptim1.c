/*
 * lptim1.c
 *
 *  Created on: Feb 3, 2019
 *      Author: neufe
 */

#include "stm32l4xx_hal.h"

#include <system.h>


TIME sys_time;

void lptim1_enable(void)
{
uint8_t i =0U;


((RCC)->APB1ENR1) |= RCC_LPTIM1_ENABLE;
((LPTIM1)->CR) |= LPTIM1_ENABLE;


((LPTIM1)->IER) |= ARRM;

((LPTIM1)->ARR) = QTR_MEG_OFF;

NVIC_ClearPendingIRQ(LPTIM1_IRQn);
NVIC_SetPriority(LPTIM1_IRQn, 2U);
NVIC_EnableIRQ(LPTIM1_IRQn);

for(i=0U; i<5U; i++)
{((&sys_time)->time_nums)[i] = 0U;}


((LPTIM1)->CR) |= CNT_START;
}

void time_increment(void)
{
(((&sys_time)->time_nums)[ticks])++;

if((((&sys_time)->time_nums)[ticks]) >= ticks_ovf)
{
(((&sys_time)->time_nums)[millis])++;
(((&sys_time)->time_nums)[ticks]) = 0U;
}

if((((&sys_time)->time_nums)[millis]) >= millis_ovf)
{
(((&sys_time)->time_nums)[seconds])++;
(((&sys_time)->time_nums)[millis]) = 0U;
}

if((((&sys_time)->time_nums)[seconds]) >= seconds_ovf)
{
(((&sys_time)->time_nums)[minutes])++;
(((&sys_time)->time_nums)[seconds]) = 0U;
}

if((((&sys_time)->time_nums)[minutes]) >= minutes_ovf)
{
(((&sys_time)->time_nums)[hours])++;
(((&sys_time)->time_nums)[minutes]) = 0U;
}

if((((&sys_time)->time_nums)[hours]) >= hours_ovf)
{
(((&sys_time)->time_nums)[days])++;
(((&sys_time)->time_nums)[hours]) = 0U;
}


}

TIME* read_time(void)
{return &sys_time;}

