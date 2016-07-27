/**
  ******************************************************************************
  * @file    keypad-d-hal.h
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

#ifndef __DRV_KEYPAD_D_HAL_KEYPAD_D_HAL_H_
#define __DRV_KEYPAD_D_HAL_KEYPAD_D_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef enum {
	Key_None = 0,
	Key_Left,
	Key_Right,
	Key_Up,
	Key_Down,
	Key_A,
	Key_B
} Key_t;

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
void KEY_Init(void);
Key_t KEY_GetKey(void);
void KEY_EnableIRQ(void);
void KEY_DisableIRQ(void);

void KEY_Test(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DRV_KEYPAD_D_HAL_KEYPAD_D_HAL_H_ */
