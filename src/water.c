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
#include "water.h"
#include "calendar.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define WATER_TASK_PRIORITY					( tskIDLE_PRIORITY + 3UL )
#define WATER_TASK_STACK					( 256/4 ) 							// 2048 bytes

#define SUPPORT_WATER_TEST_COMMAND				0 // debug command for FreeRTOS-CLI

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
WATER_ControllerTypeDef m_Controller[MAX_WATER_CONTROLLER_NUM] =
{
	{ 7, 0, 10, 200},
	{ 7, 10, 0, 200},
	{ 7, 20, 0, 200},
	{ 7, 30, 0, 200},
};

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
 *
 * @return
 */
int16_t WATER_GetNextActiveController(void)
{
	RTC_TimeTypeDef sTime;
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

	RTC_DateTypeDef sDate;
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);


	int16_t ret = -1;
	int32_t minTimeDiff = 0x7fffffff;
	int32_t timeDiff;
	int32_t curTime;

	curTime = sTime.Hours * 60 * 60 + sTime.Minutes * 60 + sTime.Seconds;

	int i;
	for(i = 0; i < MAX_WATER_CONTROLLER_NUM; i++)
	{
		if (m_Controller[i].Period == 0) continue;

		timeDiff = m_Controller[i].Hour * 60 * 60 + m_Controller[i].Minutes * 60 - curTime;

		if (timeDiff < 0) timeDiff += 24 * 60 * 60;

		if ( timeDiff < minTimeDiff)
		{
			minTimeDiff = timeDiff;
			ret = i;
		}
	}

	return ret;
}


/**
 * @brief Get watering setting of controller No.x
 * @param num
 * @param sController
 */
void WATER_GetController(uint8_t num, WATER_ControllerTypeDef *sController)
{
	if (num < MAX_WATER_CONTROLLER_NUM)
	{
		sController->Hour = m_Controller[num].Hour;
		sController->Minutes = m_Controller[num].Minutes;
		sController->Period = m_Controller[num].Period;
		sController->Moisture = m_Controller[num].Moisture;
	}
}

/**
 * @brief Set watering setting of controller No.x
 * @param num
 * @param sController
 */
void WATER_SetController(uint8_t num, WATER_ControllerTypeDef *sController)
{
	if( num < MAX_WATER_CONTROLLER_NUM )
	{
		m_Controller[num].Hour = sController->Hour;
		m_Controller[num].Minutes = sController->Minutes;
		m_Controller[num].Period = sController->Period;
		m_Controller[num].Moisture = sController->Moisture;
	}

	// TODO: find next watering controller and set alarm
}
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
 * @brief configure all pins to analog mode for power consumption
 */
void WATER_Disable(void)
{
	HAL_GPIO_WritePin(WATER_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(WATER_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(WATER_PIN_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(WATER_PIN_3, GPIO_PIN_RESET);

	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;

	GPIO_InitStruct.Pin = WATER_PIN_0_NUM;
	HAL_GPIO_Init(WATER_PIN_0_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = WATER_PIN_1_NUM;
	HAL_GPIO_Init(WATER_PIN_1_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = WATER_PIN_2_NUM;
	HAL_GPIO_Init(WATER_PIN_2_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = WATER_PIN_3_NUM;
	HAL_GPIO_Init(WATER_PIN_3_PORT, &GPIO_InitStruct);
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
