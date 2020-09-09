/*
 * =====================================================================================
 *
 *       Filename:  lcd1602.c
 *
 * =====================================================================================
 */

#include "common.h"
#include "tips.h"

void send_to_lcd(const char val,unsigned char rs) {

  ( val & 0x1 ) ?  gpio_set(GPIOB, GPIO6) : gpio_clear(GPIOB, GPIO6);
  ( val & 0x2 ) ?  gpio_set(GPIOB, GPIO7) : gpio_clear(GPIOB, GPIO7);
  ( val & 0x4 ) ?  gpio_set(GPIOB, GPIO8) : gpio_clear(GPIOB, GPIO8);
  ( val & 0x8 ) ?  gpio_set(GPIOB, GPIO9) : gpio_clear(GPIOB, GPIO9);

  ( rs & 0x1 ) ?  gpio_set(GPIOB, GPIO13) : gpio_clear(GPIOB, GPIO13);

   gpio_set(GPIOB, GPIO12);
   delay_100us(1);
   gpio_clear(GPIOB, GPIO12);
   
   return;
}

void lcd_send_data (unsigned char data,char cmd0)
{
      unsigned char datatosend;
       datatosend = ((data>>4)&0x0f);
       send_to_lcd(datatosend, cmd0);  // cmd0 is rs =1 for sending data
       /* send Lower nibble */
       datatosend = ((data)&0x0f);
       send_to_lcd(datatosend, cmd0);
}

void lcd_send_string (char *str) {
     while (*str) { 
         lcd_send_data(*str++,1);
     }
}

void clear_lcd(void) {
	lcd_send_data( LCD_BIT_DISP_CLEAR,0);
    delay_100us(20);
    return;
}

void shift_lcd(void) {
	lcd_send_data( 0xc0, 0);
    delay_100us(1);
}

void cursor_home(void) {
	lcd_send_data( 0x02, 0);
    delay_100us(20);
}

void backoff_lcd(void) {
    //   RS RW D7 D6 D5 D4 D3 D2 D1 D0
    // -> 0  0  0  0  0  0  1  D  C  B <-D0 
    //Sets entire display (D) on/off,
    //cursor on/off (C), and
    //blinking of cursor position
    //character (B).
    //37 ms
	lcd_send_data(LCD_BIT_DISPLAY_CONTROL | LCD_BIT_DISPLAY_ON,0);
    delay_100us(4);
    return;
}

void backon_lcd(void) {
	lcd_send_data(LCD_BIT_DISPLAY_CONTROL ,0);
    delay_100us(4);
    return;
}

void init_lcd(void) {
    
    lcd_send_data(0x30,0);   
    delay_100us(60); 
  
    lcd_send_data(0x30,0);   
    delay_100us(30); 
    lcd_send_data(0x30,0);   
    delay_100us(30); 
   
    lcd_send_data(0x20,0);   
    delay_100us(30); 
    
    lcd_send_data(0x20,0);   
    delay_100us(30); 

    return;
}

void welcome(char *l1, char *l2,int clear) {
    backoff_lcd();
    cursor_home();
    clear_lcd();

    delay_100us(600);
    if ( l1!=NULL ) 
    lcd_send_string(l1);
    shift_lcd();
    if ( l2!=NULL ) 
    lcd_send_string(l2);
    delay_100us(16000);

    cursor_home();
    if ( clear ) 
	    clear_lcd();
    
    return;
}
