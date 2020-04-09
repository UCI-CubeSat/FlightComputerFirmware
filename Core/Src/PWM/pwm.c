/*
 * pwm.c
 *
 *  Created on: Apr 5, 2020
 *  Author: Adrian Osorio
 */

// Includes
#include "pwm.h"

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void GPIOE_Init(void) {
  /* GPIO Ports Clock Enable For Port E */
  __HAL_RCC_GPIOE_CLK_ENABLE();
}

/**
 * TODO Duty cycle can be changed
 * */ // tim_td = &TIM1, tim_htp = &htim1, channel = TIM_CHANNEL_1
void TIM_PWM_Init_50HZ(TIM_HandleTypeDef *tim_HTP, TIM_TypeDef *tim_TD, uint32_t channel) {

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};
	// Clock Frequency Configuration
	tim_HTP->Instance = tim_TD;
	tim_HTP->Init.Prescaler = 16;
	tim_HTP->Init.CounterMode = TIM_COUNTERMODE_UP;
	tim_HTP->Init.Period = 20000;
	tim_HTP->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim_HTP->Init.RepetitionCounter = 0;
	tim_HTP->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_Base_Init(tim_HTP) != HAL_OK)
	{
	 Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(tim_HTP, &sClockSourceConfig) != HAL_OK)
	{
	 Error_Handler();
	}
	if (HAL_TIM_PWM_Init(tim_HTP) != HAL_OK)
	{
	 Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(tim_HTP, &sMasterConfig) != HAL_OK)
	{
	 Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 16;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(tim_HTP, &sConfigOC, channel) != HAL_OK)
	{
	 Error_Handler();
	}
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(tim_HTP, &sBreakDeadTimeConfig) != HAL_OK)
	{
	 Error_Handler();
	}

	HAL_TIM_MspPostInit(tim_HTP);

}

/**
  * @brief  This function is execut  ed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}
