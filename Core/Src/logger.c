#include "logger.h"

UART_HandleTypeDef huart1;

void logger_init(void (*error_handler) (void)) {
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK) {
        error_handler();
    }
}

static void send_packet(void *packet, uint16_t size) {
    HAL_UART_Transmit(&huart1, (uint8_t*) packet, size, LOGGER_TIMEOUT);
}

void logger_send_imu(uint16_t roll, uint16_t pitch, uint16_t yaw) {
    IMU_Packet packet = {
            .opcode = IMU_OPCODE,
            .roll = roll,
            .pitch = pitch,
            .yaw = yaw
    };

    send_packet(&packet, sizeof(IMU_Packet));
}

