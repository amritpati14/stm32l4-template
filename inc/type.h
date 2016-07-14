/**
  ******************************************************************************
  * @file    type.h
  * @author  Dinow
  * @version V0.0.1
  * @date    2015-11-15
  * @brief   
  ******************************************************************************
  * @attention
  *
  *
  *
  ******************************************************************************
  */

#ifndef __TYPE_H_
#define __TYPE_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
typedef enum {
	FALSE = 0,
	TRUE = !FALSE
} bool;

/* Exported constants --------------------------------------------------------*/
#define BIT0		0x0001
#define BIT1		0x0002
#define BIT2		0x0004
#define BIT3		0x0008
#define BIT4		0x0010
#define BIT5		0x0020
#define BIT6		0x0040
#define BIT7		0x0080

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

#endif /* __TYPE_H_ */