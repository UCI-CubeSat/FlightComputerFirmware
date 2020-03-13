#include "logger.h"
#include "main.h"
#include "cmsis_os.h"

void system_init() {
    HAL_Init();
    SystemClock_Config();
    osKernelInitialize();
}

void init_modules() {
    logger_init(Error_Handler);
}

int main(void) {
    system_init();
    init_modules();

    osKernelStart();

    while (1) {}
}

void Error_Handler(void) {
}

