#include "logger.h"
#include "main.h"
#include "cmsis_os.h"

osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
        .name = "defaultTask",
        .priority = (osPriority_t) osPriorityNormal,
        .stack_size = 128 * 4
};

void system_init() {
    HAL_Init();
    SystemClock_Config();
    osKernelInitialize();
}

void init_modules() {
    logger_init(Error_Handler);
}

void main_thread(void* args) {
    while (1) {
        for (int i = 0; i < 255; i++) {
            logger_send_imu(255, i, i);
            osDelay(100);
        }
    }
}

int main(void) {
    system_init();
    init_modules();

    osThreadNew(main_thread, NULL, &defaultTask_attributes);

    osKernelStart();

    while (1) {}
}



void Error_Handler(void) {
}

