/**
  ******************************************************************************
  * @file    calendar.c
  * @author  dinow
  * @version V0.0.1
  * @date    2017-03-01
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
#include "calendar.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define SUPPORT_RTC_TEST_COMMAND			0 // debug command for FreeRTOS-CLI

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

#if SUPPORT_RTC_TEST_COMMAND
/**
 * @brief Show RTC time
 * @param pcWriteBuffer
 * @param xWriteBufferLen
 * @param pcCommandString
 * @return
 */
static BaseType_t CALENDAR_GetTimeCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
	RTC_TimeTypeDef sTime;
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

	RTC_DateTypeDef sDate;
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	sprintf(pcWriteBuffer, "System Time: %02d:%02d:%02d\n",
			sTime.Hours, sTime.Minutes,
			sTime.Seconds);
	return pdFALSE;
}

static const CLI_Command_Definition_t xCalendarGetTime =
{
	"get-time", /* The command string to type. */
	"get-time:\n    Displays current time of system\n",
	CALENDAR_GetTimeCommand, /* The function to run. */
	0 /* No parameters are expected. */
};

/**
 * @brief Set RTC time
 * @param pcWriteBuffer
 * @param xWriteBufferLen
 * @param pcCommandString
 * @return
 */
static BaseType_t CALENDAR_SetTimeCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
	uint32_t hours, minutes, seconds;

	const char *parameterPtr;
	int32_t paramterLen;

	parameterPtr = FreeRTOS_CLIGetParameter(pcCommandString, 1, &paramterLen);
	if (paramterLen > 3 && parameterPtr[0] == '0' && parameterPtr[1] == 'x')
		hours = HexToInt((char *) parameterPtr, paramterLen);
	else
		hours = DecToInt((char *) parameterPtr, paramterLen);

	parameterPtr = FreeRTOS_CLIGetParameter(pcCommandString, 2, &paramterLen);
	if (paramterLen > 3 && parameterPtr[0] == '0' && parameterPtr[1] == 'x')
		minutes = HexToInt((char *) parameterPtr, paramterLen);
	else
		minutes = DecToInt((char *) parameterPtr, paramterLen);

	parameterPtr = FreeRTOS_CLIGetParameter(pcCommandString, 3, &paramterLen);
	if (paramterLen > 3 && parameterPtr[0] == '0' && parameterPtr[1] == 'x')
		seconds = HexToInt((char *) parameterPtr, paramterLen);
	else
		seconds = DecToInt((char *) parameterPtr, paramterLen);

	sprintf(pcWriteBuffer, "Set system time to %02ld:%02ld:%02ld\n", hours, minutes,
			seconds);

	RTC_TimeTypeDef sTime;
	sTime.Hours = hours;
	sTime.Minutes = minutes;
	sTime.Seconds = seconds;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

	return pdFALSE;
}

static const CLI_Command_Definition_t xCalendarSetTime =
{
	"set-time", /* The command string to type. */
	"set-time: <hour> <minute> <second>\n    Set time of system\n",
	CALENDAR_SetTimeCommand, /* The function to run. */
	3 /* No parameters are expected. */
};

/**
 * @brief Show alarm
 * @param pcWriteBuffer
 * @param xWriteBufferLen
 * @param pcCommandString
 * @return
 */
static BaseType_t CALENDAR_GetAlarmCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
	RTC_AlarmTypeDef sAlarm;

	HAL_RTC_GetAlarm(&hrtc, &sAlarm, RTC_ALARM_A, RTC_FORMAT_BIN);

	sprintf(pcWriteBuffer, "Alarm: %02d:%02d:%02d\n",
			sAlarm.AlarmTime.Hours, sAlarm.AlarmTime.Minutes,
			sAlarm.AlarmTime.Seconds);

	return pdFALSE;

}

static const CLI_Command_Definition_t xCalendarGetAlarm =
{
	"get-alarm", /* The command string to type. */
	"get-alarm:\n    Displays alarm time of system\n",
	CALENDAR_GetAlarmCommand, /* The function to run. */
	0 /* No parameters are expected. */
};

/**
 * @brief set alarm
 * @param pcWriteBuffer
 * @param xWriteBufferLen
 * @param pcCommandString
 * @return
 */
static BaseType_t CALENDAR_SetAlarmCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
	HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);

	uint32_t hours, minutes, seconds;

	const char *parameterPtr;
	int32_t paramterLen;

	parameterPtr = FreeRTOS_CLIGetParameter(pcCommandString, 1, &paramterLen);
	if (paramterLen > 3 && parameterPtr[0] == '0' && parameterPtr[1] == 'x')
		hours = HexToInt((char *) parameterPtr, paramterLen);
	else
		hours = DecToInt((char *) parameterPtr, paramterLen);

	parameterPtr = FreeRTOS_CLIGetParameter(pcCommandString, 2, &paramterLen);
	if (paramterLen > 3 && parameterPtr[0] == '0' && parameterPtr[1] == 'x')
		minutes = HexToInt((char *) parameterPtr, paramterLen);
	else
		minutes = DecToInt((char *) parameterPtr, paramterLen);

	parameterPtr = FreeRTOS_CLIGetParameter(pcCommandString, 3, &paramterLen);
	if (paramterLen > 3 && parameterPtr[0] == '0' && parameterPtr[1] == 'x')
		seconds = HexToInt((char *) parameterPtr, paramterLen);
	else
		seconds = DecToInt((char *) parameterPtr, paramterLen);

	RTC_AlarmTypeDef sAlarm;
	sAlarm.AlarmTime.Hours = hours;
	sAlarm.AlarmTime.Minutes = minutes;
	sAlarm.AlarmTime.Seconds = seconds;
	sAlarm.AlarmTime.SubSeconds = 0;
	sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
	sAlarm.AlarmDateWeekDay = RTC_WEEKDAY_MONDAY;
	sAlarm.Alarm = RTC_ALARM_A;

	HAL_StatusTypeDef ret;
	ret = HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN);

	sprintf(pcWriteBuffer, "Set alarm to %02ld:%02ld:%02ld, ret = %d\n", hours, minutes,
			seconds, ret);

	return pdFALSE;
}

static const CLI_Command_Definition_t xCalendarSetAlarm =
{
	"set-alarm", /* The command string to type. */
	"set-alarm: <hour> <minute> <second>\n    Set alarm of system\n",
	CALENDAR_SetAlarmCommand, /* The function to run. */
	3 /* No parameters are expected. */
};
#endif

/**
 * @brief Initialize RTC
 */
void CALENDAR_Init(void)
{
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 127;
	hrtc.Init.SynchPrediv = 255;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

	HAL_StatusTypeDef ret;
	ret = HAL_RTC_Init(&hrtc);

	if ( ret != HAL_OK)
	{
		Error_Handler();
	}

	__HAL_RCC_SYSCFG_CLK_ENABLE();
	HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
#if SUPPORT_RTC_TEST_COMMAND
	FreeRTOS_CLIRegisterCommand( &xCalendarGetTime );
	FreeRTOS_CLIRegisterCommand( &xCalendarSetTime );
	FreeRTOS_CLIRegisterCommand( &xCalendarGetAlarm );
	FreeRTOS_CLIRegisterCommand( &xCalendarSetAlarm );
#endif
}

void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
	if (hrtc->Instance == RTC)
	{
		/* USER CODE BEGIN RTC_MspInit 0 */

		/* USER CODE END RTC_MspInit 0 */

		/* Peripheral clock enable */
		__HAL_RCC_RTC_ENABLE();

		/* USER CODE BEGIN RTC_MspInit 1 */

		/* USER CODE END RTC_MspInit 1 */
	}

}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* hrtc)
{
	if (hrtc->Instance == RTC)
	{
		/* USER CODE BEGIN RTC_MspDeInit 0 */

		/* USER CODE END RTC_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_RTC_DISABLE();
	}
	/* USER CODE BEGIN RTC_MspDeInit 1 */

	/* USER CODE END RTC_MspDeInit 1 */

}
