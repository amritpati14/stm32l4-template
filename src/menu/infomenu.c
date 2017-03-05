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

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
 *
 */
void infoMenu_redraw(void)
{
	LCD_Clear();
	LCD_SetLoc(0,0);
	LCD_Print("INFO MENU");
}

/**
 *
 */
void infoMenu_Open(void)
{
	LCD_Display(DISABLE);
	LCD_Blink(DISABLE);

	infoMenu_redraw();

	LCD_Display(ENABLE);
}



Menu_t infoMenu =
{
	.open = infoMenu_Open,
	.close = NULL,
	.redraw = infoMenu_redraw,
	.up = NULL,
	.down = NULL,
	.right = NULL,
	.left = NULL,
	.btn_A = NULL,
	.btn_B = NULL,
};
