/**
  ******************************************************************************
  * @file    watermenu.c
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
#include "moisture.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define MOISTURE_UPDATE_TIME					1000

#define CURSOR_POS_HOUR							0
#define CURSOR_POS_MIN							1
#define CURSOR_POS_PERIOD						2
#define CURSOR_POS_MOISTURE						3

#define CHAR_UP_ARROW							0
#define CHAR_DOWN_ARROW							1

#define MENU_MODE_NORMAL						0
#define MENU_MODE_SET							1
#define MENU_MODE_TEST							2

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static int32_t m_lastUpdateTime;
static uint8_t m_controllerNum;
static uint8_t m_menuMode = MENU_MODE_NORMAL;
static uint8_t m_curPos;
static int16_t m_curMoisture;

static WATER_ControllerTypeDef m_curController;

/* Private functions ---------------------------------------------------------*/

/**
 *
 */
void waterMenu_PrintController(void)
{
	char buf[17];

	LCD_Clear();
	LCD_SetLoc(0,0);
	LCD_Put(CHAR_UP_ARROW);

	sprintf(buf, " <%d>      %2d:%02d", m_controllerNum, m_curController.Hour, m_curController.Minutes);
	LCD_Print(buf);

	LCD_SetLoc(0,1);

	if (m_controllerNum != (MAX_WATER_CONTROLLER_NUM - 1))
		LCD_Put(CHAR_DOWN_ARROW);
	else
		LCD_Put(' ');

	sprintf(buf, " %2d sec %3d/%3d", m_curController.Period, m_curMoisture, m_curController.Moisture);
	LCD_Print(buf);

}

/**
 *
 */
void waterMenu_Open(void)
{
	m_controllerNum = 0;
	WATER_GetController(m_controllerNum, &m_curController);

	m_menuMode = MENU_MODE_NORMAL;
	m_lastUpdateTime = (int32_t) xTaskGetTickCount();

	LCD_Display(DISABLE);
	LCD_Blink(DISABLE);
	LCD_CreateFont(CHAR_UP_ARROW, fUpArrow);
	LCD_CreateFont(CHAR_DOWN_ARROW, fDownArrow);

	m_curMoisture = MOISTURE_Read(m_controllerNum);
	m_curMoisture = MOISTURE_Read(m_controllerNum);
	waterMenu_PrintController();

	LCD_Display(ENABLE);
}

/**
 *
 */
void waterMenu_UpdataCurPos(void)
{
	switch(m_curPos)
	{
		case CURSOR_POS_HOUR:
			LCD_SetLoc(12, 0);
			break;

		case CURSOR_POS_MIN:
			LCD_SetLoc(15, 0);
			break;

		case CURSOR_POS_PERIOD:
			LCD_SetLoc(3, 1);
			break;

		case CURSOR_POS_MOISTURE:
			LCD_SetLoc(15, 1);
			break;

		default:
			break;
	}
}

/**
 *
 */
void waterMenu_Right(void)
{
	switch(m_menuMode)
	{
		case MENU_MODE_SET:
			if (m_curPos < CURSOR_POS_MOISTURE)
			{
				m_curPos++;
				waterMenu_UpdataCurPos();
			}
			break;

		case MENU_MODE_NORMAL:
		case MENU_MODE_TEST:
		default:
			break;
	}
}

/**
 *
 */
void waterMenu_Left(void)
{
	switch(m_menuMode)
	{
		case MENU_MODE_SET:
			if (m_curPos > CURSOR_POS_HOUR)
			{
				m_curPos--;
				waterMenu_UpdataCurPos();
			}
			break;

		case MENU_MODE_NORMAL:
		case MENU_MODE_TEST:
		default:
			break;
	}
}


/**
 *
 */
void waterMenu_Up(void)
{
	switch(m_menuMode)
	{
		case MENU_MODE_NORMAL:
			if( m_controllerNum == 0)
				MENU_SwitchMenu(&infoMenu);
			else
			{
				m_controllerNum--;
				WATER_GetController(m_controllerNum, &m_curController);
				m_curMoisture = MOISTURE_Read(m_controllerNum);
				waterMenu_PrintController();
			}
			break;

		case MENU_MODE_SET:
			switch (m_curPos)
			{
				case CURSOR_POS_HOUR:
					if( m_curController.Hour == 23 )
						m_curController.Hour = 0;
					else
						m_curController.Hour++;
					break;

				case CURSOR_POS_MIN:
					if( m_curController.Minutes == 59 )
						m_curController.Minutes = 0;
					else
						m_curController.Minutes++;
					break;

				case CURSOR_POS_PERIOD:
					if (m_curController.Period < 30)
						m_curController.Period++;
					break;

				case CURSOR_POS_MOISTURE:
					if (m_curController.Moisture < 240)
						m_curController.Moisture += 10;
					else
						m_curController.Moisture = 250;
					break;
			}

			LCD_Blink(DISABLE);
			waterMenu_PrintController();
			waterMenu_UpdataCurPos();
			LCD_Blink(ENABLE);
			break;

		case MENU_MODE_TEST:
		default:
			break;
	}
}

/**
 *
 */
void waterMenu_Down(void)
{
	switch(m_menuMode)
	{
		case MENU_MODE_NORMAL:
			if (m_controllerNum < (MAX_WATER_CONTROLLER_NUM - 1))
			{
				m_controllerNum++;
				WATER_GetController(m_controllerNum, &m_curController);
				m_curMoisture = MOISTURE_Read(m_controllerNum);
				waterMenu_PrintController();
			}
			break;

		case MENU_MODE_SET:
			switch (m_curPos)
			{
				case CURSOR_POS_HOUR:
					if( m_curController.Hour == 0 )
						m_curController.Hour = 23;
					else
						m_curController.Hour--;
					break;

				case CURSOR_POS_MIN:
					if( m_curController.Minutes == 0 )
						m_curController.Minutes = 59;
					else
						m_curController.Minutes--;
					break;

				case CURSOR_POS_PERIOD:
					if (m_curController.Period > 0)
						m_curController.Period--;
					break;

				case CURSOR_POS_MOISTURE:
					if (m_curController.Moisture >= 10)
						m_curController.Moisture -= 10;
					else
						m_curController.Moisture = 0;
					break;
			}

			LCD_Blink(DISABLE);
			waterMenu_PrintController();
			waterMenu_UpdataCurPos();
			LCD_Blink(ENABLE);
			break;

		case MENU_MODE_TEST:
			m_menuMode = MENU_MODE_NORMAL;
			waterMenu_PrintController();
			break;

		default:
			break;

	}
}

/**
 *
 */
void waterMenu_Set(void)
{
	switch(m_menuMode)
	{
		case MENU_MODE_NORMAL:
			m_menuMode = MENU_MODE_SET;
			m_curPos = CURSOR_POS_HOUR;
			waterMenu_UpdataCurPos();
			LCD_Blink(ENABLE);
			break;

		case MENU_MODE_SET:
			m_menuMode = MENU_MODE_NORMAL;
			WATER_SetController(m_controllerNum, &m_curController);
			LCD_Blink(DISABLE);
			waterMenu_PrintController();
			break;

		case MENU_MODE_TEST:
			m_menuMode = MENU_MODE_NORMAL;
			if( m_curController.Period != 0)
				WATER_OpenController(m_controllerNum);
			LCD_Blink(DISABLE);
			waterMenu_PrintController();
			break;

		default:
			break;
	}
}

/**
 *
 */
void waterMenu_Cancel(void)
{
	switch(m_menuMode)
	{
		case MENU_MODE_SET:
			m_menuMode = MENU_MODE_NORMAL;
			LCD_Blink(DISABLE);
			WATER_GetController(m_controllerNum, &m_curController);
			waterMenu_PrintController();
			break;

		case MENU_MODE_NORMAL:
			m_menuMode = MENU_MODE_TEST;

			char buf[17];

			LCD_Clear();
			LCD_SetLoc(0,0);

			sprintf(buf, "  <%d> Test?", m_controllerNum);
			LCD_Print(buf);
			LCD_Blink(ENABLE);

			break;

		case MENU_MODE_TEST:
			m_menuMode = MENU_MODE_NORMAL;
			LCD_Blink(DISABLE);
			waterMenu_PrintController();
			break;

		default:
			break;
	}
}

void waterMenu_Redraw(void)
{
	switch(m_menuMode)
	{
		case MENU_MODE_NORMAL:
			if ((((int32_t) xTaskGetTickCount()) - m_lastUpdateTime) > MOISTURE_UPDATE_TIME)
			{
				m_lastUpdateTime = xTaskGetTickCount();
				m_curMoisture = MOISTURE_Read(m_controllerNum);
				waterMenu_PrintController();
			}
			break;

		case MENU_MODE_TEST:
		case MENU_MODE_SET:
		default:
			break;

	}

}

Menu_t waterMenu =
{
	.open = waterMenu_Open,
	.close = NULL,
	.redraw = waterMenu_Redraw,
	.up = waterMenu_Up,
	.down = waterMenu_Down,
	.right = waterMenu_Right,
	.left = waterMenu_Left,
	.btn_A = waterMenu_Set,
	.btn_B = waterMenu_Cancel,
};
