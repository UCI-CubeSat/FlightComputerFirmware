# Overview

This is the main firmware that will run onboard the STM32 flight computer. It's using FreeRTOS with the CMSIS-RTOS v2 wrapper. 

# Modules
## Logging

Logging is done through a USB-Serial converter connected to USART1 (pins PA9 and PA10). The configuration is 115200 baud 8N1. See the COSMOS config repo for how to interpret the packets.

### Usage
Call `logger_init(error_handler)` to initialize the module. Use `logger_send_imu(roll, pitch, yaw)` to log data. 

### Internals
This module creates a task, along with a queue for communication to that task. When `logger_send_imu` is called, the method pushes a new message onto the queue, from the caller thread. The push operation uses try semantics, meaning that if the queue is full, the operation will fail immediately.

The logger task takes messages off the queue, and transmits them. In the future, this task will also be used for receiving incoming messages. 

## IMU

The IMU we're using is a BNO055, which is a 9-axis IMU with a built-in Cortex-M0, to do sensor fusion onboard. It's connected via an I2C to I2C1 (pins PB6 and PB7).

### Usage
Call `imu_init(Error_Handler)` to initialize the module. Use `imu_read_euler(IMU_Angles*)` to get euler angles from the IMU.

### Internals
This module is acting as a thin wrapper around the [BNO055 Driver][BNO055] 

[BNO055]:
https://github.com/BoschSensortec/BNO055_driver

# Building and running

The code can be built and flashed using STM32CubeIDE. 

In addition, I'm using CLion as my development environment, which automatically generated a CMake file. This can be used to compile the firmware. To flash it, use your preferred tool. In my case, CLion includes support for openocd. The ST-LINK utility also works. 

# Useful Resources
- [CMSIS-RTOS v2][RTOS]
- [HAL Reference][HAL]
- [STM32F103xx Reference Manual][REF]
- [Cortex M3 Programming Manual][CTX]


[CTX]: https://www.st.com/content/ccc/resource/technical/document/programming_manual/5b/ca/8d/83/56/7f/40/08/CD00228163.pdf/files/CD00228163.pdf/jcr:content/translations/en.CD00228163.pdf

[HAL]: https://www.st.com/content/ccc/resource/technical/document/user_manual/72/52/cc/53/05/e3/4c/98/DM00154093.pdf/files/DM00154093.pdf/jcr:content/translations/en.DM00154093.pdf

[REF]: https://www.st.com/content/ccc/resource/technical/document/reference_manual/59/b9/ba/7f/11/af/43/d5/CD00171190.pdf/files/CD00171190.pdf/jcr:content/translations/en.CD00171190.pdf

[RTOS]: http://www.keil.com/pack/doc/CMSIS_Dev/RTOS2/html/index.html


