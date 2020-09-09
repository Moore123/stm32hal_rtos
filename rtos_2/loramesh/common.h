#ifndef __COMMON_H

#define __COMMON_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/usart.h>

#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/cm3/nvic.h>

#include <libopencm3/cm3/systick.h>

#include "uartlib.h"

#define BIT_4BITS_PORT GPIOB
#define BIT_7_PIN GPIO9
#define BIT_6_PIN GPIO8
#define BIT_5_PIN GPIO7
#define BIT_4_PIN GPIO6

#define LCD_BIT_RS                 ((uint8_t)0x01U)
#define LCD_BIT_RW                 ((uint8_t)0x02U)
#define LCD_BIT_E                  ((uint8_t)0x04U)
#define LCD_BIT_BACKLIGHT_ON        ((uint8_t)0x08U)
#define LCD_BIT_BACKLIGHT_OFF       ((uint8_t)0x00U)

#define LCD_MODE_4BITS             ((uint8_t)0x02U)
#define LCD_BIT_1LINE              ((uint8_t)0x00U)
#define LCD_BIT_2LINE              ((uint8_t)0x08U)
#define LCD_BIT_4LINE              LCD_BIT_2LINE
#define LCD_BIT_5x8DOTS            ((uint8_t)0x00U)
#define LCD_BIT_5x10DOTS           ((uint8_t)0x04U)
#define LCD_BIT_SETCGRAMADDR       ((uint8_t)0x40U)
#define LCD_BIT_SETDDRAMADDR       ((uint8_t)0x80U)

#define LCD_BIT_DISPLAY_CONTROL    ((uint8_t)0x08U) //Pass
#define LCD_BIT_DISPLAY_ON         ((uint8_t)0x04U) //Pass
#define LCD_BIT_CURSOR_ON          ((uint8_t)0x02U)
#define LCD_BIT_CURSOR_OFF         ((uint8_t)0x00U)
#define LCD_BIT_BLINK_ON           ((uint8_t)0x01U)
#define LCD_BIT_BLINK_OFF          ((uint8_t)0x00U)

#define LCD_BIT_DISP_CLEAR         ((uint8_t)0x1U)  
#define LCD_BIT_CURSOR_HOME        ((uint8_t)0x20U)

#define LCD_BIT_ENTRY_MODE         ((uint8_t)0x04U)
#define LCD_BIT_CURSOR_DIR_RIGHT   ((uint8_t)0x02U)
#define LCD_BIT_CURSOR_DIR_LEFT    ((uint8_t)0x00U)
#define LCD_BIT_DISPLAY_SHIFT      ((uint8_t)0x06U)

extern void send_to_lcd(const char val,unsigned char rs);
extern void lcd_send_data (unsigned char data,char cmd0);
extern void lcd_send_string (char *str) ;
extern void clear_lcd(void) ;
extern void shift_lcd(void) ;
extern void cursor_home(void) ;
extern void backoff_lcd(void) ;
extern void backon_lcd(void) ;
extern void init_lcd(void) ;
void welcome(char *l1, char *l2,int clear) ;

#endif
