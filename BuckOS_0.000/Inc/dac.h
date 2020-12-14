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
#define CH1_ENABLE (1U)
#define MODE_1 1U
#define CH2_SHIFT 16U

#define MASTER_BASE_OUT    2047U
#define AF_THERMAL_STEP    75U
#define CH2_BM             0x00000FFF
#define CH1_BM             0x0FFF0000


void dac_enable(void);
void master_mode_enable(void);
void dac_up(uint32_t increment);
void dac_down(uint32_t decrement);
void master_pull_down(void);
void master_reset(void);
void master_up(void);
void master_down(void);
void dac_set(uint32_t set_point);


#endif /* DAC_H_ */
