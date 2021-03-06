/*
 * TIM1.h
 *
 *  Created on: Jan 28, 2019
 *      Author: neufe
 */

#ifndef TIM1_H_
#define TIM1_H_

#define RCC_TIM1_ENABLE ((1U)<<(11U))

#define CLK_FREQ    47000U

#define TIM1_ENABLE 1U
#define UG          1U
#define UIE			1U


#define ETRF ((7U)<<(4U))
#define OCCS ((1U)<<(3U))

#define AWD1_SELECT 1U

#define LOCKOUT_MODE 1U
#define PASSTHRU_MODE 2U
#define BOOST_MODE 3U
#define BUCK_MODE  4U

#define CC1_SHIFT 0U
#define CC2_SHIFT 4U
#define CC3_SHIFT 8U
#define CC4_SHIFT 12U

#define CC1PE  3U
#define CC2PE  11U
#define CC3PE  3U
#define CC4PE  11U


#define OCM1_SHIFT 4U
#define OCM2_SHIFT 12U
#define OCM3_SHIFT 4U
#define OCM4_SHIFT 12U

#define BOOST_PWM_SHIFT OCM1_SHIFT
#define BOOST_EN_SHIFT  OCM2_SHIFT
#define BUCK_PWM_SHIFT  OCM3_SHIFT
#define BUCK_EN_SHIFT   OCM4_SHIFT


#define MOE  ((1U)<<(15U))
#define CEN_ALN_3 ((3U)<<(5U))





#define LOW_MODE   0x04
#define HIGH_MODE  0x05

#define INV_LOW_MODE HIGH_MODE
#define INV_HIGH_MODE LOW_MODE
#define PWM_MODE_1 0x06
#define PWM_MODE_2 0x07



#define OCM1_3_SHIFT 16U
#define OCM2_3_SHIFT 24U
#define OCM3_3_SHIFT 16U
#define OCM4_3_SHIFT 24U


#define OCXCE_SHIFT 7U
#define OCYCE_SHIFT 15U



#define SW_DELAY 5U

void tim1_enable(uint32_t freq);

void buck_mode(void);
void boost_mode(void);
void lockout_mode(void);
void passthru_mode(void);

uint8_t mode_check(void);
uint8_t dc_check(void);

void duty_cycle_increment(uint32_t step);
void duty_cycle_decrement(uint32_t step);

void set_duty_cycle(uint8_t dcn);
void shift_action(void);

void pwm_lock(void);
void pwm_unlock(void);


#endif /* TIM2_H_ */
