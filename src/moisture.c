/**
  ******************************************************************************
  * @file    moisture.c
  * @author  dinow
  * @version V0.0.1
  * @date    2017-03-09
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
#define SUPPORT_MOISTURE_TEST_COMMAND			0 // debug command for FreeRTOS-CLI
#define DBG_MOISTURE							FALSE

#define MOISTURE_POWER_GPIO_PORT				GPIOC
#define MOISTURE_POWER_GPIO_PIN					GPIO_PIN_3
#define MOISTURE_POWER_PIN						MOISTURE_POWER_GPIO_PORT, MOISTURE_POWER_GPIO_PIN
#define MOISTURE_SENSOR0_GPIO_PORT				GPIOA
#define MOISTURE_SENSOR0_GPIO_PIN				GPIO_PIN_4
#define MOISTURE_SENSOR0_PIN					MOISTURE_SENSOR0_GPIO_PORT, MOISTURE_SENSOR0_GPIO_PIN
#define MOISTURE_SENSOR1_GPIO_PORT				GPIOB
#define MOISTURE_SENSOR1_GPIO_PIN				GPIO_PIN_0
#define MOISTURE_SENSOR1_PIN					MOISTURE_SENSOR1_GPIO_PORT, MOISTURE_SENSOR1_GPIO_PIN
#define MOISTURE_SENSOR2_GPIO_PORT				GPIOC
#define MOISTURE_SENSOR2_GPIO_PIN				GPIO_PIN_1
#define MOISTURE_SENSOR2_PIN					MOISTURE_SENSOR2_GPIO_PORT, MOISTURE_SENSOR2_GPIO_PIN
#define MOISTURE_SENSOR3_GPIO_PORT				GPIOC
#define MOISTURE_SENSOR3_GPIO_PIN				GPIO_PIN_0
#define MOISTURE_SENSOR3_PIN					MOISTURE_SENSOR3_GPIO_PORT, MOISTURE_SENSOR3_GPIO_PIN

#define MOISTURE_SENSOR_0						ADC_CHANNEL_9 // PA4
#define MOISTURE_SENSOR_1						ADC_CHANNEL_15 // PB0
#define MOISTURE_SENSOR_2						ADC_CHANNEL_2 // PC1
#define MOISTURE_SENSOR_3						ADC_CHANNEL_1 // PC0

#define MOISTURE_MAX_READ_ADC_TIMES				10
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
 *
 * @param hadc
 */
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	if (hadc->Instance == ADC1)
	{
		// Peripheral clock enable
		__HAL_RCC_ADC_CLK_ENABLE();

		// Configure sensor ADC pin
		GPIO_InitTypeDef GPIO_InitStruct;

		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Pin = MOISTURE_SENSOR0_GPIO_PIN;
		HAL_GPIO_Init(MOISTURE_SENSOR0_GPIO_PORT, &GPIO_InitStruct);
		GPIO_InitStruct.Pin = MOISTURE_SENSOR1_GPIO_PIN;
		HAL_GPIO_Init(MOISTURE_SENSOR1_GPIO_PORT, &GPIO_InitStruct);
		GPIO_InitStruct.Pin = MOISTURE_SENSOR2_GPIO_PIN;
		HAL_GPIO_Init(MOISTURE_SENSOR2_GPIO_PORT, &GPIO_InitStruct);
		GPIO_InitStruct.Pin = MOISTURE_SENSOR3_GPIO_PIN;
		HAL_GPIO_Init(MOISTURE_SENSOR3_GPIO_PORT, &GPIO_InitStruct);
	}

}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{

	if (hadc->Instance == ADC1)
	{
		// Peripheral clock disable
		__HAL_RCC_ADC_CLK_DISABLE();

		// Configure sensor ADC pin
		GPIO_InitTypeDef GPIO_InitStruct;

		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Pin = MOISTURE_SENSOR0_GPIO_PIN;
		HAL_GPIO_Init(MOISTURE_SENSOR0_GPIO_PORT, &GPIO_InitStruct);
		GPIO_InitStruct.Pin = MOISTURE_SENSOR1_GPIO_PIN;
		HAL_GPIO_Init(MOISTURE_SENSOR1_GPIO_PORT, &GPIO_InitStruct);
		GPIO_InitStruct.Pin = MOISTURE_SENSOR2_GPIO_PIN;
		HAL_GPIO_Init(MOISTURE_SENSOR2_GPIO_PORT, &GPIO_InitStruct);
		GPIO_InitStruct.Pin = MOISTURE_SENSOR3_GPIO_PIN;
		HAL_GPIO_Init(MOISTURE_SENSOR3_GPIO_PORT, &GPIO_InitStruct);
	}
}

/**
 *
 */
void MOISTURE_Enable(void)
{
	ADC_MultiModeTypeDef multimode;

	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	hadc1.Init.Resolution = ADC_RESOLUTION_10B;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc1.Init.LowPowerAutoWait = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.NbrOfDiscConversion = 1;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc1.Init.OversamplingMode = DISABLE;

	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}

	multimode.Mode = ADC_MODE_INDEPENDENT;

	if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
	{
		Error_Handler();
	}

    // Configure sensor power pin
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = MOISTURE_POWER_GPIO_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(MOISTURE_POWER_GPIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(MOISTURE_POWER_PIN, GPIO_PIN_SET);
}

/**
 *
 */
void MOISTURE_Disable(void)
{
	HAL_GPIO_WritePin(MOISTURE_POWER_PIN, GPIO_PIN_RESET);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Pin = MOISTURE_POWER_GPIO_PIN;
	HAL_GPIO_Init(MOISTURE_POWER_GPIO_PORT, &GPIO_InitStruct);

	if (HAL_ADC_DeInit(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 *
 * @param ch	select sensor
 */
void MOISTURE_SetChannel(int8_t ch)
{
	ADC_ChannelConfTypeDef sConfig;

	switch (ch)
	{
		case 0:
			sConfig.Channel = MOISTURE_SENSOR_0;
			break;

		case 1:
			sConfig.Channel = MOISTURE_SENSOR_1;
			break;
		case 2:
			sConfig.Channel = MOISTURE_SENSOR_2;
			break;
		case 3:
			sConfig.Channel = MOISTURE_SENSOR_3;
			break;

		default:
			sConfig.Channel = MOISTURE_SENSOR_0;
			break;
	}

	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_92CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;

	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

}

/**
 *
 * @param ch	select sensor
 * @return
 */
int16_t MOISTURE_Read(int8_t ch)
{
	int16_t ret = -1;
	int8_t i;

	for (i = 0; i < MOISTURE_MAX_READ_ADC_TIMES; i++)
	{
		vTaskDelay(10);

		MOISTURE_Enable();
		MOISTURE_SetChannel(ch);

		HAL_ADC_Start(&hadc1);

		if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
		{
			ret = HAL_ADC_GetValue(&hadc1);
			HAL_ADC_Stop(&hadc1);
			MOISTURE_Disable();
			break;
		}

		HAL_ADC_Stop(&hadc1);
		MOISTURE_Disable();
	}

	return ret;
}


#if SUPPORT_MOISTURE_TEST_COMMAND

/**
 * @brief Show RTC time
 * @param pcWriteBuffer
 * @param xWriteBufferLen
 * @param pcCommandString
 * @return
 */
static BaseType_t MOISTURE_ReadCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
#if 0
	MOISTURE_Enable();

	HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
    {
    	uint32_t value;
    	value = HAL_ADC_GetValue(&hadc1);
		sprintf(pcWriteBuffer, "ADC = %ld\n", value);
    }
    else
    {
		sprintf(pcWriteBuffer, "ADC Timeout!!!\n");
    }
    HAL_ADC_Stop(&hadc1);

    MOISTURE_Disable();
#endif

    const char *parameterPtr;
	int32_t paramterLen;
	uint32_t ch;

	parameterPtr = FreeRTOS_CLIGetParameter(pcCommandString, 1, &paramterLen);
	ch = DecToInt((char *) parameterPtr, paramterLen);


    int32_t value;
    value = MOISTURE_Read(ch);
    sprintf(pcWriteBuffer, "ADC = %ld\n", value);

	return pdFALSE;
}

static const CLI_Command_Definition_t xMoistureRead =
{
	"mread", /* The command string to type. */
	"mread:\n    Read moisture sensor\n",
	MOISTURE_ReadCommand, /* The function to run. */
	1 /* No parameters are expected. */
};

#endif

/**
 *
 */
void MOISTURE_Init(void)
{
#if SUPPORT_MOISTURE_TEST_COMMAND
	FreeRTOS_CLIRegisterCommand( &xMoistureRead );
#endif

}
