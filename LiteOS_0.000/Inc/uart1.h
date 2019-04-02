/*
 * uart1.h
 *
 *  Created on: Jan 26, 2019
 *      Author: neufe
 */



#ifndef UART1_H_
#define UART1_H_


typedef struct{
uint8_t length;
uint8_t string[32U];
}STRING;

#define RCC_USART1_ENABLE ((1U)<<(14U))

#define RXNE ((1U)<<(5U))
#define TXE  ((1U)<<(7U))

#define TE   ((1U)<<(3U))
#define RE   ((1U)<<(2U))

#define UE  1U

#define BR_115 415U

#define TXFRQ ((1U)<<(4U))

#define UART_STARTED 1U
#define TX_GO        2U
#define CMD_EXECUTE  4U

void uart1_enable(void);

void uart1_transmit(STRING* in);

void uart1_buffer_char(void);

STRING* cmd_execute(void);

uint8_t uart1_get_flags(void);



#endif /* UART1_H_ */
