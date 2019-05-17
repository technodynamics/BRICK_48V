/*
 * tim2.c
 *
 *  Created on: Jan 28, 2019
 *      Author: neufe
 */

#include "stm32l4xx_hal.h"

#include <system.h>

uint32_t clk_freq;
uint32_t arr_val;
uint8_t dc;
uint32_t dc_val;
uint32_t percent;

uint8_t mode;

void tim1_enable(uint32_t freq)
{
clk_freq = 47000U;
arr_val = clk_freq/(freq);
/*Enable TIM1 in RCC*/
((RCC)->APB2ENR) |= RCC_TIM1_ENABLE;


/*Enable GPIOA in RCC*/
((RCC)->AHB2ENR) |= RCC_GPIOA_ENABLE;

/*Configure to AF mode*/
((GPIOA)->MODER) &= (~((IN_MODE)<<(GPIO_8_DSHIFT)));
((GPIOA)->MODER) |= (((AF_MODE)<<(GPIO_8_DSHIFT)));

((GPIOA)->MODER) &= (~((IN_MODE)<<(GPIO_9_DSHIFT)));
((GPIOA)->MODER) |= (((AF_MODE)<<(GPIO_9_DSHIFT)));

((GPIOA)->MODER) &= (~((IN_MODE)<<(GPIO_10_DSHIFT)));
((GPIOA)->MODER) |= (((AF_MODE)<<(GPIO_10_DSHIFT)));

((GPIOA)->MODER) &= (~((IN_MODE)<<(GPIO_11_DSHIFT)));
((GPIOA)->MODER) |= (((AF_MODE)<<(GPIO_11_DSHIFT)));



/*AF1 Mode*/
(((GPIOA)->AFR)[1]) |= ((AF1)<<(GPIO_8_QSHIFT));
(((GPIOA)->AFR)[1]) |= ((AF1)<<(GPIO_9_QSHIFT));
(((GPIOA)->AFR)[1]) |= ((AF1)<<(GPIO_10_QSHIFT));
(((GPIOA)->AFR)[1]) |= ((AF1)<<(GPIO_11_QSHIFT));

((TIM1)->CCER) &= 0U;


/*Configure Channel 3 in PWM Mode 1 and connect to channel pin*/
lockout_mode();

/*Set Frequency*/
((TIM1)->ARR) = arr_val;
percent = ((arr_val/100U)+1U);
dc = 0;

/*Connect outputs*/
((TIM1)->CCER) |= ((1U)<<(CC1_SHIFT));
((TIM1)->CCER) |= ((1U)<<(CC2_SHIFT));
((TIM1)->CCER) |= ((1U)<<(CC3_SHIFT));
((TIM1)->CCER) |= ((1U)<<(CC4_SHIFT));

/*Invert Buck Signals*/
((TIM1)->CCER) |= ((1U)<<(CC3_SHIFT))<<(1U);
((TIM1)->CCER) |= ((1U)<<(CC4_SHIFT))<<(1U);

/*Enable Outputs*/
((TIM1)->BDTR) |= MOE;


/*Set for Center Aligned Mode 3*/
//((TIM1)->CR1) |= CEN_ALN_3;

/*Start the Timer*/
((TIM1)->CR1) |= TIM1_ENABLE;
}

void buck_mode(void)
{
mode = BUCK_MODE;
if(dc < 2U)
{dc = 2U;}

        dc_val = percent*dc;


        ((TIM1)->CCR4) &= 0;
        ((TIM1)->CCR4) |= dc_val;

        ((TIM1)->CCR3) &= 0;
        ((TIM1)->CCR3) |= (dc_val -  (percent));

	    ((TIM1)->CCMR1) &= 0U;
		((TIM1)->CCMR1) |= ((LOW_MODE)<<(BOOST_HI_SHIFT));
		((TIM1)->CCMR1) |= ((LOW_MODE)<<(BOOST_LO_SHIFT));

		((TIM1)->CCMR2) &= 0U;
		((TIM1)->CCMR2) |= ((PWM_MODE_1)<<(BUCK_HI_SHIFT));
		((TIM1)->CCMR2) |= ((INV_LOW_MODE)<<(BUCK_LO_SHIFT));
}

void boost_mode(void)
{

mode = BOOST_MODE;

if(dc < 2U)
{dc = 2U;}

			dc_val = percent*dc;

	        ((TIM1)->CCR1) &= 0;
	        ((TIM1)->CCR1) |= (dc_val - (percent));

	        ((TIM1)->CCR2) &= 0;
	        ((TIM1)->CCR2) |= (dc_val);

		    ((TIM1)->CCMR1) &= 0U;
			((TIM1)->CCMR1) |= ((HIGH_MODE)<<(BOOST_HI_SHIFT));
			((TIM1)->CCMR1) |= ((PWM_MODE_1)<<(BOOST_LO_SHIFT));

			((TIM1)->CCMR2) &= 0U;
			((TIM1)->CCMR2) |= ((INV_LOW_MODE)<<(BUCK_HI_SHIFT));
			((TIM1)->CCMR2) |= ((INV_LOW_MODE)<<(BUCK_LO_SHIFT));
}
void lockout_mode(void)
{
	mode = LOCKOUT_MODE;
	((TIM1)->CCMR1) &= 0U;
	((TIM1)->CCMR1) |= ((HIGH_MODE)<<(BOOST_HI_SHIFT));
	((TIM1)->CCMR1) |= ((LOW_MODE)<<(BOOST_LO_SHIFT));

	((TIM1)->CCMR2) &= 0U;
	((TIM1)->CCMR2) |= ((INV_HIGH_MODE)<<(BUCK_HI_SHIFT));
	((TIM1)->CCMR2) |= ((INV_LOW_MODE)<<(BUCK_LO_SHIFT));

}
void passthru_mode(void)
{
	mode = PASSTHRU_MODE;
	((TIM1)->CCMR1) &= 0U;
	((TIM1)->CCMR1) |= ((LOW_MODE)<<(BOOST_HI_SHIFT));
	((TIM1)->CCMR1) |= ((LOW_MODE)<<(BOOST_LO_SHIFT));

	((TIM1)->CCMR2) &= 0U;
	((TIM1)->CCMR2) |= ((INV_LOW_MODE)<<(BUCK_HI_SHIFT));
	((TIM1)->CCMR2) |= ((INV_LOW_MODE)<<(BUCK_LO_SHIFT));
}

uint8_t mode_check(void)
{return mode;}

uint8_t dc_check(void)
{return dc;}



void duty_cycle_increment(void)
{
dc+=1U;
if(dc > 99U)
{dc = 99U;}
dc_val = percent*dc;

if(mode == BOOST_MODE)
{
((TIM1)->CCR1) &= 0;
((TIM1)->CCR1) |= (dc_val - (percent));

((TIM1)->CCR2) &= 0;
((TIM1)->CCR2) |= (dc_val);
}

if(mode == BUCK_MODE)
{
((TIM1)->CCR3) &= 0;
((TIM1)->CCR3) |= (dc_val - (percent));

((TIM1)->CCR4) &= 0;
((TIM1)->CCR4) |= (dc_val);
}

}


void duty_cycle_decrement(void)
{
	dc-= 1U;
	if(dc < 2U)
	{dc = 2U;}
	dc_val = percent*dc;

	if(mode == BOOST_MODE)
	{
	((TIM1)->CCR1) &= 0;
	((TIM1)->CCR1) |= (dc_val - (percent));

	((TIM1)->CCR2) &= 0;
	((TIM1)->CCR2) |= (dc_val);
	}

	if(mode == BUCK_MODE)
	{
	((TIM1)->CCR3) &= 0;
	((TIM1)->CCR3) |= (dc_val - (percent));

	((TIM1)->CCR4) &= 0;
	((TIM1)->CCR4) |= (dc_val);
	}

}


void set_duty_cycle(uint8_t dcn)
{
if(dcn < 2U)
{return;}
if(dcn > 99U)
{return;}

dc = dcn;

if(mode == BOOST_MODE)
{
	dc_val = percent*dc;
    ((TIM1)->CCR1) &= 0;
    ((TIM1)->CCR1) |= (dc_val - (percent));

    ((TIM1)->CCR2) &= 0;
    ((TIM1)->CCR2) |= (dc_val);
}
if(mode == BUCK_MODE)
{
	dc_val = percent*dc;
    ((TIM1)->CCR4) &= 0;
    ((TIM1)->CCR4) |= dc_val;

    ((TIM1)->CCR3) &= 0;
    ((TIM1)->CCR3) |= (dc_val - (percent));
}


}




