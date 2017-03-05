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

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define MAX_CONTROLLER_NUM						3
#define CHAR_UP_ARROW							0
#define CHAR_DOWN_ARROW							1
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static uint8_t controllerNum;

/* Private functions ---------------------------------------------------------*/

/**
 *
 */
void waterMenu_Redraw(void)
{
	char buf[17];

	LCD_Clear();
	LCD_SetLoc(0,0);
	LCD_Put(CHAR_UP_ARROW);

	sprintf(buf, "<%d>", controllerNum);
	LCD_Print(buf);

	if( controllerNum != MAX_CONTROLLER_NUM )
	{
		LCD_SetLoc(0,1);
		LCD_Put(CHAR_DOWN_ARROW);
	}
}

/**
 *
 */
void waterMenu_Open(void)
{
	controllerNum = 0;

	LCD_Display(DISABLE);
	LCD_Blink(DISABLE);
	LCD_CreateFont(CHAR_UP_ARROW, fUpArrow);
	LCD_CreateFont(CHAR_DOWN_ARROW, fDownArrow);

	waterMenu_Redraw();

	LCD_Display(ENABLE);
}

Menu_t waterMenu =
{
	.open = waterMenu_Open,
	.close = NULL,
	.redraw = NULL,
	.up = NULL,
	.down = NULL,
	.right = NULL,
	.left = NULL,
	.btn_A = NULL,
	.btn_B = NULL,
};
