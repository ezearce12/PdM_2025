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

//Funcion que se llama cuando se presiona el pulsador
static void buttonPressed(void);

//Funcion que se llama cuando se suelta el pulsador
static void buttonReleased(void);

//Maqujina de estado
static debounceState_t FSM;

//Delay que se usa para controlar el rebote
static delay_t delay;

//Variable de pulsador
static bool pushed = false;

//Tiempo de control de rebote
const uint8_t time_ms = 40;

//Se inicializa la maquina de estado en el estado inicial
void debounceFSM_init(){
	FSM = BUTTON_UP;
}

/**
 * @brief Actualiza el estado de la maquina de estados para el rebote del pulsador.
 *
 * Evalua el estado del pulsador y maneja los cambios entre estados con un tiempo de rebote.
 */
void debounceFSM_update(){

	switch(FSM){

	case BUTTON_UP:

		if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET){
			FSM = BUTTON_FALLING;
			delayInit(&delay, time_ms);
		}
		break;

	case BUTTON_FALLING:

		if(delayRead(&delay)){
			if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET){
				FSM = BUTTON_DOWN;
				//Se registra que el pulsador es presionado.
				pushed = true;
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

/**
 * @brief Funcion a ejecutar cuando se presiona el pulsador.
 *
 * Esta funcion enciende el LED conectado al puerto LD2.
 */
//static void buttonPressed(){
//
//	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
//
//}

/**
 * @brief Funcion a ejecutar cuando se suelta el pulsador.
 *
 * Esta funcion apaga el LED conectado al puerto LD2.
 */
//static void buttonReleased(){
//
//	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
//
//}

/**
 * @brief Verifica si el boton fue presionado.
 *
 * @return true si se detecto una pulsacion, false en caso contrario.
 *
 * Devuelve true solo una vez por pulsación detectada. Y cuando se lee la variable se torna a false.
 */
bool_t readKey(){

	if(pushed){
		pushed = false;
		return true;
	}
	else{
		return false;
	}

}
