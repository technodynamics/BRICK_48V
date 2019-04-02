/*
 * opamp.h
 *
 *  Created on: Jan 29, 2019
 *      Author: neufe
 */

#ifndef OPAMP_H_
#define OPAMP_H_

#define RCC_OPAMP_ENABLE ((1U)<<(30U))

#define HV_RANGE ((1U)<<(31U))

#define PGA_SHIFT 4U

#define PGA_GAIN_2 0U
#define PGA_GAIN_4 1U
#define PGA_GAIN_8 2U
#define PGA_GAIN_16 3U

#define PGA_ENABLE ((1U)<<(3U))

#define OPAMP_ENABLE 1U

void opamp_enable(void);

#endif /* OPAMP_H_ */
