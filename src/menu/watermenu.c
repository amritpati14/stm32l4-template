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

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define MOISTURE_UPDATE_TIME					500

#define CURSOR_POS_HOUR							0
#define CURSOR_POS_MIN							1
#define CURSOR_POS_PERIOD						2
#define CURSOR_POS_MOISTURE						3

#define CHAR_UP_ARROW							0
#define CHAR_DOWN_ARROW							1
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static int32_t m_lastUpdateTime;
static uint8_t m_controllerNum;
static bool m_setController = FALSE;
static uint8_t m_curPos;

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

	sprintf(buf, " %2d sec    /%3d", m_curController.Period, m_curController.Moisture);
	LCD_Print(buf);

}

/**
 *
 */
void waterMenu_Open(void)
{
	m_controllerNum = 0;
	WATER_GetController(m_controllerNum, &m_curController);

	m_setController = FALSE;
	m_lastUpdateTime = (int32_t) xTaskGetTickCount();

	LCD_Display(DISABLE);
	LCD_Blink(DISABLE);
	LCD_CreateFont(CHAR_UP_ARROW, fUpArrow);
	LCD_CreateFont(CHAR_DOWN_ARROW, fDownArrow);

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
	if(!m_setController) return;

	if (m_curPos < CURSOR_POS_MOISTURE)
	{
		m_curPos++;
		waterMenu_UpdataCurPos();
	}
}

/**
 *
 */
void waterMenu_Left(void)
{
	if(!m_setController) return;

	if (m_curPos > CURSOR_POS_HOUR)
	{
		m_curPos--;
		waterMenu_UpdataCurPos();
	}
}


/**
 *
 */
void waterMenu_Up(void)
{
	if(!m_setController)
	{
		if( m_controllerNum == 0)
			MENU_SwitchMenu(&infoMenu);
		else
		{
			m_controllerNum--;
			WATER_GetController(m_controllerNum, &m_curController);
			waterMenu_PrintController();
		}
	}
	else
	{
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
	}
}

/**
 *
 */
void waterMenu_Down(void)
{
	if (!m_setController)
	{

		if (m_controllerNum < (MAX_WATER_CONTROLLER_NUM - 1))
		{
			m_controllerNum++;
			WATER_GetController(m_controllerNum, &m_curController);
			waterMenu_PrintController();
		}
	}
	else
	{
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
	}
}

/**
 *
 */
void waterMenu_Set(void)
{
	if( m_setController )
	{
		m_setController = FALSE;
		WATER_SetController(m_controllerNum, &m_curController);
		LCD_Blink(DISABLE);
		waterMenu_PrintController();
	}
	else
	{
		m_setController = TRUE;
		m_curPos = CURSOR_POS_HOUR;
		waterMenu_UpdataCurPos();
		LCD_Blink(ENABLE);
	}
}

/**
 *
 */
void waterMenu_Cancel(void)
{
	if( m_setController )
	{
		m_setController = FALSE;
		LCD_Blink(DISABLE);
		WATER_GetController(m_controllerNum, &m_curController);
		waterMenu_PrintController();
	}
}

Menu_t waterMenu =
{
	.open = waterMenu_Open,
	.close = NULL,
	.redraw = NULL,
	.up = waterMenu_Up,
	.down = waterMenu_Down,
	.right = waterMenu_Right,
	.left = waterMenu_Left,
	.btn_A = waterMenu_Set,
	.btn_B = waterMenu_Cancel,
};
