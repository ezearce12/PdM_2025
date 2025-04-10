/*
 * API_uart.h
 *
 *  Created on: Apr 3, 2025
 *      Author: Eze Arce
 */

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>

//Se definen tamaños maximos para los buiffer de TX y RX


bool uartInit();
void uartSendString(uint8_t * pstring);
void uartSendStringSize(uint8_t * pstring, uint16_t size);
void uartReceiveStringSize(uint8_t * pstring, uint16_t size);
void uartClearTerminal(void);
//void uartReceiveLine(uint8_t *pstring, uint16_t max_size);

#endif /* API_INC_API_UART_H_ */
