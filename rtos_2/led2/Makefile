######################################################################
#  Project Makefile
######################################################################

BINARY		= main
SRCFILES	= main.c i2c.c lcd_hd44780_i2c.c clock.c rtos/heap_4.c rtos/list.c rtos/port.c rtos/queue.c rtos/tasks.c rtos/opencm3.c 

# ../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c  ../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c  ../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2c.c  ../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c  ../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c 

LDSCRIPT	= stm32f103c8t6.ld

# DEPS		= 	# Any additional dependencies for your build
# CLOBBER	+= 	# Any additional files to be removed with "make clobber"


include ../../Makefile.hal.incl
include ../Makefile.rtos

clock.o: clock.h
i2c.o: i2c.h
lcd_hd44780_i2c.o: lcd_hd44780_i2c.h stm32f1xx.h
main.o: i2c.h

######################################################################
#  NOTES:
#	1. remove any modules you don't need from SRCFILES
#	2. "make clean" will remove *.o etc., but leaves *.elf, *.bin
#	3. "make clobber" will "clean" and remove *.elf, *.bin etc.
#	4. "make flash" will perform:
#	   st-flash write main.bin 0x8000000
######################################################################
