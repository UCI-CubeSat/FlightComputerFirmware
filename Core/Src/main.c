#include "logger.h"
#include "main.h"
#include "cmsis_os.h"

osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
        .name = "defaultTask",
        .priority = (osPriority_t) osPriorityNormal,
        .stack_size = 128 * 4
};


void StartDefaultTask(void *argument);

int main(void) {
    HAL_Init();
    SystemClock_Config();

    logger_init(Error_Handler);

    osKernelInitialize();

    defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

    osKernelStart();

    while (1) {}
}

void StartDefaultTask(void *argument) {
    for (;;) {
        logger_send_imu(30, 40, 50);
        osDelay(500);
    }
}


void Error_Handler(void) {
}

