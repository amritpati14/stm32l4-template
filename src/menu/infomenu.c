/**
  ******************************************************************************
  * @file    infomenu.c
  * @author  dinow
  * @version V0.0.1
  * @date    2017-03-05
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
#include "menufunc.h"
#include "calendar.h"
#include "water.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define CLOCK_UPDATE_TIME						100

#define CURSOR_POS_HOUR							0
#define CURSOR_POS_MIN							1
#define CURSOR_POS_SEC							2

#define CHAR_UP_ARROW							0
#define CHAR_DOWN_ARROW							1
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static int32_t m_lastUpdateTime;
static bool m_setClock = FALSE;
static uint8_t m_curPos;
static RTC_TimeTypeDef m_curTime;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
 *
 */
void infoMenu_Redraw(void)
{
	if( m_setClock ) return;

	char buf[17];

	if ((((int32_t) xTaskGetTickCount()) - m_lastUpdateTime) > CLOCK_UPDATE_TIME)
	{
		m_lastUpdateTime = xTaskGetTickCount();
		HAL_RTC_GetTime(&hrtc, &m_curTime, RTC_FORMAT_BIN);

		RTC_DateTypeDef sDate;
		HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

		sprintf(buf, "%2d:%02d:%02d", m_curTime.Hours, m_curTime.Minutes, m_curTime.Seconds);

		LCD_SetLoc(8, 0);
		LCD_Print(buf);

		int nextController;
		nextController = WATER_GetNextActiveController();

		LCD_SetLoc(2,1);

		if( nextController == -1 )
		{
			LCD_SetLoc(2,1);
			LCD_Print("Next None     ");
		}
		else
		{
			WATER_ControllerTypeDef controller;
			WATER_GetController(nextController, &controller);

			sprintf(buf, "Next <%d> %2d:%02d", nextController, controller.Hour, controller.Minutes);
			LCD_Print(buf);
		}
	}
}

/**
 *
 */
void infoMenu_Open(void)
{
	m_lastUpdateTime = (int32_t) xTaskGetTickCount();
	m_setClock = FALSE;

	LCD_Display(DISABLE);
	LCD_Blink(DISABLE);
	LCD_CreateFont(CHAR_UP_ARROW, fUpArrow);
	LCD_CreateFont(CHAR_DOWN_ARROW, fDownArrow);

	LCD_Clear();
	LCD_SetLoc(1,0);

	LCD_Print(" Time");

	LCD_SetLoc(0,1);
	LCD_Put(CHAR_DOWN_ARROW);

	LCD_Display(ENABLE);
}

/**
 *
 */
void infoMenu_UpdataCurPos(void)
{
	switch(m_curPos)
	{
		case CURSOR_POS_HOUR:
			LCD_SetLoc(9, 0);
			break;

		case CURSOR_POS_MIN:
			LCD_SetLoc(12, 0);
			break;

		case CURSOR_POS_SEC:
			LCD_SetLoc(15, 0);
			break;

		default:
			break;
	}
}

/**
 *
 */
void infoMenu_UpdataTime(void)
{
	char buf[17];

	sprintf(buf, "%2d:%02d:%02d", m_curTime.Hours, m_curTime.Minutes, m_curTime.Seconds);

	LCD_Blink(DISABLE);
	LCD_SetLoc(8, 0);
	LCD_Print(buf);

	infoMenu_UpdataCurPos();
	LCD_Blink(ENABLE);

}

/**
 *
 */
void infoMenu_Set(void)
{
	if( m_setClock )
	{
		m_curTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		m_curTime.StoreOperation = RTC_STOREOPERATION_RESET;
		HAL_RTC_SetTime(&hrtc, &m_curTime, RTC_FORMAT_BIN);
		WATER_UpdateNextActiveController();

		m_setClock = FALSE;
		LCD_Blink(DISABLE);
		infoMenu_Redraw();
	}
	else
	{
		m_setClock = TRUE;
		m_curPos = CURSOR_POS_HOUR;
		LCD_SetLoc(9,0);
		LCD_Blink(ENABLE);
	}
}

/**
 *
 */
void infoMenu_Cancel(void)
{
	if( m_setClock )
	{
		m_setClock = FALSE;
		LCD_Blink(DISABLE);
		infoMenu_Redraw();
	}
}

/**
 *
 */
void infoMenu_Left(void)
{
	if (!m_setClock) return;

	if (m_curPos > CURSOR_POS_HOUR)
	{
		m_curPos--;
		infoMenu_UpdataCurPos();
	}
}

/**
 *
 */
void infoMenu_Right(void)
{
	if( !m_setClock ) return;

	if (m_curPos < CURSOR_POS_SEC)
	{
		m_curPos++;
		infoMenu_UpdataCurPos();
	}
}

/**
 *
 */
void infoMenu_Up(void)
{
	if (!m_setClock) return;

	switch (m_curPos)
	{
		case CURSOR_POS_HOUR:
			m_curTime.Hours++;
			if (m_curTime.Hours >= 24)
				m_curTime.Hours -= 24;
			break;

		case CURSOR_POS_MIN:
			m_curTime.Minutes++;
			if (m_curTime.Minutes >= 60)
				m_curTime.Minutes -= 60;
			break;

		case CURSOR_POS_SEC:
			m_curTime.Seconds++;
			if (m_curTime.Seconds >= 60)
				m_curTime.Seconds -= 60;
			break;

		default:
			break;
	}

	infoMenu_UpdataTime();
}

/**
 *
 */
void infoMenu_Down(void)
{
	if (m_setClock)
	{
		switch (m_curPos)
		{
			case CURSOR_POS_HOUR:
				if (m_curTime.Hours == 0)
					m_curTime.Hours = 23;
				else
					m_curTime.Hours--;
				break;

			case CURSOR_POS_MIN:
				if (m_curTime.Minutes == 0)
					m_curTime.Minutes = 59;
				else
					m_curTime.Minutes--;
				break;

			case CURSOR_POS_SEC:
				if (m_curTime.Seconds == 0)
					m_curTime.Seconds = 59;
				else
					m_curTime.Seconds--;
				break;

			default:
				break;
		}

		infoMenu_UpdataTime();
	}
	else
	{
		MENU_SwitchMenu(&waterMenu);
	}
}

Menu_t infoMenu =
{
	.open = infoMenu_Open,
	.close = NULL,
	.redraw = infoMenu_Redraw,
	.up = infoMenu_Up,
	.down = infoMenu_Down,
	.right = infoMenu_Right,
	.left = infoMenu_Left,
	.btn_A = infoMenu_Set,
	.btn_B = infoMenu_Cancel,
};
