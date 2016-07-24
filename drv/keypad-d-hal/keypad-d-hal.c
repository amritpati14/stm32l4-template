/**
  ******************************************************************************
  * @file    keypad-d-hal.c
  * @author  dinow
  * @version V0.0.1
  * @date    2016-07-24
  * @brief
  ******************************************************************************
  * @attention
  *
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "keypad-d-hal.h"

/* Private typedef -----------------------------------------------------------*/
#define SUPPORT_KEY_TEST_COMMAND			0 // debug command for FreeRTOS-CLI

#define KEY_UP_GPIO_PORT					GPIOD
#define KEY_UP_GPIO_PIN						GPIO_PIN_2
#define KEY_DOWN_GPIO_PORT					GPIOC
#define KEY_DOWN_GPIO_PIN					GPIO_PIN_10
#define KEY_LEFT_GPIO_PORT					GPIOC
#define KEY_LEFT_GPIO_PIN					GPIO_PIN_12
#define KEY_RIGHT_GPIO_PORT					GPIOC
#define KEY_RIGHT_GPIO_PIN					GPIO_PIN_11
#define KEY_A_GPIO_PORT						GPIOA
#define KEY_A_GPIO_PIN						GPIO_PIN_15
#define KEY_B_GPIO_PORT						GPIOB
#define KEY_B_GPIO_PIN						GPIO_PIN_7

#define KEY_UP_PIN							KEY_UP_GPIO_PORT, KEY_UP_GPIO_PIN
#define KEY_DOWN_PIN						KEY_DOWN_GPIO_PORT, KEY_DOWN_GPIO_PIN
#define KEY_LEFT_PIN						KEY_LEFT_GPIO_PORT, KEY_LEFT_GPIO_PIN
#define KEY_RIGHT_PIN						KEY_RIGHT_GPIO_PORT, KEY_RIGHT_GPIO_PIN
#define KEY_A_PIN							KEY_A_GPIO_PORT, KEY_A_GPIO_PIN
#define KEY_B_PIN							KEY_B_GPIO_PORT, KEY_B_GPIO_PIN

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

Key_t KEY_GetKey(void)
{
	if (HAL_GPIO_ReadPin(KEY_A_PIN) == GPIO_PIN_RESET)
		return Key_A;
	if (HAL_GPIO_ReadPin(KEY_B_PIN) == GPIO_PIN_RESET)
		return Key_B;

	if (HAL_GPIO_ReadPin(KEY_UP_PIN) == GPIO_PIN_RESET)
		return Key_Up;
	if (HAL_GPIO_ReadPin(KEY_DOWN_PIN) == GPIO_PIN_RESET)
		return Key_Down;

	if (HAL_GPIO_ReadPin(KEY_LEFT_PIN) == GPIO_PIN_RESET)
		return Key_Left;
	if (HAL_GPIO_ReadPin(KEY_RIGHT_PIN) == GPIO_PIN_RESET)
		return Key_Right;

	return Key_None;
}

/**
 * @brief Configure clock and pins for keypad
 */
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;

	GPIO_InitStruct.Pin = KEY_UP_GPIO_PIN;
	HAL_GPIO_Init(KEY_UP_GPIO_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = KEY_DOWN_GPIO_PIN;
	HAL_GPIO_Init(KEY_DOWN_GPIO_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = KEY_LEFT_GPIO_PIN;
	HAL_GPIO_Init(KEY_LEFT_GPIO_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = KEY_RIGHT_GPIO_PIN;
	HAL_GPIO_Init(KEY_RIGHT_GPIO_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = KEY_A_GPIO_PIN;
	HAL_GPIO_Init(KEY_A_GPIO_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = KEY_B_GPIO_PIN;
	HAL_GPIO_Init(KEY_B_GPIO_PORT, &GPIO_InitStruct);


#if SUPPORT_KEY_TEST_COMMAND
	KEY_Test();
#endif
}
