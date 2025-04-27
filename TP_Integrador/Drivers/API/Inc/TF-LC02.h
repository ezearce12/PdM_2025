/*
 * TF-LC02.h
 *
 *  Created on: Apr 2, 2025
 *      Author: Eze
 */

 #ifndef API_INC_TF_LC02_H_
 #define API_INC_TF_LC02_H_
 
 #include  "TF-LC02_Port.h"
 #include <stdbool.h>
 #include <string.h>
 #include <assert.h>
 
 /**
  * @brief Inicializa el sensor TF-LC02.
  *
  * Esta función configura el sensor TF-LC02 y prepara la comunicación con él.
  * Es necesario llamar a esta función antes de realizar cualquier operación
  * con el sensor.
  */
 void TFLC02_Init(void);
 
 /**
  * @brief Inicia la recepción de datos del sensor TF-LC02 por interrupciones UART.
  *
  * Esta función inicia la recepción del sensor TF-LC02, habilitando las
  * interrupciones UART para recibir datos del sensor.
  */
 void TFLC02_Start(void);
 
 /**
  * @brief Envia el comando para medir la distancia.
  *
  * Esta función envía el comando al sensor para que realice una medición de distancia
  * y retorne el valor en milímetros.
  */
 void TFLC02_Mesure(void);
 
 /**
  * @brief Envia el comando para obtener la información del sensor.
  *
  * Esta función envía un comando al sensor para obtener información sobre el
  * producto, como el tipo de sensor y la configuración de puerto.
  */
 void TFLC02_Info(void);
 
 /**
  * @brief Envia el comando para obtener la configuración por defecto del sensor.
  *
  * Esta función envía un comando al sensor para obtener los valores de configuración
  * por defecto del sensor, como los offsets y el crosstalk.
  */
 void TFLC02_DefaultSettings(void);
 
 /**
  * @brief Verifica si la respuesta del sensor está completa.
  *
  * Esta función verifica si el sensor ha completado el envío de datos, lo que
  * indica que la medición o la solicitud de información ha sido completada.
  *
  * @return true Si la respuesta está completa.
  * @return false Si la respuesta no está completa.
  */
 bool TFLC02_RspComplete(void);
 
 /**
  * @brief Obtiene la última distancia medida por el sensor.
  *
  * Esta función retorna la última distancia medida por el sensor en milímetros.
  *
  * @return uint16_t La distancia medida en milímetros.
  */
 uint16_t TFLC02_GetDistance(void);
 
 /**
  * @brief Obtiene el tipo de puerto utilizado por el sensor.
  *
  * Esta función retorna el tipo de puerto que utiliza el sensor, que puede ser
  * UART, I2C, o ambos.
  *
  * @return uint8_t El tipo de puerto utilizado (UART, I2C, o ambos).
  */
 uint8_t TFLC02_GetPort(void);
 
 /**
  * @brief Obtiene la versión del firmware del sensor.
  *
  * Esta función retorna la versión del firmware del sensor TF-LC02.
  *
  * @return uint8_t La versión del firmware del sensor.
  */
 uint8_t TFLC02_GetFirm(void);
 
 /**
  * @brief Obtiene el estado de la calibración del sensor.
  *
  * Esta función retorna el estado de calibración del sensor, indicando si está
  * calibrado o si necesita calibración adicional.
  *
  * @return uint8_t El estado de la calibración del sensor.
  */
 uint8_t TFLC02_GetCalib(void);
 
 /**
  * @brief Verifica si hay una trama completa para procesar.
  *
  * Esta función verifica si existe una trama completa en el buffer de recepción
  * que puede ser procesada. La trama debe contener todos los datos necesarios.
  *
  * @return true Si hay una trama completa disponible.
  * @return false Si no hay una trama completa.
  */
 bool TFLC02_FramePresent(void);
 
 #endif /* API_INC_TF_LC02_H_ */
 