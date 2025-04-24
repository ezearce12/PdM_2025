/*
 * API_delay.c
 *
 *  Created on: Mar 20, 2025
 *      Author: Eze Arce
 */

#include "../../Drivers/API/Inc/API_delay.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "stm32f4xx_hal.h"

#define DEFAULT_DELAY_MS 50



//Se inicializa el delay.
//configurando su duracion, configurando su parametro running y tomando la muestra de tiempo de si inicio.
// recibe delay  - Puntero a la estructura de temporizador (delay_t).
// recibe duration - Duración del temporizador en milisegudnos.

void delayInit( delay_t * delay, tick_t duration ){

	assert(delay != NULL);

	delay->duration = duration;
	delay->running = true;
	delay->startTime = HAL_GetTick();

}


//Se revisa si el delay cumplio su periodo de actividad.
// recibe delay  - Puntero a la estructura de temporizador (delay_t).
//Retorna true o false segun corresponde
bool_t delayRead( delay_t * delay ){

	assert(delay != NULL);

	if((HAL_GetTick() - delay->startTime) >= delay->duration){
		delay->startTime = HAL_GetTick();
		return true;
	}
	else{
		return false;
	}
}

//Establece la duración de un temporizador.
// recibe delay  - Puntero a la estructura de temporizador (delay_t).
// recibe duration - Duración del temporizador en milisegudnos.


void delayWrite( delay_t * delay, tick_t duration ){

	assert(delay != NULL);

	if(duration <= 0) duration = DEFAULT_DELAY_MS;

	delay->startTime = HAL_GetTick();
	delay->duration = duration;
}

