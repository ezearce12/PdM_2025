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

void delayInit( delay_t * delay, tick_t duration ){

	delay->duration = duration;
	delay->running = false;

}

bool_t delayRead( delay_t * delay ){

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

void delayWrite( delay_t * delay, tick_t duration ){
	delay->duration=duration;
}

bool_t delayIsRunning(delay_t * delay){

	if(delay != NULL){
	return delay->running;
	}
	else{
		return false;
	}
}
