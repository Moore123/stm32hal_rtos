/* Task based UART demo, using queued communication.
 *
 *	get the wiring correct.
 */
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include "common.h"

#include "uartlib.h"

#include "tips.h"


#define BUFFLEN  512

static QueueHandle_t uart_txq;	// TX queue for UART

int strncmp(const char *src, const char *dst, size_t len);


static void uart_setup(void)
{

    rcc_periph_clock_enable(RCC_USART3);
	gpio_set_mode(GPIOB,GPIO_MODE_OUTPUT_10_MHZ,GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,GPIO_USART3_TX);
	gpio_set_mode(GPIOB,GPIO_MODE_OUTPUT_10_MHZ,GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,GPIO10);

	gpio_set_mode(GPIOB,GPIO_MODE_INPUT,GPIO_CNF_INPUT_FLOAT,GPIO_USART3_RX);
	gpio_set_mode(GPIOB,GPIO_MODE_INPUT,GPIO_CNF_INPUT_FLOAT,GPIO11);
	
	open_uart(3, 9600, "8N1", "rw", 1, 1);

	// Create a queue for data to transmit from UART
	uart_txq = xQueueCreate(BUFFLEN, sizeof(char));
}

/*********************************************************************
 * USART Task: 
 *********************************************************************/
static void time_parser(void *args __attribute__ ((unused)))
{
	char ch, buff[BUFFLEN];
	int gc;

    for( gc = 0 ; gc < BUFFLEN; gc++ ) buff[gc] = 0x0;
	for (;;) {
       puts_uart(3, "\r\nRMC 0 OK: [\r\n");
       puts_uart(3, "\r\nRMC 3 OK: [\r\n");
	   vTaskDelay(pdMS_TO_TICKS(800));
    }
}


/*********************************************************************
 * Demo Task:
 *	Simply queues up two line messages to be TX, one second
 *	apart.
 *********************************************************************/
static void blink13_task(void *args __attribute__ ((unused)))
{
    
	for (;;) {
		gpio_toggle(GPIOC, GPIO13);
		vTaskDelay(pdMS_TO_TICKS(500));
    }
	
}

#define LCD_BIT_RS                 ((uint8_t)0x01U)
#define LCD_BIT_RW                 ((uint8_t)0x02U)
#define LCD_BIT_E                  ((uint8_t)0x04U)
#define LCD_BIT_BACKIGHT_ON        ((uint8_t)0x08U)
#define LCD_BIT_BACKIGHT_OFF       ((uint8_t)0x00U)

#define LCD_MODE_4BITS             ((uint8_t)0x02U)
#define LCD_BIT_1LINE              ((uint8_t)0x00U)
#define LCD_BIT_2LINE              ((uint8_t)0x08U)
#define LCD_BIT_4LINE              LCD_BIT_2LINE
#define LCD_BIT_5x8DOTS            ((uint8_t)0x00U)
#define LCD_BIT_5x10DOTS           ((uint8_t)0x04U)
#define LCD_BIT_SETCGRAMADDR       ((uint8_t)0x40U)
#define LCD_BIT_SETDDRAMADDR       ((uint8_t)0x80U)

#define LCD_BIT_DISPLAY_CONTROL    ((uint8_t)0x08U)
#define LCD_BIT_DISPLAY_ON         ((uint8_t)0x04U)
#define LCD_BIT_CURSOR_ON          ((uint8_t)0x02U)
#define LCD_BIT_CURSOR_OFF         ((uint8_t)0x00U)
#define LCD_BIT_BLINK_ON           ((uint8_t)0x01U)
#define LCD_BIT_BLINK_OFF          ((uint8_t)0x00U)

#define LCD_BIT_DISP_CLEAR         ((uint8_t)0x10U)
#define LCD_BIT_CURSOR_HOME        ((uint8_t)0x20U)

#define LCD_BIT_ENTRY_MODE         ((uint8_t)0x04U)
#define LCD_BIT_CURSOR_DIR_RIGHT   ((uint8_t)0x02U)
#define LCD_BIT_CURSOR_DIR_LEFT    ((uint8_t)0x00U)
#define LCD_BIT_DISPLAY_SHIFT      ((uint8_t)0x01U)

static void send_to_lcd(const char val,unsigned char rs) {

  ( val & 0x1 ) ?  gpio_set(GPIOB, GPIO8) : gpio_clear(GPIOB, GPIO8);
  ( val & 0x2 ) ?  gpio_set(GPIOB, GPIO5) : gpio_clear(GPIOB, GPIO5);
  ( val & 0x4 ) ?  gpio_set(GPIOB, GPIO6) : gpio_clear(GPIOB, GPIO6);
  ( val & 0x8 ) ?  gpio_set(GPIOB, GPIO7) : gpio_clear(GPIOB, GPIO7);

  ( rs & 0x1 ) ?  gpio_set(GPIOA, GPIO8) : gpio_clear(GPIOA, GPIO8);

   gpio_set(GPIOA, GPIO10);
   delay_100us(1);
   gpio_clear(GPIOA, GPIO10);
/*
   gpio_clear(GPIOB, GPIO5);
   gpio_clear(GPIOB, GPIO6);
   gpio_clear(GPIOB, GPIO7);
   gpio_clear(GPIOB, GPIO8); 
   delay_100us(10); */
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


static void clear_lcd() {
	lcd_send_data( LCD_BIT_DISP_CLEAR,0);
    delay_100us(20);
    return;
}

static void shift_lcd() {
	lcd_send_data( 0xc0, 0);
    delay_100us(1);
}

static void cursor_home() {
	lcd_send_data( 0x02, 0);
    delay_100us(20);
}

static void lcd_backoff() {
	lcd_send_data(LCD_BIT_BLINK_OFF,0);
    delay_100us(1);
}

static void init_lcd() {
    
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



/*********************************************************************
 * Main program & scheduler:
 *********************************************************************/
int main(void)
{

	rcc_clock_setup_in_hse_8mhz_out_72mhz();	// CPU clock is 72 MHz
//    rcc_periph_clock_enable(RCC_GPIOA);


    rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPAEN |
                     RCC_APB2ENR_IOPBEN |
                     RCC_APB2ENR_IOPCEN |
                     RCC_APB2ENR_AFIOEN);

	gpio_set_mode(GPIOA,GPIO_MODE_OUTPUT_10_MHZ,GPIO_CNF_OUTPUT_PUSHPULL,GPIO8|GPIO10);
    //rcc_periph_clock_enable(RCC_AFIO);
    //rcc_periph_clock_enable(RCC_GPIOB);
	gpio_set_mode(GPIOB,GPIO_MODE_OUTPUT_10_MHZ,GPIO_CNF_OUTPUT_PUSHPULL,GPIO8|GPIO5|GPIO6|GPIO7|GPIO4);

	// GPIO PC13:
	//rcc_periph_clock_enable(RCC_GPIOC);
	gpio_set_mode(GPIOC,
		 GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);

    gpio_primary_remap(0,0);

    timer_setup();
    delay_100us(9000);
    uart_setup();
    /*
    gpio_clear(GPIOB, GPIO5);
    gpio_clear(GPIOB, GPIO6);
    gpio_clear(GPIOB, GPIO7);
    gpio_clear(GPIOB, GPIO8);
    gpio_clear(GPIOA, GPIO8);
    gpio_clear(GPIOA, GPIO10); */
    delay_100us(4000);
    init_lcd();
    //clear_lcd();
 //   lcd_backoff();
//    delay_ms(1000);
/*     lcd_send_data('G',1);
     lcd_send_data('o',1);
     lcd_send_data('o',1);
     lcd_send_data('d',1);
     lcd_send_data('!',1); */
    lcd_send_string("Hello there!");
/*
    for(;;) {
        continue;
      send_to_lcd(0x3);
      delay_ms(2000);
      send_to_lcd(0xc);
      delay_ms(2000);
    }    
*/

	xTaskCreate(time_parser, "UART", 400, NULL, configMAX_PRIORITIES - 2, NULL);
	xTaskCreate(blink13_task, "LED", 200, NULL, configMAX_PRIORITIES - 1, NULL);

	vTaskStartScheduler();
	for (;;) ;
	return 0;
}

/* End */

