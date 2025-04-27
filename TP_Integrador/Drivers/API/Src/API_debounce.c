/**
 * @file API_debounce.c
 *
 * @brief Implementación de una máquina de estados para el control de rebotes de un pulsador.
 *
 */


 #include "main.h"
 #include "../../Drivers/API/Inc/API_debounce.h"
 #include "stdlib.h"
 #include "stm32f4xx_hal.h"
 
 /**
  * @brief Estados posibles de la máquina de estados de antirebote.
  */
 typedef enum {
	 BUTTON_UP,       /**< Estado: botón liberado. */
	 BUTTON_FALLING,  /**< Estado: posible transición a presionado. */
	 BUTTON_DOWN,     /**< Estado: botón presionado. */
	 BUTTON_RAISING,  /**< Estado: posible transición a liberado. */
	 BUTTON_PRESSING, /**< Estado opcional no utilizado actualmente. */
 } debounceState_t;
 
 /**
  * @brief Variable que almacena el estado actual de la máquina de estados.
  */
 static debounceState_t FSM;
 
 /**
  * @brief Delay utilizado para filtrar el rebote del pulsador.
  */
 static delay_t delay;
 
 /**
  * @brief Variable que indica si el botón fue presionado.
  */
 static bool pushed = false;
 
 /**
  * @brief Tiempo de control de rebote en milisegundos.
  */
 const uint8_t time_ms = 10;
 
 /**
  * @brief Inicializa la máquina de estados de antirebote.
  *
  * Coloca el estado inicial en BUTTON_UP.
  */
 void debounceFSM_init(void) {
	 FSM = BUTTON_UP;
 }
 
 /**
  * @brief Actualiza el estado de la máquina de estados de antirebote.
  *
  * Evalúa periódicamente el estado físico del pulsador y maneja la transición de estados
  * considerando el tiempo de rebote para detectar presiones reales.
  */
 void debounceFSM_update(void) {
 
	 switch (FSM) {
 
	 case BUTTON_UP:
		 if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET) {
			 FSM = BUTTON_FALLING;
			 delayInit(&delay, time_ms);
		 }
		 break;
 
	 case BUTTON_FALLING:
		 if (delayRead(&delay)) {
			 if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET) {
				 FSM = BUTTON_DOWN;
				 pushed = true; /**< Se registra que el botón fue presionado. */
			 } else {
				 FSM = BUTTON_UP;
			 }
		 }
		 break;
 
	 case BUTTON_DOWN:
		 if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET) {
			 FSM = BUTTON_RAISING;
			 delayInit(&delay, time_ms);
		 }
		 break;
 
	 case BUTTON_RAISING:
		 if (delayRead(&delay)) {
			 if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET) {
				 FSM = BUTTON_UP;
			 } else {
				 FSM = BUTTON_DOWN;
			 }
		 }
		 break;
 
	 default:
		 /* No se espera llegar aquí. */
		 break;
	 }
 
 }
 
 
 /**
  * @brief Verifica si el boton fue presionado.
  *
  * @return true si se detecto una pulsacion, false en caso contrario.
  *
  * Devuelve true solo una vez por pulsación detectada. Y cuando se lee la variable se torna a false.
  */
 bool_t readPushed(){
 
	 if(pushed){
		 pushed = false;
		 return true;
	 }
	 else{
		 return false;
	 }
 
 }
 
