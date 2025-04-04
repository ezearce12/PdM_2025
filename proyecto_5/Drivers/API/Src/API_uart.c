/*
 * API_uart.c
 *
 *  Created on: Apr 3, 2025
 *      Author: Eze Arce
 */
#include "main.h"
#include "API_uart.h"
#include "stdlib.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

UART_HandleTypeDef huart2;

const uint16_t MAX_TX_BUFFER = 512;
const uint16_t MAX_RX_BUFFER = 512;


/**	Inicializa la UART Numero 2 y envia Un primer mensaje con las configuraciones propias.
 *
 */
bool uartInit(void){

	char buffer[120];

	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;

	sprintf(buffer,
	        "BaudRate: %lu\n"
	        "WordLength: %s\n"
	        "StopBits: %s\n"
	        "Parity: %s\n"
	        "Mode: %s\n"
	        "Flow Control: %s\n"
	        "Oversampling: %lu\n",
	        huart2.Init.BaudRate,
	        (huart2.Init.WordLength == UART_WORDLENGTH_8B) ? "8 bits" : "9 bits",
	        (huart2.Init.StopBits == UART_STOPBITS_1) ? "1 bit" : "2 bits",
	        (huart2.Init.Parity == UART_PARITY_NONE) ? "None" :
	        (huart2.Init.Parity == UART_PARITY_EVEN) ? "Even" : "Odd",
	        (huart2.Init.Mode == UART_MODE_TX_RX) ? "TX/RX" :
	        (huart2.Init.Mode == UART_MODE_TX) ? "TX only" : "RX only",
	        (huart2.Init.HwFlowCtl == UART_HWCONTROL_NONE) ? "None" :
	        (huart2.Init.HwFlowCtl == UART_HWCONTROL_RTS) ? "RTS" :
	        (huart2.Init.HwFlowCtl == UART_HWCONTROL_CTS) ? "CTS" : "RTS/CTS",
	        huart2.Init.OverSampling
	    );

	  if (HAL_UART_Init(&huart2) != HAL_OK)
	  {
		  Error_Handler();
		  return false;

	  }
	  else{
		  uartSendString((uint8_t*)buffer);
		  return true;
	  }

}

/**
 * Envia un string por la UART 2
 * Debe pasarse el puntero del string que se quiere enviar
 */
void uartSendString(uint8_t * pstring){

	if(pstring == NULL) return;

	//strlen devuelve el tamaño de un string
	uint16_t string_size = strlen((char *)pstring);

	if(string_size != 0 && string_size < MAX_TX_BUFFER){
		if(HAL_UART_Transmit(&huart2,pstring,string_size,HAL_MAX_DELAY) != HAL_OK) {
			  Error_Handler();
		}
	}

}
/**
 * Envia un string por la UART 2
 * Debe pasarse el puntero del string que se quiere enviar y el tamaño del string
 */
void uartSendStringSize(uint8_t * pstring, uint16_t size){

	if(pstring == NULL) return;

	if(size != 0 && size < MAX_TX_BUFFER){
		if(HAL_UART_Transmit(&huart2,pstring,size,HAL_MAX_DELAY) != HAL_OK) {
			  Error_Handler();
		}
	}

}

/*
 * Recibe datos de la uart 2
 * Se le debe pasar el puntero a donde se guardan los datos recibidos
 * Y el tammaño del buffer.
 */
void uartReceiveStringSize(uint8_t * pstring, uint16_t size){

	if(HAL_UART_Receive(&huart2, pstring, size, HAL_MAX_DELAY) != HAL_OK) {
		  Error_Handler();
	}

}
