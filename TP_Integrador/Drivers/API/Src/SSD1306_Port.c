/**
 * @file SSD1306_Port.c
 * @brief Implementación de funciones de bajo nivel para la comunicación con la pantalla SSD1306.
 *
 * Este archivo proporciona una capa de abstracción para la transmisión de comandos y datos
 * hacia la pantalla OLED SSD1306 a través del bus I2C u otros métodos de comunicación,
 * dependiendo de la plataforma utilizada.
 *
 * @details
 * Funciones principales:
 * - Enviar comandos individuales al display.
 * - Enviar bloques de datos (buffer gráfico).
 * - Inicializar el hardware de comunicación necesario.
 *
 * @note
 * Estas funciones deben ser implementadas para cada plataforma o microcontrolador
 * específico que utilice el driver de pantalla SSD1306.
 *
 */

 #include "../../Drivers/API/Inc/SSD1306_Port.h"

 extern I2C_HandleTypeDef hi2c1;
 
 #define SSD1306_I2C_ADDR         0x78 // Dirección I2C (0x3C << 1)
 
 void SSD1306_Error_Handler(void);
 
 /**
  * @brief Envía datos al display SSD1306 mediante I2C.
  *
  * @param[in] pData Puntero al buffer de datos a transmitir.
  * @param[in] Size Cantidad de bytes a transmitir.
  */
 void SSD1306_I2C_Transmit(uint8_t *pData, uint16_t Size){
 
	 HAL_StatusTypeDef err = HAL_I2C_Master_Transmit(&hi2c1, SSD1306_I2C_ADDR, pData, Size,  HAL_MAX_DELAY);
 
	 if(err != HAL_OK){
		 SSD1306_Error_Handler();
	 }
 
 }
 
 /**
  * @brief Manejador de errores para la comunicación I2C.
  *
  * En caso de error, enciende el LED de la placa y entra en un bucle infinito.
  */
 void SSD1306_Error_Handler(void){
 
	 //Se enciende el led de la placa para alertar un error
	 HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
 
	 //While infinito
	 while(1){
 
	 }
 
 }
 
