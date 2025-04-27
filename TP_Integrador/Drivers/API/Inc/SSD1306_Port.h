/*
 * SSD1306_Port.h
 *
 *  Created on: Apr 19, 2025
 *      Author: Eze
 */

 #ifndef API_INC_SSD1306_PORT_H_
 #define API_INC_SSD1306_PORT_H_
 
 #include "main.h"
 #include "stm32f4xx_hal.h"
 #include <string.h>
 #include <stdlib.h>
 
 
 /**
  * @brief Envía datos al display SSD1306 mediante I2C.
  *
  * @param[in] pData Puntero al buffer de datos a transmitir.
  * @param[in] Size Cantidad de bytes a transmitir.
  */
 void SSD1306_I2C_Transmit(uint8_t *pData, uint16_t Size);
 
 
 #endif /* API_INC_SSD1306_PORT_H_ */
 