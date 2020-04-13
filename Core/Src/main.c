#include "logger.h"
#include "main.h"
#include "cmsis_os.h"
#include "IMU.h"

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
    imu_init(Error_Handler);
}

void main_thread(void* args) {
    IMU_Angles angles;

    while (1) {
        imu_read_euler(&angles);
        logger_send_imu(angles.h, angles.p, angles.r);
        HAL_Delay(50);
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
    while (1) {

    }
}

