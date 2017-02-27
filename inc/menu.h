/**
  ******************************************************************************
  * @file    menu.h
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

#ifndef __INC_MENU_H_
#define __INC_MENU_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#define MENU_WAKEUP_ALARM					0x0001
#define MENU_WAKEUP_KEYPAD					0x0002

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
void MENU_Init(void);
void MENU_SetWakeupEvent(uint32_t wakeupEvent);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INC_MENU_H_ */
