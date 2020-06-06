######################################################################
#  Project Makefile
######################################################################

HAL= ../../Drivers/STM32F1xx_HAL_Driver/Src/
BINARY		= main
SRCFILES	= main.c I2C.c  delay.c USART.c LiquidCrystal_I2C.c stm32f1xx_hal_msp.c stm32f1xx_it.c system_stm32f1xx.c rtos/heap_4.c rtos/list.c rtos/port.c rtos/queue.c rtos/tasks.c rtos/opencm3.c  $(HAL)stm32f1xx_hal.c $(HAL)stm32f1xx_hal_rcc.c $(HAL)stm32f1xx_hal_rcc_ex.c $(HAL)stm32f1xx_hal_gpio.c $(HAL)stm32f1xx_hal_i2c.c


LDSCRIPT	= stm32f103c8t6.ld

# DEPS		= 	# Any additional dependencies for your build
# CLOBBER	+= 	# Any additional files to be removed with "make clobber"


include ../../Makefile.hal.incl
include ../Makefile.rtos

delay.o: delay.h
I2C.o: I2C.h
LiquidCrystal_I2C.c: LiquidCrystal_I2C.h
USART.c: USART.h
stm32f1xx_hal_rcc.c: stm32f1xx_hal_rcc.h
stm32f1xx_hal_rcc_ex.c: stm32f1xx_hal_rcc_ex.h
stm32f1xx_hal_i2c.c: stm32f1xx_hal_i2c.h
stm32f1xx_hal_gpio.c: stm32f1xx_hal_gpio.h
stm32f1xx_hal_dma.c: stm32f1xx_hal_dma.h

main.o: I2C.h delay.h stm32f1xx.h stm32f1xx_hal_conf.h

######################################################################
#  NOTES:
#	1. remove any modules you don't need from SRCFILES
#	2. "make clean" will remove *.o etc., but leaves *.elf, *.bin
#	3. "make clobber" will "clean" and remove *.elf, *.bin etc.
#	4. "make flash" will perform:
#	   st-flash write main.bin 0x8000000
######################################################################
