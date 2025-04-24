# Trabajo practico final PdM



## Funcionalidad

El proyecto consiste en el desarrollo de un sistema embebido que corre en una placa Nucleo F446RE (con microcontrolador STM32F446RE). El sistema utiliza un sensor LiDAR TF-LC02 conectado mediante UART y una pantalla OLED SSD1306 mediante I2C. El objetivo es medir distancias en tiempo real y mostrarlas en la pantalla.

La frecuencia de muestreo es configurable por el usuario utilizando un pulsador integrado en la placa. Las opciones disponibles son 50, 250, 500 y 1000 ms. En cada medición, el sistema cambia el estado del LED incorporado en la placa para ofrecer una indicación visual del ritmo de muestreo.


### SSD1306 (Display OLED)

- Inicialización de pantalla
- Impresión de texto mediante fuente 5x7
- Manejo de cursor
- Visualización de datos (distancia, estado, muestreo)
- Capa de puerto adaptada a HAL I2C de STM32

### TF-LC02 (Sensor LiDAR)

- Comunicación UART con interrupciones
- Envío de comandos y recepción de tramas
- Máquina de estados para parseo de datos
- Acceso a distancia medida, puertos y configuración


## Requisitos

- STM32 HAL (I2C y UART)
- Pantalla SSD1306 (128x64, I2C)
- Sensor TF-LC02 (UART 115200 bps)
