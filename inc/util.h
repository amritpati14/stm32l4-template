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
												printf( args );								\
										}													\
									}while(0)
/* Exported functions ------------------------------------------------------- */
uint32_t HexToInt(char *str, uint16_t len);
uint32_t DecToInt(char *str, uint16_t len);
void udelay(uint32_t us);

#endif /* __INC_UTIL_H_ */
