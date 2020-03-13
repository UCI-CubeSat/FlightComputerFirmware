#ifndef CUBESATUART_LOGGER_H
#define CUBESATUART_LOGGER_H

#include "stm32f1xx_hal.h"
#include <cmsis_os2.h>
#include "system_interrupts.h"

#define IMU_OPCODE 0x1

#define LOGGER_QUEUE_COUNT 8
#define LOGGER_MAX_MSG_SIZE 8

typedef struct {
    uint8_t size;
    uint8_t packet[LOGGER_MAX_MSG_SIZE];
} Logger_Queue_Msg;

#pragma pack(1)
typedef struct {
    uint8_t opcode;
    uint16_t roll;
    uint16_t pitch;
    uint16_t yaw;
} IMU_Packet;

void logger_init(void (*error_handler) (void));
void logger_send_imu(uint16_t roll, uint16_t pitch, uint16_t yaw);


#endif
