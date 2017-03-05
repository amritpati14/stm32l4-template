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

#define CHAR_UP_ARROW							0
#define CHAR_DOWN_ARROW							1
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static int32_t m_lastUpdateTime;
static uint8_t m_controllerNum;
static WATER_ControllerTypeDef m_curController;

/* Private functions ---------------------------------------------------------*/

/**
 *
 */
void waterMenu_PrintController(void)
{
	char buf[17];

	WATER_GetController(m_controllerNum, &m_curController);

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
void waterMenu_Up(void)
{
	if( m_controllerNum == 0)
		MENU_SwitchMenu(&infoMenu);
	else
	{
		m_controllerNum--;
		waterMenu_PrintController();
	}
}

/**
 *
 */
void waterMenu_Down(void)
{
	if (m_controllerNum < (MAX_WATER_CONTROLLER_NUM - 1))
	{
		m_controllerNum++;
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
	.right = NULL,
	.left = NULL,
	.btn_A = NULL,
	.btn_B = NULL,
};
