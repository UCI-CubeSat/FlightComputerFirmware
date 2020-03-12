#include "main.h"
#include "system_interrupts.h"
#include "FreeRTOS.h"
#include "task.h"

extern TIM_HandleTypeDef htim1;

void NMI_Handler(void) {
    __NOP();
}

void HardFault_Handler(void) {
    while (1) {
    }
}

void MemManage_Handler(void) {
    while (1) {
    }
}

void BusFault_Handler(void) {
    while (1) {
    }
}

void UsageFault_Handler(void) {
    while (1) {
    }
}

void DebugMon_Handler(void) {
    __NOP();
}

void TIM1_UP_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim1);
}
