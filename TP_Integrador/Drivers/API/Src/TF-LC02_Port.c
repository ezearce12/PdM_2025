/**
 * @file TF-LC02_Port.c
 * @brief Implementación de funciones de bajo nivel para la comunicación UART con el sensor TF-LC02.
 *
 * Este archivo proporciona las funciones de transmisión y recepción de datos,
 * tanto en modo bloqueante como por interrupción, además del manejo básico de errores.
 */


 #include "../../Drivers/API/Inc/TF-LC02.h"

 extern UART_HandleTypeDef huart4;
 
 extern void TFLC02__RxCpltCallback(UART_HandleTypeDef *huart);
 
 /**
  * @brief Manejador de errores en la comunicación con el sensor TF-LC02.
  *
  * En caso de error en la comunicación UART, esta función apaga el LED de la placa
  * y detiene el programa en un bucle infinito.
  */
 void TFLC02_Error_Handler(void);
 
 /**
  * @brief Envía datos al sensor TF-LC02 utilizando UART en modo bloqueante.
  *
  * @param[in] pData Puntero al buffer de datos a transmitir.
  * @param[in] Size Cantidad de bytes a transmitir.
  * @param[in] Timeout Tiempo máximo de espera para la transmisión.
  */
 void TFLC02_Port_Transmit(const uint8_t *pData, uint16_t Size, uint32_t Timeout){
 
	 HAL_StatusTypeDef status = HAL_UART_Transmit(&huart4, pData, Size, Timeout);
 
	 if(status != HAL_OK){
		 TFLC02_Error_Handler();
	 }
 
 }
 
 /**
  * @brief Recibe datos del sensor TF-LC02 utilizando UART en modo bloqueante.
  *
  * @param[out] pData Puntero al buffer donde se almacenarán los datos recibidos.
  * @param[in] Size Cantidad de bytes a recibir.
  * @param[in] Timeout Tiempo máximo de espera para la recepción.
  */
 void TFLC02_Port_Receive(uint8_t *pData, uint16_t Size, uint32_t Timeout){
 
	 HAL_StatusTypeDef status = HAL_UART_Receive(&huart4, pData, Size, Timeout);
 
	 if(status != HAL_OK){
		 TFLC02_Error_Handler();
	 }
 
 }
 
 void TFLC02_Port_Transmit_IT(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size){
 
	 HAL_StatusTypeDef status = HAL_UART_Transmit_IT(&huart4, pData, Size);
 
	 if(status != HAL_OK){
		 TFLC02_Error_Handler();
	 }
 
 }
 
 /**
  * @brief Envía datos al sensor TF-LC02 utilizando UART en modo interrupción.
  *
  * @param[in] huart Puntero a la estructura UART (no utilizado, siempre se usa huart4).
  * @param[in] pData Puntero al buffer de datos a transmitir.
  * @param[in] Size Cantidad de bytes a transmitir.
  */
 void TFLC02_Port_Receive_IT(uint8_t *pData, uint16_t Size){
 
	 HAL_StatusTypeDef status = HAL_UART_Receive_IT(&huart4, pData, Size);
 
	 if(status != HAL_OK){
		 TFLC02_Error_Handler();
	 }
 
 }
 
 /**
  * @brief Callback de HAL llamado al completarse la recepción UART.
  *
  * @param[in] huart Puntero a la estructura UART_HandleTypeDef.
  * @note Llama a la función de procesamiento de recepción del módulo TF-LC02.
  */
 void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
 
	 TFLC02__RxCpltCallback(huart);
 
 }
 
 /**
  * @brief Manejador de error específico del sensor TF-LC02.
  *
  * @note Apaga el LED de usuario para indicar error y detiene la ejecución.
  */
 void TFLC02_Error_Handler(void){
 
	 //Se apaga el led de la placa para alertar el error en el sensor
	 HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
 
	 //While infinito
	 while(1){
 
	 }
 
 }
 
 
 
