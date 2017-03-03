/**
  ******************************************************************************
  * @file    water.c
  * @author  dinow
  * @version V0.0.1
  * @date    2017-03-03
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

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define WATER_TASK_PRIORITY					( tskIDLE_PRIORITY + 3UL )
#define WATER_TASK_STACK					( 256/4 ) 							// 2048 bytes

#define SUPPORT_WATER_TEST_COMMAND				1 // debug command for FreeRTOS-CLI

#define WATER_PIN_0_PORT						GPIOB
#define WATER_PIN_0_NUM							GPIO_PIN_11
#define WATER_PIN_1_PORT						GPIOB
#define WATER_PIN_1_NUM							GPIO_PIN_12
#define WATER_PIN_2_PORT						GPIOA
#define WATER_PIN_2_NUM							GPIO_PIN_11
#define WATER_PIN_3_PORT						GPIOA
#define WATER_PIN_3_NUM							GPIO_PIN_12

#define WATER_PIN_0								WATER_PIN_0_PORT, WATER_PIN_0_NUM
#define WATER_PIN_1								WATER_PIN_1_PORT, WATER_PIN_1_NUM
#define WATER_PIN_2								WATER_PIN_2_PORT, WATER_PIN_2_NUM
#define WATER_PIN_3								WATER_PIN_3_PORT, WATER_PIN_3_NUM

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
#if SUPPORT_WATER_TEST_COMMAND
/**
 * @param	pcWriteBuffer
 * @param	xWriteBufferLen
 * @param	pcCommandString
 * @return
 */
static BaseType_t WATER_SetCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
	const char *parameterPtr;
	int32_t paramterLen;
	uint32_t num, value;

	parameterPtr = FreeRTOS_CLIGetParameter(pcCommandString, 1, &paramterLen);
	num = DecToInt((char *) parameterPtr, paramterLen);

	parameterPtr = FreeRTOS_CLIGetParameter(pcCommandString, 2, &paramterLen);
	value = DecToInt((char *) parameterPtr, paramterLen);

	if (value == 0)
		value = GPIO_PIN_RESET;
	else
		value = GPIO_PIN_SET;


	switch(num)
	{
		case 0:
			HAL_GPIO_WritePin(WATER_PIN_0, value);
			break;

		case 1:
			HAL_GPIO_WritePin(WATER_PIN_1, value);
			break;

		case 2:
			HAL_GPIO_WritePin(WATER_PIN_2, value);
			break;

		case 3:
			HAL_GPIO_WritePin(WATER_PIN_3, value);
			break;

		default:
			break;

	}
	sprintf(pcWriteBuffer, "\n");

	return pdFALSE;

}

static const CLI_Command_Definition_t xWaterSet =
{
	"wset",
	"wset <n> <on/off>:\n    set pin<n> to on/off\n",
	WATER_SetCommand,
	2
};
#endif

/**
 * @brief Configure all pin
 */
void WATER_Enable(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	GPIO_InitStruct.Pin = WATER_PIN_0_NUM;
	HAL_GPIO_Init(WATER_PIN_0_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = WATER_PIN_1_NUM;
	HAL_GPIO_Init(WATER_PIN_1_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = WATER_PIN_2_NUM;
	HAL_GPIO_Init(WATER_PIN_2_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = WATER_PIN_3_NUM;
	HAL_GPIO_Init(WATER_PIN_3_PORT, &GPIO_InitStruct);

	HAL_GPIO_WritePin(WATER_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(WATER_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(WATER_PIN_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(WATER_PIN_3, GPIO_PIN_RESET);
}

/**
 * @brief Configure hardware and create tasks
 */
void WATER_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	WATER_Enable();


#if SUPPORT_WATER_TEST_COMMAND
	FreeRTOS_CLIRegisterCommand(&xWaterSet);
#endif
}
