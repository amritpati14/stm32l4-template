/**
  ******************************************************************************
  * @file    menu.c
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
#include "menufunc.h"
#include "usart.h"
#include "water.h"

/* Private typedef -----------------------------------------------------------*/
#define MENU_TASK_PRIORITY					( tskIDLE_PRIORITY + 1UL )
#define MENU_TASK_STACK						(2048/4) // 2048 bytes

#define MENU_KEY_POLLING_DELAY				(10 / portTICK_PERIOD_MS)
#define MENU_KEY_DETECTION_TIMEOUT			(400 / portTICK_PERIOD_MS)
#define MENU_KEY_SLEEP_TIMEOUT				(20000 / portTICK_PERIOD_MS)

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static Menu_t *m_currentMenu;
volatile uint32_t m_wakeupEvent = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
 *
 * @param wakeupEvent
 */
void MENU_SetWakeupEvent(uint32_t wakeupEvent)
{
	m_wakeupEvent |= wakeupEvent;
}

/**
 *
 * @param wakeupEvent
 */
void MENU_ClrWakeupEvent(uint32_t wakeupEvent)
{
	taskENTER_CRITICAL();
	m_wakeupEvent &= ~wakeupEvent;
	taskEXIT_CRITICAL();
}

/**
 * @brief	switch menu
 * @param	newMenu
 */
void MENU_SwitchMenu(Menu_t *newMenu)
{
	if( m_currentMenu->close != NULL)
		m_currentMenu->close();

	m_currentMenu = newMenu;

	if( m_currentMenu->open != NULL )
		m_currentMenu->open();
}

/**
 * @brief	Get the new key pressed with debounce
 * @param	timeout :
 * @return	key pressed
 */
#define GNK_STATE_INIT			0
#define GNK_STATE_HOLD			1
#define GNK_STATE_UP			2
Key_t MENU_GetNewKey(int16_t timeout)
{
	Key_t ret = Key_None;
	Key_t key = Key_None;

	bool newKey = FALSE;
	int8_t pressCount = 0;
	int32_t startTime = (int32_t) xTaskGetTickCount();

	while ((((int32_t) xTaskGetTickCount()) - startTime) < timeout)
	{
		key = KEY_GetKey();

		if (key == Key_None)
		{
			pressCount = 0;
			newKey = TRUE;
			ret = Key_None;
		}
		else
		{
			if (newKey == TRUE)
			{
				newKey = FALSE;
				ret = key;
				pressCount = 1;
			}
			else
			{
				if (ret != Key_None)
				{
					if (ret == key)
						pressCount++;
					else
					{
						ret = key;
						pressCount = 1;
					}
				}

				if (pressCount == 2)
					return ret;
			}

		}

		vTaskDelay(MENU_KEY_POLLING_DELAY);
	}

	return key;
}

/**
 *
 */
void MENU_InputProcess(void)
{
	Key_t key;
	int32_t idleStartTime;

	// Reset Menu
	//KEY_Enable();
	idleStartTime = (int32_t) xTaskGetTickCount();

	m_currentMenu = &MENU_DEFAULT_MENU;
	if( m_currentMenu->open != NULL )
		m_currentMenu->open();

	// Start menu loop
	while(1)
	{
		key = MENU_GetNewKey(MENU_KEY_DETECTION_TIMEOUT);

		if( key != Key_None )
		{
			idleStartTime = (int32_t) xTaskGetTickCount();
		}
		else
		{
			if ((((int32_t) xTaskGetTickCount()) - idleStartTime) > MENU_KEY_SLEEP_TIMEOUT)
			{
				if( m_currentMenu->close != NULL)
					m_currentMenu->close();

				break;
			}
		}

		switch(key)
		{
			case Key_None:
				if( m_currentMenu->redraw != NULL )
					m_currentMenu->redraw();
				break;

			case Key_Left:
				if( m_currentMenu->left != NULL )
					m_currentMenu->left();
				break;

			case Key_Right:
				if( m_currentMenu->right != NULL )
					m_currentMenu->right();
				break;

			case Key_Up:
				if( m_currentMenu->up != NULL )
					m_currentMenu->up();
				break;

			case Key_Down:
				if( m_currentMenu->down != NULL )
					m_currentMenu->down();
				break;

			case Key_A:
				if( m_currentMenu->btn_A != NULL )
					m_currentMenu->btn_A();
				break;

			case Key_B:
				if( m_currentMenu->btn_B != NULL )
					m_currentMenu->btn_B();
				break;

			default:
				break;
		}
	}

}

void MENU_Sleep(void)
{
	// Disable systick interrupt to prevent that wake up immediately
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;

	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	SystemClock_Config();

	// Enable systick interrupt
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

/**
 *
 * @param pvParameters
 */
static void MENU_Task( void *pvParameters )
{
	MENU_SetWakeupEvent(MENU_WAKEUP_KEYPAD); // Enabel display after power

	while(1)
	{
		// Check alarm event
		if (m_wakeupEvent & MENU_WAKEUP_ALARM)
		{
			MENU_ClrWakeupEvent(MENU_WAKEUP_ALARM);
			WATER_AlarmHandler();
			WATER_UpdateNextActiveController();
		}

		// Check keypad event
		if (m_wakeupEvent & MENU_WAKEUP_KEYPAD)
		{
			KEY_DisableIRQ();
			LCD_Enable();
			MENU_InputProcess();
			LCD_Disable();
			KEY_EnableIRQ();
			MENU_ClrWakeupEvent(MENU_WAKEUP_KEYPAD);
		}

		if (m_wakeupEvent == 0)
		{
			if (WATER_TryLock())
			{
				DEBUG_printf(DBG_MENU, "\n%s: Sleep\n", __FUNCTION__);
				MENU_Sleep();
				WATER_Unlock();
			}
			else
			{
				vTaskDelay(100);
			}
		}
	}

}

/**
 * @brief	Menu task initialize flow before schedule start
 */
void MENU_Init(void)
{
	xTaskCreate( MENU_Task, "MENU", MENU_TASK_STACK, NULL, MENU_TASK_PRIORITY, NULL );
}
