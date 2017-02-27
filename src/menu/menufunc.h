/**
  ******************************************************************************
  * @file    menufunc.h
  * @author  dinow
  * @version V0.0.1
  * @date    2016-07-24
  * @brief
  ******************************************************************************
  * @attention
  *
  *
  *
  ******************************************************************************
  */

#ifndef __SRC_MENU_MENUFUNC_H_
#define __SRC_MENU_MENUFUNC_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "menu.h"
#include "lcd1602-hal.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
	void (*open)(void);
	void (*close)(void);
	void (*redraw)(void);
	void (*up)(void);
	void (*down)(void);
	void (*right)(void);
	void (*left)(void);
	void (*btn_A)(void);
	void (*btn_B)(void);
} Menu_t;

/* Exported constants --------------------------------------------------------*/
#define MENU_DEFAULT_MENU				(mainMenu)

#define DBG_MENU						1

/* Exported macro ------------------------------------------------------------*/

/* Exported module variables -------------------------------------------------*/
extern Menu_t mainMenu;

/* Exported functions ------------------------------------------------------- */
void MENU_SwitchMenu(Menu_t *newMenu);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SRC_MENU_MENUFUNC_H_ */
