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
uint32_t shifting_stage;
uint8_t up_down;

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

//((TIM1)->SMCR) |= (OCCS)|ETRF;
//((TIM1)->OR1) |= AWD1_SELECT;

/*Configure Channel 3 in PWM Mode 1 and connect to channel pin*/
lockout_mode();

/*Set Frequency*/
((TIM1)->ARR) = arr_val;
percent = ((arr_val/100U)+1U);
dc = 0;
shifting_stage = 0U;
up_down = 0U;
/*Connect outputs*/
((TIM1)->CCER) |= ((1U)<<(CC1_SHIFT));
((TIM1)->CCER) |= ((1U)<<(CC2_SHIFT));
((TIM1)->CCER) |= ((1U)<<(CC3_SHIFT));
((TIM1)->CCER) |= ((1U)<<(CC4_SHIFT));

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

        ((TIM1)->CCR3) = dc_val;

	    ((TIM1)->CCMR1) &= 0U;
	    ((TIM1)->CCMR1) |= (((1U)<<(CC1PE))|((1U)<<(CC2PE)));
		((TIM1)->CCMR1) |= ((LOW_MODE)<<(BOOST_PWM_SHIFT));
		((TIM1)->CCMR1) |= ((LOW_MODE)<<(BOOST_EN_SHIFT));

		((TIM1)->CCMR2) &= 0U;
		((TIM1)->CCMR2) |= (((1U)<<(CC3PE))|((1U)<<(CC4PE)));
		((TIM1)->CCMR2) |= ((PWM_MODE_1)<<(BUCK_PWM_SHIFT));
		((TIM1)->CCMR2) |= ((HIGH_MODE)<<(BUCK_EN_SHIFT));



}

void boost_mode(void)
{

		mode = BOOST_MODE;

		((TIM1)->CCR1) = dc_val;

		((TIM1)->CCMR1) &= 0U;
		((TIM1)->CCMR1) |= (((1U)<<(CC1PE))|((1U)<<(CC2PE))|((1U)<<(OCXCE_SHIFT)));
		((TIM1)->CCMR1) |= ((HIGH_MODE)<<(BOOST_EN_SHIFT));
		((TIM1)->CCMR1) |= ((PWM_MODE_2)<<(BOOST_PWM_SHIFT));


		((TIM1)->CCMR2) &= 0U;
		((TIM1)->CCMR2) |= (((1U)<<(CC3PE))|((1U)<<(CC4PE)));
		((TIM1)->CCMR2) |= ((LOW_MODE)<<(BUCK_EN_SHIFT));
		((TIM1)->CCMR2) |= ((LOW_MODE)<<(BUCK_PWM_SHIFT));
}
void lockout_mode(void)
{
	mode = LOCKOUT_MODE;
	((TIM1)->CCMR1) &= 0U;
    ((TIM1)->CCMR1) |= (((1U)<<(CC1PE))|((1U)<<(CC2PE)));
	((TIM1)->CCMR1) |= ((LOW_MODE)<<(BOOST_EN_SHIFT));
	((TIM1)->CCMR1) |= ((LOW_MODE)<<(BOOST_PWM_SHIFT));

	((TIM1)->CCMR2) &= 0U;
    ((TIM1)->CCMR2) |= (((1U)<<(CC3PE))|((1U)<<(CC4PE)));
	((TIM1)->CCMR2) |= ((LOW_MODE)<<(BUCK_EN_SHIFT));
	((TIM1)->CCMR2) |= ((LOW_MODE)<<(BUCK_PWM_SHIFT));

}
void passthru_mode(void)
{
	mode = PASSTHRU_MODE;
	((TIM1)->CCMR1) &= 0U;
    ((TIM1)->CCMR1) |= (((1U)<<(CC1PE))|((1U)<<(CC2PE)));
	((TIM1)->CCMR1) |= ((HIGH_MODE)<<(BOOST_PWM_SHIFT));
    ((TIM1)->CCMR1) |= ((HIGH_MODE)<<(BOOST_EN_SHIFT));

	((TIM1)->CCMR2) &= 0U;
    ((TIM1)->CCMR2) |= (((1U)<<(CC3PE))|((1U)<<(CC4PE)));
	((TIM1)->CCMR2) |= ((LOW_MODE)<<(BUCK_PWM_SHIFT));
    ((TIM1)->CCMR2) |= ((HIGH_MODE)<<(BUCK_EN_SHIFT));
}

void pwm_lock(void)
{((TIM1)->CCMR2) |= ((LOW_MODE)<<(BUCK_EN_SHIFT));}

void pwm_unlock(void)
{((TIM1)->CCMR2) |= ((HIGH_MODE)<<(BUCK_EN_SHIFT));}



uint8_t mode_check(void)
{return mode;}

uint8_t dc_check(void)
{return (dc);}

void shift_action(void)
{
if(up_down == 1U)
{
if((shifting_stage % 2U) == 0U)
{duty_cycle_increment(1U);}
else
{duty_cycle_decrement(1U);}
}

if(up_down ==2U)
{
	if((shifting_stage % 2U) == 0U)
	{duty_cycle_decrement(1U);}
	else
	{duty_cycle_increment(1U);}
}

}


void duty_cycle_increment(uint32_t step)
{


dc_val+=step;
if(dc_val >= (arr_val - 1U))
{dc_val = (arr_val - 1U);}
dc = dc_val/percent;

if(mode == BOOST_MODE)
{((TIM1)->CCR1) = (dc_val);}

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

	if(mode == BOOST_MODE)
	{((TIM1)->CCR1) = ((dc_val));}

	if(mode == BUCK_MODE)
	{((TIM1)->CCR3) = (dc_val);}


	/*switch(shifting_stage)
	{
	case 0U: shifting_stage += 1U; ((TIM1)->DIER) |=  UIE; up_down = 2U; break;
	case 1U: shifting_stage += 1U; break;
	case 2U: shifting_stage += 1U; break;
	case 3U: shifting_stage += 1U; break;
	case 4U: shifting_stage += 1U; break;
	case 5U: shifting_stage += 1U; break;
	case 6U: shifting_stage += 1U; break;
	case 7U: shifting_stage += 1U; break;
	case 8U: shifting_stage += 1U; break;
	case 9U: shifting_stage += 1U; break;
	case 10U: shifting_stage = 0U;((TIM1)->DIER) &= ~(UIE); break;
	}*/
}


void set_duty_cycle(uint8_t dcn)
{
if(dcn == 0U)
{return;}
if(dcn > 99U)
{return;}

	dc = dcn;
	dc_val = percent*dc;
	((TIM1)->CCR1) = ((dc_val));
    ((TIM1)->CCR3) = (dc_val);
    ((TIM1)->EGR) |= UG;
}




