/*
 * pwm.h
 *
 *  Created on: Apr 5, 2020
 *      Author: Adrian Osorio
 */

#ifndef __PWM_H_
#define __PWM_H_

/** Includes */
#include "stm32f4xx_hal.h"

// Public Functions

/** Currently Initializes Port E GPIO Timers */
void GPIOE_Init(void);

/** Initializes a PWM Signal For a Timer */
void TIM_PWM_Init_50HZ(TIM_HandleTypeDef *tim_HTP, TIM_TypeDef *tim_TD, uint32_t channel);

/** Error Handler Function */
void Error_Handler(void);


#endif /* INC_PWM_H_ */
