/**
 * @file TF-LC02.c
 * @brief Driver para la comunicación y procesamiento de datos del sensor LiDAR TF-LC02.
 * @details Implementa el envío de comandos, recepción y parseo de respuestas para el sensor.
 */


#include "main.h"
#include "../../Drivers/API/Inc/TF-LC02.h"
#include "../../Drivers/API/Inc/TF-LC02_Port.h"
#include "../../Drivers/API/Inc/API_delay.h"



#define LIDAR_FRAME_LEN 	32    /**< Longitud máxima de una trama de datos */
#define LIDAR_FRAME_MIN		5     /**< Longitud mínima de una trama de datos */
#define LIDAR_FRAME_HEADER1 0x55 /**< Primer byte de cabecera de la trama */
#define LIDAR_FRAME_HEADER2 0xAA /**< Segundo byte de cabecera de la trama */
#define LIDAR_FRAME_END   	0xFA    /**< Byte de fin de trama */

/**
 * @brief Protocolos soportados por el sensor.
 */
typedef enum {
    UART_AND_I2C = 0x41,  /**< Comunicación simultánea UART e I2C */
    ONLY_I2C = 0x49,      /**< Solo comunicación I2C */
    ONLY_UART = 0x55      /**< Solo comunicación UART */
} TF_PORT;

/**
 * @brief Estados de calibración del sensor.
 */
typedef enum {
    NO_CALIBRATE,         /**< Sin calibración */
    CROSSTALK_CALIBRATE,  /**< Calibración de crosstalk */
    OFFSET_CALIBRATE,     /**< Calibración de offset */
    COMPLETE_CALIBRATE    /**< Calibración completa */
} TF_CALIBRATE;

/**
 * @brief Estados de la máquina de estados para recepción de datos.
 */
typedef enum {
    STATE_WAIT_HEADER_1,
    STATE_WAIT_HEADER_2,
    STATE_CMD,
    STATE_LEN,
    STATE_DATA,
    STATE_END
} ParserState;

/**
 * @brief Comandos disponibles del sensor TF-LC02.
 */
typedef enum {
    Measure = 0x81,                /**< Medición de distancia en [mm] */
    Crosstalk_correction,          /**< Corrección de crosstalk */
    Offset_correction,             /**< Corrección de offset */
    TFLC02_Reset,                  /**< Reseteo del sensor */
    Get_Factory_default_settings,  /**< Obtención de valores de fábrica */
    Get_Prod_info                  /**< Obtención de información del producto */
} TF_CMD;

/**
 * @brief Códigos de error reportados por el sensor.
 */
typedef enum {
    VALID_DATA,             /**< Datos válidos */
    VCSEL_SHORT,            /**< Cortocircuito en VCSEL */
    LOW_SIGNAL,             /**< Señal reflejada baja */
    LOW_SN = 0x04,          /**< Baja relación señal/ruido */
    TOO_MUCH_AMB = 0x08,    /**< Demasiada luz ambiente */
    WAF = 0x10,             /**< Error de wrapping */
    CAL_ERROR = 0x20,       /**< Error de cálculo interno */
    CROSSTALK_ERROR = 0x80  /**< Error de crosstalk */
} TF_ERROR;

/**
 * @brief Estructura para almacenar la información del sensor.
 */
typedef struct {
    uint16_t distance;        /**< Distancia medida [mm] */
    uint8_t errorCode;        /**< Código de error */
    bool receiveComplete;     /**< Bandera de recepción completa */
    uint8_t calib;            /**< Estado de calibración */
    uint8_t port;             /**< Tipo de puerto configurado */
    uint8_t firmware;         /**< Versión de firmware */
    uint8_t type;             /**< Tipo de producto */
    uint8_t offset_short1;    /**< Offset corto 1 */
    uint8_t offset_short2;    /**< Offset corto 2 */
    uint8_t offset_long1;     /**< Offset largo 1 */
    uint8_t offset_long2;     /**< Offset largo 2 */
    uint16_t crosstalk;       /**< Crosstalk */
} TF_t;

/* Variables internas */
static uint8_t rx_byte;                           /**< Byte de recepción */
static uint8_t rx_buffer[LIDAR_FRAME_LEN];         /**< Buffer de recepción */
static uint8_t rx_index = 0;                       /**< Índice del buffer de recepción */
static bool frame_present;                         /**< Indica si una trama completa está disponible */

volatile TF_t lidar = {0};                         /**< Variable global de estado del sensor */

/* Prototipos de funciones privadas */
bool TFLC02_Parse_Packet(void);
bool ParserInfo(uint8_t *buffer, uint8_t size);
void TFLC02_Send_Command(uint8_t cmd);

/**
 * @brief Envía un comando al sensor.
 * @param cmd Comando a enviar (ver @ref TF_CMD)
 */
void TFLC02_Send_Command(uint8_t cmd) {
    assert(cmd >= Measure && cmd <= Get_Prod_info);

    uint8_t frame[5] = {0x55, 0xAA, cmd, 0x00, 0xFA};
    TFLC02_Port_Transmit(frame, sizeof(frame), HAL_MAX_DELAY);
}


/**
 * @brief Inicia la recepción UART por interrupción.
 */
void TFLC02_Start(void){
    TFLC02_Port_Receive_IT(&rx_byte, 1);
}

/**
 * @brief Solicita una medición de distancia al sensor.
 */
void TFLC02_Mesure(void){
    TFLC02_Send_Command(Measure);
}

/**
 * @brief Solicita información del producto al sensor.
 */
void TFLC02_Info(void){
    TFLC02_Send_Command(Get_Prod_info);
}

/**
 * @brief Solicita valores de fábrica del sensor.
 */
void TFLC02_DefaultSettings(void){
    TFLC02_Send_Command(Get_Factory_default_settings);
}

/**
 * @brief Verifica si la respuesta del sensor está completa.
 * @return true si hay nueva respuesta lista, false si no.
 */
bool TFLC02_RspComplete(void){
    if(lidar.receiveComplete){
        lidar.receiveComplete = false;
        return true;
    }
    else{
        return false;
    }
}

/**
 * @brief Obtiene la última distancia medida.
 * @return Distancia en milímetros.
 */
uint16_t TFLC02_GetDistance(void) {
    return lidar.distance;
}

/**
 * @brief Obtiene el tipo de puerto configurado en el sensor.
 * @return Tipo de puerto (ver @ref TF_PORT).
 */
uint8_t TFLC02_GetPort(void) {
    assert(lidar.port == ONLY_I2C || lidar.port == ONLY_UART || lidar.port == UART_AND_I2C);
    return lidar.port;
}

/**
 * @brief Obtiene la versión de firmware del sensor.
 * @return Versión de firmware.
 */
uint8_t TFLC02_GetFirm(void) {
    return lidar.firmware;
}

/**
 * @brief Obtiene el estado de calibración del sensor.
 * @return Estado de calibración (ver @ref TF_CALIBRATE).
 */
uint8_t TFLC02_GetCalib(void) {
    assert(lidar.calib >= NO_CALIBRATE && lidar.calib <= COMPLETE_CALIBRATE);
    return lidar.calib;
}

/**
 * @brief Verifica si existe una trama pendiente para procesar.
 * @return true si existe una nueva trama, false en caso contrario.
 */
bool TFLC02_FramePresent(void) {
    if(frame_present){
        frame_present = false;
        return true;
    }
    return false;
}

/**
 * @brief Callback de recepción de UART.
 * @param huart Puntero a la estructura UART_HandleTypeDef.
 * @note Esta función debe ser llamada dentro del callback de recepción de HAL.
 */
void TFLC02__RxCpltCallback(UART_HandleTypeDef *huart) {
    assert(huart != NULL);

    if (huart->Instance == UART4) {
        rx_buffer[rx_index] = rx_byte;
        rx_index++;

        if(rx_index >= LIDAR_FRAME_LEN){
            rx_index = 0;
            frame_present = false;
        }

        if(rx_byte == LIDAR_FRAME_END){
            frame_present = true;
        }

        TFLC02_Start(); // Reinicia la recepción
    }
}

/**
 * @brief Máquina de estados que verifica la integridad de la trama recibida.
 * @return true si la trama fue validada y procesada correctamente, false en caso contrario.
 */
bool TFLC02_Parse_Packet(void) {
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
                } else {
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
                expected_length = byte;
                if ((expected_length + 5) <= LIDAR_FRAME_LEN) {
                    parser_state = (expected_length > 0) ? STATE_DATA : STATE_END;
                    byte_index++;
                } else {
                    parser_state = STATE_WAIT_HEADER_1;
                    rx_index = 0;
                }
                break;

            case STATE_DATA:
                byte_index++;
                if (byte_index == (4 + expected_length)) {
                    parser_state = STATE_END;
                }
                break;

            case STATE_END:
                if (byte == LIDAR_FRAME_END) {
                    if (ParserInfo(rx_buffer, byte_index + 1)) {
                        lidar.receiveComplete = true;
                        rx_index = 0;
                        return true;
                    } else {
                        lidar.receiveComplete = false;
                        rx_index = 0;
                        return false;
                    }
                }
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

/**
 * @brief Procesa la trama recibida según el comando.
 * @param buffer Puntero al buffer de datos recibidos.
 * @param size Tamaño del buffer.
 * @return true si el paquete fue válido y procesado correctamente, false en caso contrario.
 */
bool ParserInfo(uint8_t *buffer, uint8_t size) {
    static uint8_t cmd;
    cmd = buffer[2];

    switch(cmd) {
        case Measure:
            lidar.distance = (rx_buffer[4] << 8) | rx_buffer[5];
            lidar.errorCode = rx_buffer[6];
            break;

        case TFLC02_Reset:
            // No hay payload
            break;

        case Get_Factory_default_settings:
            lidar.offset_short1 = rx_buffer[4];
            lidar.offset_short2 = rx_buffer[5];
            lidar.offset_long1 = rx_buffer[6];
            lidar.offset_long2 = rx_buffer[7];
            lidar.crosstalk = (rx_buffer[8] << 8) | rx_buffer[9];
            lidar.calib = rx_buffer[10];
            break;

        case Get_Prod_info:
            lidar.type = rx_buffer[4];
            lidar.port = rx_buffer[5];
            lidar.firmware = rx_buffer[6];
            break;

        default:
            return false;
    }

    return true;
}
