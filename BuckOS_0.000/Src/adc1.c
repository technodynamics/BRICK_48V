/*
 * adc.c
 *
 *  Created on: Jan 28, 2019
 *      Author: neufe
 */

#include "stm32l4xx_hal.h"
#include "stm32l4xx_it.h"

#include <system.h>

uint32_t adci;

void adc1_enable(void)
{

/*Enable ADC1 CLK in RCC*/
((RCC)->AHB2ENR) |= RCC_ADC1_ENABLE;

/*Enable the GPIOA CLK in RCC*/
((RCC)->AHB2ENR) |= (RCC_GPIOA_ENABLE|RCC_GPIOB_ENABLE);

((GPIOB)->MODER) |= ((AN_MODE) << (GPIO_0_DSHIFT));
((GPIOB)->MODER) |= ((AN_MODE) << (GPIO_1_DSHIFT));

((GPIOA)->MODER) |= ((AN_MODE) << (GPIO_6_DSHIFT));
((GPIOA)->MODER) |= ((AN_MODE) << (GPIO_2_DSHIFT));
((GPIOA)->MODER) |= ((AN_MODE) << (GPIO_1_DSHIFT));


/*Disable Deep Power Down*/
while(((ADC1)->CR) & DPD)
{((ADC1)->CR) &= ~(DPD);}

/*Enable the internal Temperature Sensor*/
//((ADC1_COMMON)->CCR) |= (TEMP_CHANNEL_ENABLE);

/*Adjust the CLK*/
((ADC1_COMMON)->CCR) |= ((HCLK_1)<<(ADC_CLK_SHIFT));



/*Enable the VoltageRegulator*/
while((((ADC1)->CR) & ADC_REG_GEN) == 0U)
{((ADC1)->CR) |= ADC_REG_GEN;}
/*Prep for calibration*/

while(((ADC1)->CR) & ((ADC_JSTP)|(ADC_STP)|(ADC_JSTART)|(ADC_START)|(ADC_DIS)|(ADC_EN)))
{((ADC1)->CR) |= ((ADC_JSTP)|(ADC_STP));}
adci=0U;
while(adci < 100U)
{adci++;}
adci=0U;

/*Calibrate*/
((ADC1)->CR) |= ADC_CALIBRATE;
while(((ADC1)->CR) & ADC_CALIBRATE){;}

while(adci < 100U)
{adci++;}
adci=0U;
/*Enable the ADC*/
((ADC1)->CR) |= ADC_EN;
while((((ADC1)->ISR) & ADC_READY) == 0U){;}



/*Clear Flag*/
((ADC1)->ISR) |= (ADC_READY);
/*Enable Interrupts*/
((ADC1)->IER) |= ((EOSI)|(EOCI));


/*Set Channels*/
((ADC1)->SQR1) &= 0U;
((ADC1)->SQR1) |= ((CURRENT_SENSE_CHANNEL)<<(SQ1_SHIFT));
((ADC1)->SQR1) |= ((OUTPUT_VOLTAGE_CHANNEL)<<(SQ3_SHIFT));
((ADC1)->SQR2) |= ((INPUT_VOLTAGE_CHANNEL));
((ADC1)->SQR1) |= REG_CONV_CNT;

((ADC1)->JSQR) &= 0U;
((ADC1)->JSQR) |= ((INT_TEMP_CHANNEL)<<(JSQ2_SHIFT));
((ADC1)->JSQR) |= ((EXT_TEMP_CHANNEL)<<(JSQ1_SHIFT));
((ADC1)->JSQR) |= (INJ_CONV_CNT);



/*Set Sampling Time on all channels*/
((ADC1)->SMPR1) &= 0U;
((ADC1)->SMPR2) &= 0U;
((ADC1)->SMPR1) |= (SMPR1_CONF << 1U);
((ADC1)->SMPR2) |= (SMPR2_CONF << 1U);

/*Configure the Setup*/
((ADC1)->CFGR) |= (AUTO_DELAY);
//((ADC1)->CFGR) &= ~(JQDIS);


NVIC_ClearPendingIRQ(ADC1_IRQn);
NVIC_SetPriority(ADC1_IRQn, 0U);
NVIC_EnableIRQ(ADC1_IRQn);


}


void adc1_regular_conversions(void)
{
((ADC1)->ISR) |= ((EOSI)|(EOCI));
((ADC1)->IER) |= ((EOSI)|(EOCI));
((ADC1)->CR) |= ADC_START;

}

void adc1_inject_conversions(void)
{

	((ADC1)->ISR) |= ((JEOSI));
	((ADC1)->IER) |= ((JEOSI));
	((ADC1)->CR) |= ADC_JSTART;
}


