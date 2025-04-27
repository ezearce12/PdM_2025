/**
 * @file SSD1306.h
 * @brief Interfaz para el control de un display OLED SSD1306 vía I2C.
 *
 * Proporciona funciones para inicialización, control de encendido, limpieza,
 * y escritura de caracteres y strings en pantalla.
 *
 */

 #ifndef API_INC_SSD1306_H_
 #define API_INC_SSD1306_H_
 
 #include <string.h>
 #include "stm32f4xx_hal.h"
 #include <assert.h>
 
 /**
  * @name Dimensiones y configuraciones del display SSD1306
  * @{
  */
 
 /// @brief Ancho del display en píxeles.
 #define SSD1306_WIDTH           128
 
 /// @brief Alto del display en píxeles.
 #define SSD1306_HEIGHT          64
 
 /// @brief Máximo número de caracteres por línea (fuente 5x7).
 #define SDD1306_MAX_CHARACTER   21
 
 /// @brief Máximo número de páginas de memoria (8 páginas de 8 píxeles).
 #define SDD1306_MAX_PAGE        7
 
 /** @} */ // Fin de configuraciones
 
 /**
  * @brief Inicializa el display SSD1306 con los parámetros de configuración necesarios.
  */
 void SSD1306_Init(void);
 
 /**
  * @brief Limpia completamente la pantalla.
  */
 void SSD1306_Clear(void);
 
 /**
  * @brief Enciende el display (salir de modo de apagado).
  */
 void SSD1306_DisplayOn(void);
 
 /**
  * @brief Apaga el display (modo bajo consumo).
  */
 void SSD1306_DisplayOff(void);
 
 /**
  * @brief Escribe un solo carácter en la posición actual del cursor.
  *
  * @param c Carácter a escribir.
  */
 void SSD1306_WriteChar(char c);
 
 /**
  * @brief Escribe una cadena de texto en la pantalla, de forma continua.
  *
  * @param str Puntero a la cadena de caracteres terminada en NULL.
  */
 void SSD1306_WriteString(char* str);
 
 /**
  * @brief Muestra en pantalla los valores de medición actual, máxima y mínima.
  *
  * @param Actual Valor actual medido.
  * @param Maxima Valor máximo registrado.
  * @param Minima Valor mínimo registrado.
  */
 void SSD1306_PrintMesurement(uint16_t Actual, uint16_t Maxima, uint16_t Minima);
 
 /**
  * @brief Muestra en pantalla el valor de tiempo de muestreo seleccionado.
  *
  * @param muestreo Tiempo de muestreo en milisegundos.
  */
 void SSD1306_PrintMuestreo(uint32_t muestreo);
 
 /**
  * @brief Muestra en pantalla la configuración del puerto y calibración.
  *
  * @param port Puerto configurado.
  * @param cal Estado de calibración.
  */
 void SSD1306_PrintSetup(uint8_t port, uint8_t cal);
 
 #endif /* API_INC_SSD1306_H_ */
 
