/*
 * API_debounce.c
 *
 *  Created on: Mar 28, 2025
 *      Author: Eze Arce
 */

#include "main.h"
#include "API_debounce.h"
#include "stdlib.h"
#include "stm32f4xx_hal.h"



typedef enum{
BUTTON_UP,
BUTTON_FALLING,
BUTTON_DOWN,
BUTTON_RAISING,
} debounceState_t;

static void buttonPressed(void);
static void buttonReleased(void);

//Maqujina de estado
static debounceState_t FSM;

//Delay que se usa para controlar el rebote
static delay_t delay;

//Variable de pulsador
static bool pushed = false;

//Tiempo de control de rebote
const uint8_t time_ms = 40;

void debounceFSM_init(){
	FSM = BUTTON_UP;
}

void debounceFSM_update(){

	switch(FSM){

	case BUTTON_UP:

		if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET){
			FSM = BUTTON_FALLING;
			delayInit(&delay, time_ms);
		}
		break;

	case BUTTON_FALLING:

		pushed = true;

		if(delayRead(&delay)){
			if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET){
				FSM = BUTTON_DOWN;
				buttonPressed();
			}
			else{
				FSM = BUTTON_UP;
			}
		}
		break;

	case BUTTON_DOWN:
		if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET){
			FSM = BUTTON_RAISING;
			delayInit(&delay, time_ms);
		}
		break;

	case BUTTON_RAISING:
		if(delayRead(&delay)){
			if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET){
				FSM = BUTTON_UP;
				buttonReleased();
			}
			else{
				FSM = BUTTON_DOWN;
			}
		}
		break;

	default:
		break;
	}

}

void buttonPressed(){

	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);

}

void buttonReleased(){

	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

}

bool_t readKey(){

	if(pushed){
		pushed = false;
		return true;
	}
	else{
		return false;
	}

}
