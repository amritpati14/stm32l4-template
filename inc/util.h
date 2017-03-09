/**
  ******************************************************************************
  * @file    util.h
  * @author  Dinow
  * @version V0.0.1
  * @date    2015-12-06
  * @brief   
  ******************************************************************************
  * @attention
  *
  *
  *
  ******************************************************************************
  */

#ifndef __INC_UTIL_H_
#define __INC_UTIL_H_

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define DEBUG_printf(CONDITION, args...)	do												\
									{														\
										if(CONDITION)										\
										{													\
											if(xSemaphoreTake(xPrintMutex, portMAX_DELAY))	\
											{												\
												printf( args );								\
												xSemaphoreGive(xPrintMutex);				\
											}												\
										}													\
									}while(0)
/* Exported functions ------------------------------------------------------- */
extern SemaphoreHandle_t xPrintMutex;
uint32_t HexToInt(char *str, uint16_t len);
uint32_t DecToInt(char *str, uint16_t len);
void udelay(uint32_t us);

#endif /* __INC_UTIL_H_ */
