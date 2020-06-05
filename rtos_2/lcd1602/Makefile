######################################################################
#  Project Makefile
######################################################################

STMLIB= ../../stm_lib/src/
BINARY		= main
SRCFILES	= main.c delay.c I2C.c LiquidCrystal_I2C.c USART.c $(STMLIB)stm32f10x_rcc.c $(STMLIB)stm32f10x_gpio.c $(STMLIB)stm32f10x_i2c.c rtos/heap_4.c rtos/list.c rtos/port.c rtos/queue.c rtos/tasks.c rtos/opencm3.c 



LDSCRIPT	= stm32f103c8t6.ld

# DEPS		= 	# Any additional dependencies for your build
# CLOBBER	+= 	# Any additional files to be removed with "make clobber"


include ../../Makefile.stmlib.incl
include ../Makefile.rtos

delay.o: delay.h
I2C.o: I2C.h
LiquidCrystal_I2C.o: LiquidCrystal_I2C.h
main.o: I2C.h delay.h USART.h

######################################################################
#  NOTES:
#	1. remove any modules you don't need from SRCFILES
#	2. "make clean" will remove *.o etc., but leaves *.elf, *.bin
#	3. "make clobber" will "clean" and remove *.elf, *.bin etc.
#	4. "make flash" will perform:
#	   st-flash write main.bin 0x8000000
######################################################################
