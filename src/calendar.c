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
#define DBG_CONSOLE								TRUE


/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
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

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
	DEBUG_printf(DBG_CONSOLE, "\n%s\n", __FUNCTION__);

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
