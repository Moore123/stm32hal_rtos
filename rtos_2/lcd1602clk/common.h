#ifndef __COMMON_H

#define __COMMON_H

#include <stdio.h>
#include <stdint.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/usart.h>

#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/nvic.h>

#include "uartlib.h"

#define BIT_4BITS_PORT GPIOB
#define BIT_7_PIN GPIO7
#define BIT_6_PIN GPIO6
#define BIT_5_PIN GPIO5
#define BIT_4_PIN GPIO8


#endif
