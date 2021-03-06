/*
 * adc1.h
 *
 *  Created on: Jan 28, 2019
 *      Author: neufe
 */

#ifndef ADC1_H_
#define ADC1_H_

#define RCC_ADC1_ENABLE ((1U)<<(13U))
#define ADC1_FLAG_CLEAR 0x000007FF

#define ADC_CALIBRATE ((1U)<<(31U))
#define ADC_CAL_DIF   ((1U)<<(30U))
#define DPD           ((1U)<<(29U))
#define ADC_REG_GEN   ((1U)<<(28U))

#define ADC_JSTP      ((1U)<<(5U))
#define ADC_STP       ((1U)<<(4U))
#define ADC_JSTART    ((1U)<<(3U))
#define ADC_START     ((1U)<<(2U))
#define ADC_DIS       ((1U)<<(1U))
#define ADC_EN       (1U)

#define HCLK_4         (3U)
#define HCLK_1         (1U)
#define ADC_CLK_SHIFT  (16U)

#define TEMP_CHANNEL_ENABLE ((1U)<<(23U))

#define JEOSI           ((1U)<<(6U))
#define JEOCI           ((1U)<<(5U))
#define EOSI            ((1U)<<(3U))
#define EOCI            ((1U)<<(2U))
#define EOSMPI          ((1U)<<(1U))
#define ADC_READY       ((1U))

#define CURRENT_SENSE_CHANNEL  15U
#define OUTPUT_VOLTAGE_CHANNEL 16U
#define INPUT_VOLTAGE_CHANNEL  7U

#define HIPW_SIG_CHANNEL 8U
#define INTL_SIG_CHANNEL 9U
#define INT_TEMP_CHANNEL 6U
#define EXT_TEMP_CHANNEL 11U

#define SMPR1_CONF    0x09249249
#define SMPR2_CONF    0x01249249

#define REG_CONT     ((1U)<<(13U))
#define REG_DISCONT  ((1U)<<(16U))
#define AUTO_DELAY   ((1U)<<(14U))
#define OVER_WRITE   ((1U)<<(12U))
#define JQDIS        ((1U)<<(31U))

#define DISCNUM_SHIFT (17U)


#define SQ4_SHIFT 24U
#define SQ3_SHIFT 18U
#define SQ2_SHIFT 12U
#define SQ1_SHIFT 6U

#define REG_CONV_CNT 5U


#define JSQ4_SHIFT 26U
#define JSQ3_SHIFT 20U
#define JSQ2_SHIFT 14U
#define JSQ1_SHIFT 8U

#define INJ_CONV_CNT 3U


void adc1_enable(void);
void adc1_regular_conversions(void);
void adc1_inject_conversions(void);

#endif /* ADC1_H_ */
