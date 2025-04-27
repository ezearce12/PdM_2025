/**
 * @file TF-LC02_Port.h
 * @brief Interfaz de bajo nivel para la comunicación UART con el sensor TF-LC02.
 *
 * Este archivo declara las funciones necesarias para transmitir y recibir datos,
 * en modo bloqueante e interrupción, utilizando UART.
 */

 #ifndef API_INC_TF_LC02_PORT_H_
 #define API_INC_TF_LC02_PORT_H_
 
 #include "stm32f4xx_hal.h"
 #include "main.h"
 
 /**
  * @brief Transmite datos hacia el sensor TF-LC02 mediante UART en modo bloqueante.
  *
  * @param[in] pData Puntero al buffer de datos a transmitir.
  * @param[in] Size Cantidad de bytes a transmitir.
  * @param[in] Timeout Tiempo máximo de espera para completar la transmisión (en ms).
  */
 void TFLC02_Port_Transmit(const uint8_t *pData, uint16_t Size, uint32_t Timeout);
 
 /**
  * @brief Recibe datos desde el sensor TF-LC02 mediante UART en modo bloqueante.
  *
  * @param[out] pData Puntero al buffer donde se almacenarán los datos recibidos.
  * @param[in] Size Cantidad de bytes a recibir.
  * @param[in] Timeout Tiempo máximo de espera para completar la recepción (en ms).
  */
 void TFLC02_Port_Receive(uint8_t *pData, uint16_t Size, uint32_t Timeout);
 
 /**
  * @brief Transmite datos hacia el sensor TF-LC02 mediante UART en modo interrupción.
  *
  * @param[in] huart Puntero a la estructura de la UART utilizada (no es utilizado internamente, se usa huart4).
  * @param[in] pData Puntero al buffer de datos a transmitir.
  * @param[in] Size Cantidad de bytes a transmitir.
  */
 void TFLC02_Port_Transmit_IT(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size);
 
 /**
  * @brief Recibe datos desde el sensor TF-LC02 mediante UART en modo interrupción.
  *
  * @param[out] pData Puntero al buffer donde se almacenarán los datos recibidos.
  * @param[in] Size Cantidad de bytes a recibir.
  */
 void TFLC02_Port_Receive_IT(uint8_t *pData, uint16_t Size);
 
 #endif /* API_INC_TF_LC02_PORT_H_ */
 
