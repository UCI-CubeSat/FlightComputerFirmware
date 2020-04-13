#ifndef CUBESATUART_IMU_H
#define CUBESATUART_IMU_H

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "bno055.h"

typedef struct bno055_euler_t IMU_Angles;

void imu_init(void (*error_handler) (void));
void imu_read_euler(IMU_Angles *angles);

#endif
