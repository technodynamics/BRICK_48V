/*
 * lptim1.h
 *
 *  Created on: Feb 3, 2019
 *      Author: neufe
 */

#ifndef LPTIM1_H_
#define LPTIM1_H_

typedef struct{
	uint32_t time_nums[6U];
}TIME;

#define RCC_LPTIM1_ENABLE ((1U)<<(31U))

#define ARRM          ((1U)<<(1U))
#define LPTIM1_ENABLE (1U)
#define CNT_START     ((1U)<<(2U))
#define LPTIM1_FLAG_CLEAR 0x000001FF

#define ONE_MEG_OFF 48U
#define QTR_MEG_OFF 320U


#define ticks   0U
#define millis  1U
#define seconds 2U
#define minutes 3U
#define hours   4U
#define days    5U

#define ticks_ovf   250U
#define millis_ovf  1000U
#define seconds_ovf 60U
#define minutes_ovf 60U
#define hours_ovf   24U


void lptim1_enable(void);
void time_increment(void);
TIME* read_time(void);

#endif /* LPTIM1_H_ */
