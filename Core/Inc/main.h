#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f1xx_hal.h"

void Error_Handler(void);
void SystemClock_Config(void);

#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC

#endif

