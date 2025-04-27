/**
 * @file API_delay.h
 *
 * @brief Módulo de temporizadores no bloqueantes.
 *
 * Este encabezado define las estructuras y funciones necesarias para
 * trabajar con temporizadores utilizando la función HAL_GetTick().
 */

 #ifndef API_API_DELAY_H_
 #define API_API_DELAY_H_
 
 #include <stdint.h>
 #include <stdbool.h>
 #include <assert.h>
 
 /**
  * @brief Definición del tipo de dato para contar ticks (milisegundos).
  */
 typedef uint32_t tick_t;
 
 /**
  * @brief Definición de tipo booleano.
  */
 typedef bool bool_t;
 
 /**
  * @brief Estructura de temporizador no bloqueante.
  */
 typedef struct {
     tick_t startTime;   /**< Marca de tiempo del inicio del temporizador. */
     tick_t duration;    /**< Duración configurada del temporizador (en ms). */
     bool_t running;     /**< Estado del temporizador (true: en marcha, false: detenido). */
 } delay_t;
 
 /**
  * @brief Inicializa un temporizador.
  *
  * @param[in,out] delay Puntero a la estructura de temporizador (delay_t).
  * @param[in] duration Duración inicial del temporizador en milisegundos.
  */
 void delayInit(delay_t *delay, tick_t duration);
 
 /**
  * @brief Lee el estado del temporizador.
  *
  * Verifica si el tiempo configurado ha expirado.
  *
  * @param[in,out] delay Puntero a la estructura de temporizador (delay_t).
  * @return true si el tiempo expiró, false en caso contrario.
  */
 bool_t delayRead(delay_t *delay);
 
 /**
  * @brief Modifica la duración de un temporizador.
  *
  * Permite actualizar la duración configurada del temporizador.
  *
  * @param[in,out] delay Puntero a la estructura de temporizador (delay_t).
  * @param[in] duration Nueva duración en milisegundos.
  */
 void delayWrite(delay_t *delay, tick_t duration);
 
 #endif /* API_API_DELAY_H_ */
 
