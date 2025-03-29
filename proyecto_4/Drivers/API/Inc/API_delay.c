/*
 * API_delay.c
 *
 *  Created on: Mar 20, 2025
 *      Author: Eze Arce
 */

#include "API_delay.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "stm32f4xx_hal.h"


/**
 * @brief Inicializa el temporizador con una duracion especifica.
 *
 * @param delay Puntero a la estructura de temporizador.
 * @param duration Duracion del temporizador en milisegundos.
 *
 * @note Si el puntero `delay` es NULL, el programa entrara en un bucle infinito.
 *
 * @details Esta funcion configura la duracion del temporizador y lo inicializa en estado inactivo.
 */
void delayInit( delay_t * delay, tick_t duration ){

	if(delay != NULL){
		delay->duration = duration;
		delay->running = false;
	}
	else{
		while(1){
		//El programa se cuelga aca si delay no esta correctamente inicializado.
				}
	}
}

/**
 * @brief Verifica si ha transcurrido el tiempo configurado en un temporizador.
 *
 * @param delay Puntero a la estructura de temporizador (delay_t).
 * @return bool_t Devuelve `true` si el tiempo ha transcurrido, `false` en caso contrario.
 *
 * @note Si el puntero `delay` es NULL, el programa entrará en un bucle infinito.
 *
 * @details Si el temporizador no está en ejecución, lo inicia y devuelve `false`.
 * Si ya está corriendo, verifica si el tiempo configurado ha pasado y, de ser así,
 * detiene el temporizador y devuelve `true`.
 */
bool_t delayRead( delay_t * delay ){


	if(delay != NULL){
		if(!delay->running){
			delay->running = true;
			delay->startTime = HAL_GetTick();
			return false;
		}
		else{
			if((HAL_GetTick() - delay->startTime) >= delay->duration){
				delay->running = false;
				return true;
			}
			else{
				return false;
			}
		}
	}
	else{
		while(1){
		//El programa se cuelga aca si delay no esta correctamente inicializado.
				}
	}

}

/**
 * @brief Establece la duración de un temporizador.
 *
 * @param delay Puntero a la estructura de temporizador (delay_t).
 * @param duration Duración del temporizador en milisegudnos.
 *
 * @note Si el puntero `delay` es NULL, el programa entrará en un bucle infinito.
 */

void delayWrite( delay_t * delay, tick_t duration ){

	if(delay != NULL){
		delay->duration=duration;
	}
	else{
		while(1){
		//El programa se cuelga aca si delay no esta correctamente inicializado.
				}
	}
}


/**
 * @brief Verifica si el temporizador está en ejecución.
 *
 * @param delay Puntero a la estructura de temporizador.
 * @return bool_t Devuelve `true` si el temporizador está en ejecución, `false` en caso contrario.
 *
 * @note Si el puntero `delay` es NULL, el programa entrará en un bucle infinito.
 */
bool_t delayIsRunning(delay_t * delay){

	if(delay != NULL){
	return delay->running;
	}
	else{
		while(1){
		//El programa se cuelga aca si delay no esta correctamente inicializado.
		}
	}
}
