/*
 * dac.h
 *
 *  Created on: Mar 27, 2019
 *      Author: neufe
 */
#ifndef DAC_H_
#define DAC_H_


#define RCC_DAC_ENABLE ((1U)<<(29U))

#define CH2_ENABLE ((1U)<<(16U));
#define MODE_1 1U
#define CH2_SHIFT 16U

void dac_enable(void);
void dac_up(uint32_t increment);
void dac_down(uint32_t decrement);
void dac_set(uint32_t set_point);


#endif /* DAC_H_ */
