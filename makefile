
TOP = .

# CMSIS files
VPATH					:=	$(TOP)/lib/CMSIS/src
PROJECT_INC_PATHS		:=	-I$(TOP)/lib/CMSIS/inc
PROJECT_SOURCE			:=	startup_stm32l476xx.s system_stm32l4xx.c

# STM32 Library
VPATH					+=  $(TOP)/lib/STM32L4xx_HAL_Driver/src

PROJECT_INC_PATHS		+=	-I$(TOP)/lib/STM32L4xx_HAL_Driver/inc

PROJECT_SOURCE			+=	stm32l4xx_hal.c								\
							stm32l4xx_hal_cortex.c						\
							stm32l4xx_hal_gpio.c						\
							stm32l4xx_hal_pwr.c							\
							stm32l4xx_hal_pwr_ex.c						\
							stm32l4xx_hal_rcc.c							\
							stm32l4xx_hal_rcc_ex.c						\
							stm32l4xx_hal_rtc.c							\
							stm32l4xx_hal_rtc_ex.c						\
							stm32l4xx_hal_tim.c							\
							stm32l4xx_hal_tim_ex.c						\
							stm32l4xx_hal_uart.c						\
#							stm32l4xx_hal_adc.c							\
#							stm32l4xx_hal_adc_ex.c						\
#							stm32l4xx_hal_can.c							\
#							stm32l4xx_hal_comp.c						\
#							stm32l4xx_hal_cortex.c						\
#							stm32l4xx_hal_crc.c							\
#							stm32l4xx_hal_cryp.c						\
#							stm32l4xx_hal_cryp_ex.c						\
#							stm32l4xx_hal_dac.c							\
#							stm32l4xx_hal_dac_ex.c						\
#							stm32l4xx_hal_dfsdm.c						\
#							stm32l4xx_hal_dma.c							\
#							stm32l4xx_hal_firewall.c					\
#							stm32l4xx_hal_flash.c						\
#							stm32l4xx_hal_flash_ex.c					\
#							stm32l4xx_hal_flash_ramfunc.c				\
#							stm32l4xx_hal_hcd.c							\
#							stm32l4xx_hal_i2c.c							\
#							stm32l4xx_hal_i2c_ex.c						\
#							stm32l4xx_hal_irda.c						\
#							stm32l4xx_hal_iwdg.c						\
#							stm32l4xx_hal_lcd.c							\
#							stm32l4xx_hal_lptim.c						\
#							stm32l4xx_hal_msp_template.c				\
#							stm32l4xx_hal_nand.c						\
#							stm32l4xx_hal_nor.c							\
#							stm32l4xx_hal_opamp.c						\
#							stm32l4xx_hal_opamp_ex.c					\
#							stm32l4xx_hal_pcd.c							\
#							stm32l4xx_hal_pcd_ex.c						\
#							stm32l4xx_hal_qspi.c						\
#							stm32l4xx_hal_rng.c							\
#							stm32l4xx_hal_sai.c							\
#							stm32l4xx_hal_sd.c							\
#							stm32l4xx_hal_smartcard.c					\
#							stm32l4xx_hal_smartcard_ex.c				\
#							stm32l4xx_hal_smbus.c						\
#							stm32l4xx_hal_spi.c							\
#							stm32l4xx_hal_spi_ex.c						\
#							stm32l4xx_hal_sram.c						\
#							stm32l4xx_hal_swpmi.c						\
#							stm32l4xx_hal_timebase_tim_template.c		\
#							stm32l4xx_hal_tsc.c							\
#							stm32l4xx_hal_uart_ex.c						\
#							stm32l4xx_hal_usart.c						\
#							stm32l4xx_hal_wwdg.c						\
#							stm32l4xx_ll_adc.c							\
#							stm32l4xx_ll_comp.c							\
#							stm32l4xx_ll_crc.c							\
#							stm32l4xx_ll_crs.c							\
#							stm32l4xx_ll_dac.c							\
#							stm32l4xx_ll_dma.c							\
#							stm32l4xx_ll_exti.c							\
#							stm32l4xx_ll_fmc.c							\
#							stm32l4xx_ll_gpio.c							\
#							stm32l4xx_ll_i2c.c							\
#							stm32l4xx_ll_lptim.c						\
#							stm32l4xx_ll_lpuart.c						\
#							stm32l4xx_ll_opamp.c						\
#							stm32l4xx_ll_pwr.c							\
#							stm32l4xx_ll_rcc.c							\
#							stm32l4xx_ll_rng.c							\
#							stm32l4xx_ll_rtc.c							\
#							stm32l4xx_ll_sdmmc.c						\
#							stm32l4xx_ll_spi.c							\
#							stm32l4xx_ll_swpmi.c						\
#							stm32l4xx_ll_tim.c							\
#							stm32l4xx_ll_usart.c						\
#							stm32l4xx_ll_usb.c							\
#							stm32l4xx_ll_utils.c						\

# FreeRTOS
VPATH					+=  $(TOP)/lib/FreeRTOS/src

PROJECT_INC_PATHS		+=	-I$(TOP)/lib/FreeRTOS/inc

PROJECT_SOURCE			+=	croutine.c						\
							event_groups.c					\
							heap_2.c						\
							list.c							\
							port.c							\
							queue.c							\
							tasks.c							\
							timers.c

# FreeRTOS-CLI
VPATH					+=  $(TOP)/lib/FreeRTOS-Plus-CLI

PROJECT_INC_PATHS		+=	-I$(TOP)/lib/FreeRTOS-Plus-CLI

PROJECT_SOURCE			+=	FreeRTOS_CLI.c


# LCD1602 driver
VPATH					+=  $(TOP)/drv/lcd1602-hal

PROJECT_INC_PATHS		+=	-I$(TOP)/drv/lcd1602-hal

PROJECT_SOURCE			+=	lcd1602-hal.c					\
							lcd1602-hal-test.c

# KeyPad(Digital) driver
VPATH					+=  $(TOP)/drv/keypad-d-hal

PROJECT_INC_PATHS		+=	-I$(TOP)/drv/keypad-d-hal

PROJECT_SOURCE			+=	keypad-d-hal.c					\
							keypad-d-hal-test.c

# General project files
VPATH					+=	$(TOP)/src						\
							$(TOP)/src/menu					\
                             
PROJECT_INC_PATHS		+=	-I$(TOP)/inc					\

PROJECT_SOURCE			+=	main.c							\
							gpio.c							\
							stm32l4xx_hal_msp.c				\
							stm32l4xx_hal_timebase_TIM.c	\
							stm32l4xx_it.c					\
							usart.c							\
							systemcall.c					\
							console.c						\
							util.c							\
							menu.c							\
							mainmenu.c						\
							calendar.c						\
							water.c							\


MCU_CC_FLAGS = $(CORTEX_M4_HWFP_CC_FLAGS)
MCU_LIB_PATH = $(CORTEX_M4_HWFP_LIB_PATH)
                   
PROJECT_NAME = STM32L476

PROJECT_SYMBOLS	=	-DSTM32L476xx                                  \
					-D__FPU_PRESENT=1                              \
                             
PROJECT_LINKER_SCRIPT = $(TOP)/STM32L476RG_FLASH.ld

PROJECT_DOXYGEN_CONFIG = config.doxyfile

PROJECT_OPENOCD_CONFIG = board/st_nucleo_l476rg.cfg

sinclude $(TOP)/common.mk

# Force to update the builded time in console.c
obj/console.o : force

obj/console.c.d : inc/gitversion.h

inc/gitversion.h: .git/HEAD .git/index
	@echo -e "#define GIT_VERSION\t\t\"$(shell git rev-parse --short HEAD)\"" > $@
 