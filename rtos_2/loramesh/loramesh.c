/* CDC Demo, using the usbcdc library
 * Warren W. Gay VE3WWG
 * Wed Mar 15 21:56:50 2017
 *
 * This demo consists of a text menu driven app, to display
 * STM32F103 registers (STM32F103C8T6 register set assumed).
 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "mcuio.h"
#include "miniprintf.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "common.h"
#include "tips.h"

static QueueHandle_t uart_txq;		// TX queue for UART
static void uart_setup(void) {

	open_uart(1,9600,"8N1","rw",1,1);
	open_uart(2,9600,"8N1","rw",1,1);
	uart_txq = xQueueCreate(256,sizeof(char));
}


/*
 * Monitor task:
 */
char disp_buff[32];

static void usbmonitor_task(void *arg __attribute((unused))) {

	for (;;) {
		vTaskDelay(pdMS_TO_TICKS(500));
        
		//monitor();
    }
}


static void leddisp_task(void *args __attribute((unused))) {
   char x[16]; 
   int xcnt=0,xidx=0;
	for (;;) {
		gpio_toggle(GPIOC,GPIO13);
        backoff_lcd();
        clear_lcd();
        cursor_home();
        sprintf(x,"Recv MSG: %04d",xcnt);
        //lcd_send_string(x); 
        shift_lcd();
		
        //lcd_send_string(disp_buff); 
        
        memset(disp_buff,0x0,16);
        if ( xcnt > 99999 ) xcnt = 0;
		vTaskDelay(pdMS_TO_TICKS(1000));
        backoff_lcd();
	}
}

static inline void uart_putc(char ch) {
	usart_send_blocking(USART1,ch);
}

static void lora_send(void *args __attribute__((unused))) {

    int xcnt;

	for (;;) {

        cursor_home();
        xcnt++;
        if ( xcnt>99999 ) xcnt = 1; 
        sprintf(disp_buff, "LoraSent %05d",xcnt);
        lcd_send_string(disp_buff);
//	    puts_uart(1,disp_buff);
    
 //       memset(disp_buff,0x0,16); 
        
		gpio_toggle(GPIOC,GPIO13);
   
		//vTaskDelay(pdMS_TO_TICKS(2000));
        delay_100us(20000);
	}
}

static void lora_recv(void *args __attribute__((unused))) {
	char ch=0;
    int xidx = 0;

	for (;;) {

/*		if ( xQueueReceive(uart_txq,&ch,500) == pdPASS ) {
			while ( !usart_get_flag(USART1,USART_SR_TXE) )
				taskYIELD();	// Yield until ready
	        disp_buff[xidx] = ch;
            if ( ++xidx >= 16 ) xidx = 0;
		} */
	 uart1_gets(disp_buff,sizeof disp_buff-1);
   /*  if ( ( ch != '0xa' ) & ( ch!= '0xd' ) ) {
		ch = uart_getc(1);
        disp_buff[xidx] = ch;
        xidx++;
        continue;
     }
        xidx = 0; */
        lcd_send_string(disp_buff);
		gpio_toggle(GPIOC,GPIO13);
        delay_100us(100); 
        for(xidx=0; xidx<(sizeof disp_buff); xidx++ ) disp_buff[xidx]=0x0;
	   //if ( xQueueReceive(uart_txq,&ch,10) == pdPASS ) ;
	}
}

int main(void) {

	rcc_clock_setup_in_hse_8mhz_out_72mhz();	// Use this for "blue pill"

    rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPAEN |
                     RCC_APB2ENR_IOPBEN |
                     RCC_APB2ENR_IOPCEN |
                     RCC_APB2ENR_AFIOEN);

	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_set_mode( GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
	rcc_periph_clock_enable(RCC_GPIOB);
    gpio_set_mode(GPIOB,GPIO_MODE_OUTPUT_2_MHZ,GPIO_CNF_OUTPUT_PUSHPULL,GPIO6|GPIO7|GPIO8|GPIO9|GPIO12|GPIO13);

    timer_setup();
    uart_setup();
    init_lcd();
    delay_100us(8000);

    welcome("LoraMesh","      Ready....",1);

	xTaskCreate(lora_recv,"UART",100,NULL,configMAX_PRIORITIES-1,NULL);
//	xTaskCreate(usbmonitor_task,"monitor",100,NULL,configMAX_PRIORITIES-1,NULL);
//	xTaskCreate(leddisp_task,"LED",100,NULL,configMAX_PRIORITIES-4,NULL);

//	usb_start(0,configMAX_PRIORITIES-1);
	gpio_clear(GPIOC,GPIO13);
//	std_set_device(mcu_usb);			// Use USB for std I/O

	vTaskStartScheduler();
	for (;;);
}

/* End main.c */
