/**
  ******************************************************************************
  * @file    keypad-d-hal-test.c
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

/* Includes ------------------------------------------------------------------*/
#include "global.h"
#include "keypad-d-hal.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
 * @param	pcWriteBuffer
 * @param	xWriteBufferLen
 * @param	pcCommandString
 * @return
 */
static BaseType_t KEY_GetCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
	Key_t key;
	key = KEY_GetKey();

	switch (key)
	{
		case Key_None:
			sprintf(pcWriteBuffer, "Key_None\n");
			break;

		case Key_Left:
			sprintf(pcWriteBuffer, "Key_Left\n");
			break;

		case Key_Right:
			sprintf(pcWriteBuffer, "Key_Right\n");
			break;

		case Key_Up:
			sprintf(pcWriteBuffer, "Key_Up\n");
			break;

		case Key_Down:
			sprintf(pcWriteBuffer, "Key_Down\n");
			break;

		case Key_A:
			sprintf(pcWriteBuffer, "Key_A\n");
			break;

		case Key_B:
			sprintf(pcWriteBuffer, "Key_B\n");
			break;

	}
	return pdFALSE;
}

static const CLI_Command_Definition_t xKeyGet =
{
	"key-get",
	"key-get:\n    Return button pressed on keypad\n",
	KEY_GetCommand,
	0
};

/**
 * @brief	Register CLI command
 */
void KEY_Test(void)
{
	FreeRTOS_CLIRegisterCommand(&xKeyGet);
}
