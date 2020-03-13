#include "logger.h"
#include "string.h"

UART_HandleTypeDef huart1;
osMessageQueueId_t queue;

const osThreadAttr_t task_attributes = {
        .name = "logger_task",
        .priority = osPriorityNormal,
        .stack_size = 128 * 4};

void USART1_IRQHandler(void) {
    HAL_UART_IRQHandler(&huart1);
}

void tx_gpio_init() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void rx_gpio_init() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void gpio_init() {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    tx_gpio_init();
    rx_gpio_init();
}

void uart_init(void (*error_handler)(void)) {
    __HAL_RCC_USART1_CLK_ENABLE();

    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

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

void send_packet(uint8_t *packet, uint8_t size) {
    HAL_UART_Transmit_IT(&huart1, packet, size);
}

void thread(void* arguments) {
    Logger_Queue_Msg message;
    uint8_t *priority = NULL;

    while (1) {
        if (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY) continue;

        osMessageQueueGet(queue, &message, priority, osWaitForever);
        send_packet(message.packet, message.size);
    }
}

void logger_init(void (*error_handler)(void)) {
    gpio_init();
    uart_init(error_handler);

    queue = osMessageQueueNew(LOGGER_QUEUE_COUNT, sizeof(Logger_Queue_Msg), NULL);
    osThreadNew(thread, NULL, &task_attributes);
}

void queue_message(uint8_t *buf, uint8_t size) {
    Logger_Queue_Msg message;
    message.size = size;
    memcpy(message.packet, buf, size);
    osMessageQueuePut(queue, &message, osPriorityNormal, 0);
}

void logger_send_imu(uint16_t roll, uint16_t pitch, uint16_t yaw) {
    IMU_Packet packet = {.opcode = IMU_OPCODE, .roll = roll,
            .pitch = pitch, .yaw = yaw};
    queue_message((uint8_t *) &packet, sizeof(packet));
}

