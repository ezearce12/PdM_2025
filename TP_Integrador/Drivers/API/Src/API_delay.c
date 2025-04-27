/**
 * @file API_delay.c
 * @brief Implementación de temporizadores no bloqueantes.
 *
 * Este módulo permite inicializar, leer y configurar temporizadores
 * de manera no bloqueante utilizando la función HAL_GetTick().
 */

 #include "../../Drivers/API/Inc/API_delay.h"	/**< Incluye el encabezado correspondiente. */
 #include <stdint.h>
 #include <stdbool.h>
 #include <stddef.h>
 #include "stm32f4xx_hal.h"
 
 #define DEFAULT_DELAY_MS 50  /**< Duración por defecto en milisegundos si no se especifica un valor válido. */
 
 /**
  * @brief Inicializa un temporizador.
  *
  * Configura la duración del temporizador, lo marca como activo (running)
  * y toma la muestra de tiempo de inicio.
  *
  * @param[in,out] delay Puntero a la estructura de temporizador (delay_t).
  * @param[in] duration Duración del temporizador en milisegundos.
  */
 void delayInit(delay_t *delay, tick_t duration) {
 
	 assert(delay != NULL);
 
	 delay->duration = duration;
	 delay->running = true;
	 delay->startTime = HAL_GetTick();
 }
 
 /**
  * @brief Lee el estado de un temporizador.
  *
  * Verifica si el tiempo configurado ha transcurrido.
  * Si se cumple el tiempo, reinicia el tiempo de inicio.
  *
  * @param[in,out] delay Puntero a la estructura de temporizador (delay_t).
  * @return true si el tiempo expiró, false en caso contrario.
  */
 bool_t delayRead(delay_t *delay) {
 
	 assert(delay != NULL);
 
	 if ((HAL_GetTick() - delay->startTime) >= delay->duration) {
		 delay->startTime = HAL_GetTick();
		 return true;
	 }
	 else {
		 return false;
	 }
 }
 
 /**
  * @brief Modifica la duración de un temporizador ya existente.
  *
  * Permite actualizar la duración del temporizador. Si el valor es inválido (menor o igual a 0),
  * se utiliza una duración por defecto.
  *
  * @param[in,out] delay Puntero a la estructura de temporizador (delay_t).
  * @param[in] duration Nueva duración del temporizador en milisegundos.
  */
 void delayWrite(delay_t *delay, tick_t duration) {
 
	 assert(delay != NULL);
 
	 if (duration <= 0) {
		 duration = DEFAULT_DELAY_MS;
	 }
 
	 delay->startTime = HAL_GetTick();
	 delay->duration = duration;
 }
 
