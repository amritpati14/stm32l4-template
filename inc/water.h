/**
  ******************************************************************************
  * @file    water.h
  * @author  dinow
  * @version V0.0.1
  * @date    2017-03-03
  * @brief
  ******************************************************************************
  * @attention
  *
  *
  *
  ******************************************************************************
  */

#ifndef __INC_WATER_H_
#define __INC_WATER_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef struct
{
	uint8_t Hour;
	uint8_t Minutes;
	uint8_t Period;
	uint8_t Moisture;
} WATER_ControllerTypeDef;

/* Exported constants --------------------------------------------------------*/
#define MAX_WATER_CONTROLLER_NUM				4

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
void WATER_Init(void);
void WATER_Enable(void);
void WATER_Disable(void);
void WATER_GetController(uint8_t num, WATER_ControllerTypeDef *sController);
void WATER_SetController(uint8_t num, WATER_ControllerTypeDef *sController);
int16_t WATER_GetNextActiveController(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INC_WATER_H_ */
