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
#define WATER_TASK_PRIORITY						( tskIDLE_PRIORITY + 4UL )
#define WATER_TASK_STACK						(2048/4) // 2048 bytes

#define SUPPORT_WATER_TEST_COMMAND				0 // debug command for FreeRTOS-CLI
#define DBG_WATER								0

// Pin mapping
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

#define CONTROLLER_QUEUE_SIZE					8

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
int16_t m_nextActiveController = -1;
WATER_ControllerTypeDef m_Controller[MAX_WATER_CONTROLLER_NUM] =
{
	{ 7, 0, 0, 200},
	{ 7, 0, 0, 200},
	{ 7, 0, 0, 200},
	{ 7, 0, 0, 200},
};

static SemaphoreHandle_t m_waterLock;
static QueueHandle_t m_cotrollerQueue;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
 *
 * @return
 */
int16_t WATER_FindNextActiveController(void)
{
	RTC_TimeTypeDef sTime;
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

	RTC_DateTypeDef sDate;
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);


	int16_t ret = -1;
	int32_t minTimeDiff = 0x7fffffff;
	int32_t timeDiff;
	int32_t curTime;

	curTime = sTime.Hours * 60 * 60 + sTime.Minutes * 60 + sTime.Seconds + 1;

	int i;
	for(i = 0; i < MAX_WATER_CONTROLLER_NUM; i++)
	{
		if (m_Controller[i].Period == 0) continue;

		timeDiff = m_Controller[i].Hour * 60 * 60 + m_Controller[i].Minutes * 60 + i * 10 - curTime;

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
 * @brief Update alarm when clock is changed or alarm is actived
 */
void WATER_UpdateNextActiveController(void)
{
	m_nextActiveController = WATER_FindNextActiveController();

	HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);

	if( m_nextActiveController != -1 )
	{
		RTC_AlarmTypeDef sAlarm;
		sAlarm.AlarmTime.Hours = m_Controller[m_nextActiveController].Hour;
		sAlarm.AlarmTime.Minutes = m_Controller[m_nextActiveController].Minutes;
		sAlarm.AlarmTime.Seconds = m_nextActiveController * 10;
		sAlarm.AlarmTime.SubSeconds = 0;
		sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
		sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
		sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
		sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
		sAlarm.AlarmDateWeekDay = RTC_WEEKDAY_MONDAY;
		sAlarm.Alarm = RTC_ALARM_A;

		HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN);

		HAL_RTC_GetAlarm(&hrtc, &sAlarm, RTC_ALARM_A, RTC_FORMAT_BIN);
		DEBUG_printf(DBG_WATER, "set alarm controller %d @ %2d:%02d:%02d\n", (int)m_nextActiveController, sAlarm.AlarmTime.Hours,
				sAlarm.AlarmTime.Minutes, sAlarm.AlarmTime.Seconds);
	}
}

/**
 *
 * @return
 */
int16_t WATER_GetNextActiveController(void)
{
	return m_nextActiveController;
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

	WATER_UpdateNextActiveController();
}

/**
 *
 * @return
 */
bool WATER_TryLock(void)
{
	if( xSemaphoreTake(m_waterLock, 0) )
		return TRUE;
	else
		return FALSE;
}

/**
 *
 */
void WATER_Unlock(void)
{
	xSemaphoreGive(m_waterLock);
}

/**
 *
 * @param num
 */
void WATER_AlarmHandler(void)
{
	if (m_nextActiveController != -1)
		xQueueSend(m_cotrollerQueue, &m_nextActiveController, 0);
}

/**
 * @brief Configure all pin
 */
void WATER_EnableController(uint8_t num)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	switch (num)
	{
		case 0:
			GPIO_InitStruct.Pin = WATER_PIN_0_NUM;
			HAL_GPIO_Init(WATER_PIN_0_PORT, &GPIO_InitStruct);
			HAL_GPIO_WritePin(WATER_PIN_0, GPIO_PIN_SET);
			break;

		case 1:
			GPIO_InitStruct.Pin = WATER_PIN_1_NUM;
			HAL_GPIO_Init(WATER_PIN_1_PORT, &GPIO_InitStruct);
			HAL_GPIO_WritePin(WATER_PIN_1, GPIO_PIN_SET);
			break;

		case 2:
			GPIO_InitStruct.Pin = WATER_PIN_2_NUM;
			HAL_GPIO_Init(WATER_PIN_2_PORT, &GPIO_InitStruct);
			HAL_GPIO_WritePin(WATER_PIN_2, GPIO_PIN_SET);
			break;

		case 3:
			GPIO_InitStruct.Pin = WATER_PIN_3_NUM;
			HAL_GPIO_Init(WATER_PIN_3_PORT, &GPIO_InitStruct);
			HAL_GPIO_WritePin(WATER_PIN_3, GPIO_PIN_SET);
			break;
	}
}

/**
 * @brief configure all pins to analog mode for power consumption
 */
void WATER_DisableController(uint8_t num)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;

	switch (num)
	{
		case 0:
			HAL_GPIO_WritePin(WATER_PIN_0, GPIO_PIN_RESET);
			GPIO_InitStruct.Pin = WATER_PIN_0_NUM;
			HAL_GPIO_Init(WATER_PIN_0_PORT, &GPIO_InitStruct);
			break;

		case 1:
			HAL_GPIO_WritePin(WATER_PIN_1, GPIO_PIN_RESET);
			GPIO_InitStruct.Pin = WATER_PIN_1_NUM;
			HAL_GPIO_Init(WATER_PIN_1_PORT, &GPIO_InitStruct);
			break;

		case 2:
			HAL_GPIO_WritePin(WATER_PIN_2, GPIO_PIN_RESET);
			GPIO_InitStruct.Pin = WATER_PIN_2_NUM;
			HAL_GPIO_Init(WATER_PIN_2_PORT, &GPIO_InitStruct);
			break;

		case 3:
			HAL_GPIO_WritePin(WATER_PIN_3, GPIO_PIN_RESET);
			GPIO_InitStruct.Pin = WATER_PIN_3_NUM;
			HAL_GPIO_Init(WATER_PIN_3_PORT, &GPIO_InitStruct);
			break;
	}
}

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

static BaseType_t WATER_ActiveCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
	const char *parameterPtr;
	int32_t paramterLen;
	uint32_t num;

	parameterPtr = FreeRTOS_CLIGetParameter(pcCommandString, 1, &paramterLen);
	num = DecToInt((char *) parameterPtr, paramterLen);

	WATER_AlarmHandler(num);

	sprintf(pcWriteBuffer, "\n");

	return pdFALSE;

}

static const CLI_Command_Definition_t xWaterActive =
{
	"wact",
	"wact <n>:\n    active controller <n>\n",
	WATER_ActiveCommand,
	1
};

#endif

/**
 * @brief Water task for valve controlling
 * @param pvParameters
 */
static void WATER_Task( void *pvParameters )
{
	uint8_t activedController;
	while(1)
	{
		xQueueReceive(m_cotrollerQueue, &activedController, portMAX_DELAY);

		xSemaphoreTake(m_waterLock, portMAX_DELAY);

		WATER_EnableController(activedController);
		DEBUG_printf(DBG_WATER, "Enable controller %d, %d sec\n", (int)activedController, m_Controller[activedController].Period);
		vTaskDelay(m_Controller[activedController].Period * 1000 );

		WATER_DisableController(activedController);
		DEBUG_printf(DBG_WATER, "Disable controller %d\n", (int)activedController);

		xSemaphoreGive(m_waterLock);
	}
}

/**
 * @brief Configure hardware and create tasks
 */
void WATER_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	m_waterLock = xSemaphoreCreateMutex();
	m_cotrollerQueue = xQueueCreate(CONTROLLER_QUEUE_SIZE, sizeof(int16_t));

	WATER_UpdateNextActiveController();

#if SUPPORT_WATER_TEST_COMMAND
	FreeRTOS_CLIRegisterCommand(&xWaterSet);
	FreeRTOS_CLIRegisterCommand(&xWaterActive);
#endif

	xTaskCreate(WATER_Task, "WATER", WATER_TASK_STACK, NULL, WATER_TASK_PRIORITY, NULL);

}
