#include <cmsis_os2.h>
#include "logger.h"

UART_HandleTypeDef huart1;

uint8_t buf[] = {1, 2, 3};

void logger_init(void (*error_handler)(void)) {
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


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

void USART1_IRQHandler(void) {
    HAL_UART_IRQHandler(&huart1);
}

void send_packet(void *packet, uint16_t size) {
    HAL_UART_Transmit_IT(&huart1, buf, 3);
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

