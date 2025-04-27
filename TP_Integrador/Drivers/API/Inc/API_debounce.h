/**
 * @file API_debounce.h
 *
 *  Created on: Mar 28, 2025
 *      Author: Eze Arce
 */

 #ifndef API_INC_API_DEBOUNCE_H_
 #define API_INC_API_DEBOUNCE_H_
 
 #include <stdbool.h>
 #include <stdint.h>
 #include "API_delay.h"
 /**
  * @brief Lee el estado actual del pulsador.
  *
  * @return true si el pulsador está presionado, false si está liberado.
  */
 bool_t readKey();
 
 /**
  * @brief Indica si el pulsador sigue presionado (presión continua).
  *
  * @return true si el pulsador se mantiene presionado, false en caso contrario.
  */
 bool_t readPressing();
 
 /**
  * @brief Indica si se ha detectado una nueva pulsación del botón.
  *
  * Se activa una sola vez por cada nuevo evento de presión.
  *
  * @return true si se detectó una nueva pulsación, false si no.
  */
 bool_t readPushed();
 
 #endif /* API_INC_API_DEBOUNCE_H_ */
 
