/*
 * tim1.c
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
uint32_t tim1i;


uint8_t mode;

void tim1_enable(uint32_t freq)
{
clk_freq = 80000U;
arr_val = clk_freq/(freq);
/*Enable TIM1 in RCC*/
((RCC)->APB2ENR) |= RCC_TIM1_ENABLE;


/*Enable GPIOA in RCC*/
((RCC)->AHB2ENR) |= RCC_GPIOA_ENABLE;

/*Configure to AF mode*/
((GPIOA)->MODER) &= (~((IN_MODE)<<(GPIO_10_DSHIFT)));
((GPIOA)->MODER) |= (((AF_MODE)<<(GPIO_10_DSHIFT)));

((GPIOA)->MODER) &= (~((IN_MODE)<<(GPIO_11_DSHIFT)));
((GPIOA)->MODER) |= (((OUT_MODE)<<(GPIO_11_DSHIFT)));


/*AF1 Mode*/

(((GPIOA)->AFR)[1]) |= ((AF1)<<(GPIO_10_QSHIFT));


((TIM1)->CCER) &= 0U;

//((TIM1)->SMCR) |= (OCCS)|ETRF;
//((TIM1)->OR1) |= AWD1_SELECT;

/*Configure Channel 3 in PWM Mode 1 and connect to channel pin*/
lockout_mode();

/*Set Frequency*/
((TIM1)->ARR) = arr_val;
percent = ((arr_val/100U)+1U);
dc = 0;


/*Connect outputs*/
((TIM1)->CCER) |= ((1U)<<(CC3_SHIFT));

/*Invert Buck Signals*/
//((TIM1)->CCER) |= ((1U)<<(CC3_SHIFT))<<(1U);
//((TIM1)->CCER) |= ((1U)<<(CC4_SHIFT))<<(1U);
//((TIM1)->CCER) |= ((1U)<<(CC1_SHIFT))<<(1U);
//((TIM1)->CCER) |= ((1U)<<(CC2_SHIFT))<<(1U);




/*Enable the Pre-Load for CCR registers*/
((TIM1)->CCMR1) |= (((1U)<<(CC1PE))|((1U)<<(CC2PE)));
((TIM1)->CCMR2) |= (((1U)<<(CC3PE))|((1U)<<(CC4PE)));



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
        pwm_unlock();

        ((TIM1)->CCR3) = dc_val;

		((TIM1)->CCMR2) &= 0U;
		((TIM1)->CCMR2) |= (((1U)<<(CC3PE))|((1U)<<(CC4PE)));
		((TIM1)->CCMR2) |= ((PWM_MODE_1)<<(BUCK_PWM_SHIFT));
}

void lockout_mode(void)
{

	((TIM1)->CCMR2) &= 0U;
	((TIM1)->CCMR2) |= (((LOW_MODE)<<(BUCK_PWM_SHIFT)));
    ((TIM1)->EGR) |= UG;

    tim1i=0U;
    while(tim1i<10)
    {tim1i+=1U;}

    pwm_unlock();
	set_duty_cycle(0U);
	mode = LOCKOUT_MODE;
}


void pwm_lock(void)
{((GPIOA)->ODR)  &= ~(((1U)<<(GPIO_11_SHIFT)));}

void pwm_unlock(void)
{((GPIOA)->ODR)  |= ((1U)<<(GPIO_11_SHIFT));}



uint8_t mode_check(void)
{return mode;}

uint8_t dc_check(void)
{return (dc);}

void duty_cycle_increment(uint32_t step)
{
dc_val+=step;
if(dc_val >= (arr_val - 10U))
{dc_val = (arr_val - 10U);}
dc = dc_val/percent;

if(mode == BUCK_MODE)
{((TIM1)->CCR3) = (dc_val);}

}


void duty_cycle_decrement(uint32_t step)
{
    if(step >= dc_val)
    {dc_val = 0U;}
    else
    {dc_val -= step;}

    dc = dc_val/percent;

	if(mode == BUCK_MODE)
	{((TIM1)->CCR3) = (dc_val);}

}


void set_duty_cycle(uint8_t dcn)
{
if(dcn < 0U)
{return;}
if(dcn > 99U)
{return;}

	dc = dcn;
	dc_val = percent*dc;
	((TIM1)->CCR1) = ((dc_val));
    ((TIM1)->CCR3) = (dc_val);
    ((TIM1)->EGR) |= UG;
}




