/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  *
  *Punto 1

		Crear un nuevo proyecto como copia del proyecto realizado para la práctica 2.
		Crear una carpeta API dentro de la carpeta Drivers en la estructura de directorios del nuevo proyecto. Crear dentro de la carpeta API, subcarpetas /Src y /Inc.
		Encapsular las funciones necesarias para usar retardos no bloqueantes en un archivo fuente API_delay.c con su correspondiente archivo de cabecera API_delay.h, y ubicar estos archivos en la carpeta API creada.

		En API_delay.h se deben ubicar los prototipos de las funciones y declaraciones

		typedef uint32_t tick_t; // Qué biblioteca se debe incluir para que esto compile?
		typedef bool bool_t;	  // Qué biblioteca se debe incluir para que esto compile?
		typedef struct{
		   tick_t startTime;
		   tick_t duration;
		   bool_t running;
		} delay_t;
		void delayInit( delay_t * delay, tick_t duration );
		bool_t delayRead( delay_t * delay );
		void delayWrite( delay_t * delay, tick_t duration );

		En API_delay.c se deben ubicar la implementación de todas las funciones.
		nota: cuando se agregar carpetas a un proyecto de eclipse se deben incluir en el include path para que se incluya su contenido en la compilación.  Se debe hacer clic derecho sobre la carpeta con los archivos de encabezamiento y seleccionar la opción add/remove include path.


	Punto 2

		Implementar un programa que utilice retardos no bloqueantes y haga titilar en forma periódica un led de la placa NUCLEO-F4xx de acuerdo a una secuencia predeterminada:

		const uint32_t TIEMPOS[] = {500, 100, 100, 1000};
		Utilizar la función delayWrite y una única variable tipo delay_t para cambiar el tiempo de encendido del led.
		NOTA: los tiempos indicados son de encendido y el led debe trabajar con duty = 50%

		Punto 3

		Implementar la siguiente función auxiliar pública en API_delay.c

		bool_t delayIsRunning(delay_t * delay);

		Esta función debe devolver una copia del valor del campo running de la estructura delay_t

		Utilizar esta función en el código implementado para el punto dos para verificar que el delay no esté corriendo antes de cambiar su valor con delayWrite.


  *
  *
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "API_delay.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  const uint32_t TIEMPOS[] = {500, 100, 100, 1000};
    uint8_t cantTiempos = sizeof(TIEMPOS) / sizeof(TIEMPOS[0]);
    uint8_t indice = 0;

    delay_t delay;

    bool ledState = false;

    delayInit(&delay, TIEMPOS[indice]);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {


//	  if(delayRead(&non_block_delay_1)){
//		  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
//	  }


		if (delayRead(&delay))
		{
			ledState = !ledState;
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, ledState ? GPIO_PIN_SET : GPIO_PIN_RESET);

			indice ++;

			if (indice >= cantTiempos) {
				indice = 0;
			}

			if(delayIsRunning(&delay) == false){
				delayWrite(&delay, TIEMPOS[indice]);
			}

		}
//	  }


//	  		  if(!led_state){
//	  			  cycle_count++;
//	  			  if(cycle_count == time_cycles[actual_pattern]){
//	  				  cycle_count=0;
//	  				  if(actual_pattern==2){
//	  					  actual_pattern=0;
//	  				  }
//	  				  else{
//	  					  actual_pattern++;
//	  				  }
//	  				delayWrite(&non_block_delay_1,time_pattern_dutty[actual_pattern]);
//	  			  }
//	  		  }
	  	  }


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
//}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

//void delayInit( delay_t * delay, tick_t duration ){
//
//	delay->duration = duration;
//	delay->running = false;
//
//}
//
//bool_t delayRead( delay_t * delay ){
//
//	if(!delay->running){
//		delay->running = true;
//		delay->startTime = HAL_GetTick();
//		return false;
//	}
//	else{
//		if((HAL_GetTick() - delay->startTime) >= delay->duration){
//			delay->running = false;
//			return true;
//		}
//		else{
//			return false;
//		}
//	}
//
//}
//
//void delayWrite( delay_t * delay, tick_t duration ){
//	delay->duration=duration;
//}



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
