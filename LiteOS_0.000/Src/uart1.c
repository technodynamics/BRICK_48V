/*
 * uart1.c
 *
 *  Created on: Jan 26, 2019
 *      Author: neufe
 */
#include "stm32l4xx_hal.h"
#include <system.h>


STRING* output_string;
STRING* last_attempt;
STRING current_input;
STRING last_command;

uint32_t attempt_count;
uint8_t uart_flags;
uint8_t outi;
uint8_t ui;

uint8_t debug;

void uart1_enable(void)
{
/*ENABLE THE USART RCC*/
((RCC)->APB2ENR) |= RCC_USART1_ENABLE;

/*ENABLE GPIOA RCC*/
((RCC)->AHB2ENR) |= RCC_GPIOB_ENABLE;

/*Set the PA9 PA10 to AF_MODE*/
((GPIOB)->MODER) &= (~((IN_MODE)<<(GPIO_6_DSHIFT)));
((GPIOB)->MODER) |= ((AF_MODE)<<(GPIO_6_DSHIFT));

((GPIOB)->MODER) &= (~((IN_MODE)<<(GPIO_7_DSHIFT)));
((GPIOB)->MODER) |= ((AF_MODE)<<(GPIO_7_DSHIFT));

(((GPIOB)->AFR)[0]) |= ((AF7)<<(GPIO_6_QSHIFT));
(((GPIOB)->AFR)[0]) |= ((AF7)<<(GPIO_7_QSHIFT));



/*Enable Receiver Transmitter and Receiver Not Empty Interrupt*/
((USART1)->CR1) |= ((RXNE)|(RE)|TE);
/*Set Baud Rate to 115200*/
((USART1)->BRR) &= 0U;
((USART1)->BRR) |= BR_115;

/*Enable Interrupts in NVIC*/
NVIC_ClearPendingIRQ(USART1_IRQn);
NVIC_SetPriority(USART1_IRQn, 1U);
NVIC_EnableIRQ(USART1_IRQn);

/*Enable UART*/
((USART1)->CR1) |= UE;

((USART1)->TDR) = 17U;

outi = 0U;
debug = 0U;
uart_flags |= ((UART_STARTED)|(CMD_EXECUTE));


}


void uart1_transmit(STRING* in)
{

	for(outi=0U; outi<((in)->length); outi++)
	{
	((USART1)->TDR) = (((in)->string)[outi]);
	while((((USART1)->ISR) & TXE) == 0U)
	{;}
	}
}





void uart1_buffer_char(void)
{
uint8_t char_in = ((USART1)->RDR);

if((char_in > 31U) && (char_in < 127U))
{
((&current_input)->string)[((&current_input)->length)] = char_in;
((&current_input)->length)++;
((USART1)->TDR) = char_in;
}


if(char_in == 13U)
{
for(ui=0U; ui<((&current_input)->length); ui++)
{(((&last_command)->string)[ui]) = (((&current_input)->string)[ui]);}

((&last_command)->length) = ((&current_input)->length);
((&current_input)->length) = 0U;

uart_flags |= (CMD_EXECUTE);
}


((USART1)->ICR) |= (RXNE);

}


STRING* cmd_execute(void)
{
uart_flags &= (~(CMD_EXECUTE));
return (&last_command);
}


uint8_t uart1_get_flags(void)
{return uart_flags;}
