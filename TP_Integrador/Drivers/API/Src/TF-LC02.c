/*
 * TF-LC02.c
 *
 *  Created on: Apr 2, 2025
 *      Author: Eze Arce
 */


#include "main.h"
#include "../../Drivers/API/Inc/TF-LC02.h"
#include "../../Drivers/API/Inc/TF-LC02_Port.h"
#include "../../Drivers/API/Inc/API_delay.h"



#define LIDAR_FRAME_LEN 32
#define LIDAR_FRAME_MIN 5
#define LIDAR_FRAME_HEADER1 0x55
#define LIDAR_FRAME_HEADER2 0xAA
#define LIDAR_FRAME_END 	0xFA

//Enumeracion de protocolos soportados por el sensor
typedef enum {
    UART_AND_I2C = 0x41,
    ONLY_I2C = 0x49,
    ONLY_UART = 0x55
} TF_PORT;

//Enumeracion de estados de calibracion del sensor
typedef enum {
    NO_CALIBRATE,
    CROSSTALK_CALIBRATE,
    OFFSET_CALIBRATE,
    COMPLETE_CALIBRATE
} TF_CALIBRATE;


//Enumeracion de estados para la recepcion de datos
typedef enum {
    STATE_WAIT_HEADER_1,
    STATE_WAIT_HEADER_2,
    STATE_CMD,
    STATE_LEN,
    STATE_DATA,
    STATE_END
} ParserState;

//Enumeracion de comandos del sensor
typedef enum{
	Measure = 0x81,					//Medicion de distancia en [mm].
	Crosstalk_correction,			//Correccion de Crosstalk.
	Offset_correction,				//Correccion de Offset.
	TFLC02_Reset,					//Reset del sensor.
	Get_Factory_default_settings,	//Obtencion de datos de configuracion por defecto.
	Get_Prod_info,					//Obtencion de la informacion del producto.
}TF_CMD;

//Enumeracion de errores del sensor
typedef enum{
	VALID_DATA,						//Valor valido.
	VCSEL_SHORT,					//VCSEL esta corto circuitado.
	LOW_SIGNAL,						//La cantidad de luz reflejada obtenida del objeto detectado es pequeña.
	LOW_SN = 0x04,					//La relación entre la luz reflejada del objeto detectado y la luz perturbadora es pequeña.
	TOO_MUCH_AMB = 0x08,			//La luz perturbadora es grande.
	WAF = 0x10,						//Error wrapping.
	CAL_ERROR = 0x20,				//Error de calculo interno.
	CROSSTALK_ERROR = 0x80,			//Error de crosstalk.
}TF_ERROR;

// Estructura de datos para almacenar la información del sensor.
typedef struct {
    uint16_t distance;
    uint8_t errorCode;
    bool receiveComplete;
    uint8_t calib;
    uint8_t port;
    uint8_t firmware;
    uint8_t type;
    uint8_t offset_short1;
    uint8_t offset_short2;
    uint8_t offset_long1;
    uint8_t offset_long2;
    uint16_t crosstalk;
} TF_t;

static uint8_t rx_byte;							// Byte de recepcion.
static uint8_t rx_buffer[LIDAR_FRAME_LEN];		// Buffer para la recepcion de datos.
static uint8_t rx_index = 0;					// Indice del buffer.

bool frame_present;


volatile TF_t lidar = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

bool TFLC02_Parse_Packet(void);

bool ParserInfo(uint8_t *buffer, uint8_t size);

void TFLC02_Send_Command(uint8_t cmd);


//Se envia un comando al sensor
void TFLC02_Send_Command(uint8_t cmd) {

	assert(cmd >= Measure && cmd <= Get_Prod_info);

    uint8_t frame[5] = {0x55, 0xAA, cmd, 0x00, 0xFA};
    TFLC02_Port_Transmit(frame, sizeof(frame), HAL_MAX_DELAY);
}


//Inicializa el puerto UART.
void TFLC02_Init(void){
	TFLC02_Port_Init();
}

//Inicia la recepcion UART por interrupcion.
void TFLC02_Start(void){
	TFLC02_Port_Receive_IT(&rx_byte, 1);
}

//Se envia el comando para recibir la distancia medida
void TFLC02_Mesure(void){
	TFLC02_Send_Command(Measure);
}

//Se envia el comando para recibir informacion del sensor
void TFLC02_Info(void){
	TFLC02_Send_Command(Get_Prod_info);
}

//Se envia el comando para recibir los valores de fabrica del sensor
void TFLC02_DefaultSettings(void){
	TFLC02_Send_Command(Get_Factory_default_settings);
}

//Se revisa si la respuesta del sensor esta completa.
//Devuelve True o False dependiendo si la respuesta del sensor esta completa.
bool TFLC02_RspComplete(void){

	if(lidar.receiveComplete){
		lidar.receiveComplete = false;
		return true;
	}
	else{
		return false;
	}

}

//Devuelve la ultima distancia sensada.
uint16_t TFLC02_GetDistance(void) {

	return lidar.distance;

}

//Devuelve el tipo de puerto que utiliza el sensor.
uint8_t TFLC02_GetPort(void) {

	assert(lidar.port == ONLY_I2C || lidar.port == ONLY_UART || lidar.port == UART_AND_I2C);

	return lidar.port;

}

//Devuelve la version de firmware del sensor.
uint8_t TFLC02_GetFirm(void) {

    return lidar.firmware;
}

//Devuelve la calibracion del sensor.
uint8_t TFLC02_GetCalib(void) {

	assert(lidar.calib >= NO_CALIBRATE && lidar.calib <= COMPLETE_CALIBRATE);

    return lidar.calib;
}

//Devuelve si existe una trama para tratar
bool TFLC02_FramePresent(void) {

	if(frame_present){
		frame_present = false;
		return true;
	}
    return false;

}

// Callback de recepción
// Se almacenan los bytes recibidos por la UART en un buffer global.
// Se levanta una bandera indicando si la ttrama esta completa
// Se desestima la trama si excede los limites de longitud correspondientes.
void TFLC02__RxCpltCallback(UART_HandleTypeDef *huart) {

	assert(huart != NULL);

	if (huart->Instance == UART4) {		//Se verifica el puerto de la interrupcion.

		rx_buffer[rx_index] = rx_byte;
		rx_index++;

		if(rx_index >= LIDAR_FRAME_LEN){
			rx_index=0;
			frame_present = false;
		}

		if(rx_byte == LIDAR_FRAME_END){
			frame_present = true;
		}

        TFLC02_Start();		//Se reinicia la recepcion UART.
    }
}


//Maquina de estado que verifica la integridad de de las tramas recibidas y actualiza la informacion del sensor correspondiente.
//Se verifica encabezado, longitud de trama y finalde trama.
//Retorna true si es una trama valiada, false si no lo es.
bool TFLC02_Parse_Packet(void) {

//	Para cada trama analizada se inicializan las variables de control

	uint8_t i = 0;
	ParserState parser_state = STATE_WAIT_HEADER_1;
	uint8_t expected_length = 0;
	uint8_t byte;
	uint8_t size = rx_index;
	uint8_t byte_index = 0;

	while (i < size) {

		byte = rx_buffer[i];

        switch (parser_state) {

            case STATE_WAIT_HEADER_1:
                if (byte == LIDAR_FRAME_HEADER1) {
                	byte_index++;
                    parser_state = STATE_WAIT_HEADER_2;
                }
                else{
                	parser_state = STATE_WAIT_HEADER_1;
                	rx_index = 0;
                	return false;
                }
                break;

            case STATE_WAIT_HEADER_2:
                if (byte == LIDAR_FRAME_HEADER2) {
                    parser_state = STATE_CMD;
                    byte_index++;
                } else {
                	parser_state = STATE_WAIT_HEADER_1;
                	rx_index = 0;
                	return false;
                }
                break;

            case STATE_CMD:
            	byte_index++;
            	parser_state = STATE_LEN;
            	break;

            case STATE_LEN:
            	expected_length = byte;						//Se almacena la longitud esperada.
            	if((expected_length + 5) <= LIDAR_FRAME_LEN){	//Se verifica que la longitud esperada del paquete no exceda el maximo permitido.
            		parser_state = (expected_length > 0) ? STATE_DATA : STATE_END;	//Se verifica si hay que esperar datos o no.
            		byte_index++;
            	}
            	else{
            		parser_state = STATE_WAIT_HEADER_1;
            		rx_index = 0;
            	}

            	break;

            case STATE_DATA:

            		byte_index++;
					if (byte_index == (4 + expected_length)) {		//Se verifica si se recibieron todos datos.
						parser_state = STATE_END;

					}
                break;

            case STATE_END:
					if (byte == LIDAR_FRAME_END) {		//Se verifica el ultimo byte.

						if(ParserInfo(rx_buffer,byte_index+1)){	//Se procesa el paquete.
							lidar.receiveComplete = true;
							rx_index = 0;
							return true;
						}
						else{
							lidar.receiveComplete = false;
							return false;
						}
					}

                //Se reinicia la maquina de estados.
                parser_state = STATE_WAIT_HEADER_1;
                rx_index = 0;

                break;

            default:
                parser_state = STATE_WAIT_HEADER_1;
                rx_index = 0;
                break;
        }

        i++;

	}


	rx_index = 0;
	return false;

}


//Se procesa el paquete recibido
//Recibe puntero al buffer que contiene los datos recibidos
//Recibe size: Tamaño del buffer
//Devuelve 1 si el paquete es válido y se procesó correctamente, 0 en caso contrario
bool ParserInfo(uint8_t *buffer, uint8_t size){
	static uint8_t cmd;

	    cmd = buffer[2];

	    // Interpretar campos según comando
	    switch(cmd) {
	        case Measure:  // Medición de distancia

					lidar.distance = (rx_buffer[4] << 8) | rx_buffer[5];
					lidar.errorCode = rx_buffer[6];

	            break;

	        case TFLC02_Reset:  // Reset
	            // No hay payload
	            break;

	        case Get_Factory_default_settings:

					lidar.offset_short1 = rx_buffer[4];
					lidar.offset_short2 = rx_buffer[5];
					lidar.offset_long1 = rx_buffer[6];
					lidar.offset_long2 =rx_buffer[7];
					lidar.crosstalk = (rx_buffer[8] << 8) | rx_buffer[9];
					lidar.calib = rx_buffer[10];

	            break;

	        case Get_Prod_info:

					lidar.type = rx_buffer[4];
					lidar.port = rx_buffer[5];
					lidar.firmware = rx_buffer[6];

	            break;

	        default:
	        	//Comando no soportado.
	        	return false;

	        	break;
	    }

	    return true;
}
