/* Demo of I2C I/O with PCF8574 device
 * Warren Gay Sat Dec  9 17:36:29 2017
 *
 *	PCF8574 /INT	on PC14
 *	LED		on PC13
 *	I2C		on PB6, PB7
 */
#include <string.h>
#include <stdbool.h>
#include <setjmp.h>

#include "FreeRTOS.h"
#include "task.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/exti.h>

#include <libopencm3/cm3/nvic.h>

#include "lcd_hd44780_i2c.h"

uint32_t SystemCoreClock;
#define PCF8574_ADDR(n)		(0x20|((n)&7))	// PCF8574
// #define PCF8574_ADDR(n)	(0x38|((n)&7))	// PCF8574A
//	uint8_t addr = PCF8574_ADDR(0);	// I2C Address

//static I2C_Control i2c;			// I2C Control struct
//static LCD_HandleTypeDef lcddisp;
static volatile bool readf = false; 	// ISR flag
static volatile int isr_count = 0;	// ISR count

/*********************************************************************
 * EXTI15 ISR
 *********************************************************************/

void
exti15_10_isr() {

	++isr_count;
	gpio_toggle(GPIOC,GPIO13);
	exti_reset_request(EXTI14);	// Reset cause of ISR
	readf = true;			// Indicate data change
}


/*********************************************************************
 * Demo task 1
 *********************************************************************/
#if 0
static void
task1(void *args __attribute__((unused))) {
    uint16_t value=0;
    uint8_t i;

    char *hstr="Hello There";

	vTaskDelay(pdMS_TO_TICKS(1000));

}
#endif
/*********************************************************************
 * Demo Task:
 *	Simply queues up two line messages to be TX, one second
 *	apart.
 *********************************************************************/
static void blink13_task(void *args __attribute__ ((unused)))
{
    
	for (;;) {
		gpio_toggle(GPIOC, GPIO13);
		vTaskDelay(pdMS_TO_TICKS(2000));
    }
	
}

static void
task1(void *args __attribute((unused))) {

	for (;;) {
		gpio_toggle(GPIOC,GPIO13);
         lcdPrintStr((uint8_t*)"Hello,", 6);  // for compile test
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

int
main(void) {

	rcc_clock_setup_in_hse_8mhz_out_72mhz(); // For "blue pill"

    SystemCoreClock = 72000000U;
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_set_mode(
		GPIOC,
		GPIO_MODE_OUTPUT_2_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL,
		GPIO13);

	xTaskCreate(task1,"LED",100,NULL,configMAX_PRIORITIES-1,NULL);
	vTaskStartScheduler();

	for (;;);
	return 0;
}

// End
/*********************************************************************
 * Main routine and peripheral setup:
 *********************************************************************/

#if 0
int
main(void) {

	rcc_clock_setup_in_hse_8mhz_out_72mhz(); // For "blue pill"

	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_set_mode(
		GPIOC,
		GPIO_MODE_OUTPUT_2_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL,
		GPIO13);

	xTaskCreate(task1,"LED",100,NULL,configMAX_PRIORITIES-1,NULL);
	vTaskStartScheduler();
	rcc_clock_setup_in_hse_8mhz_out_72mhz();// For "blue pill"
	rcc_periph_clock_enable(RCC_GPIOB);	// I2C
	rcc_periph_clock_enable(RCC_GPIOC);	// LED
	rcc_periph_clock_enable(RCC_AFIO);	// EXTI
	rcc_periph_clock_enable(RCC_I2C1);	// I2C

	gpio_set_mode(GPIOB,
		GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
		GPIO6|GPIO7);			// I2C
	gpio_set(GPIOB,GPIO6|GPIO7);		// Idle high

	gpio_set_mode(GPIOC,
		GPIO_MODE_OUTPUT_2_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL,
		GPIO13);			// LED on PC13
	gpio_set(GPIOC,GPIO13);			// PC13 LED dark

	// AFIO_MAPR_I2C1_REMAP=0, PB6+PB7
	gpio_primary_remap(0,0); 

	gpio_set_mode(GPIOC,			// PCF8574 /INT
		GPIO_MODE_INPUT,		// Input
		GPIO_CNF_INPUT_FLOAT,
		GPIO14);			// on PC14

	exti_select_source(EXTI14,GPIOC);
	exti_set_trigger(EXTI14,EXTI_TRIGGER_FALLING);
	exti_enable_request(EXTI14);
	nvic_enable_irq(NVIC_EXTI15_10_IRQ);	// PC14 <- /INT
	xTaskCreate(task1,"task1",100,NULL,configMAX_PRIORITIES-1 ,NULL);
	xTaskCreate(blink13_task, "LED", 400, NULL, configMAX_PRIORITIES - 2, NULL);

	vTaskStartScheduler();

	for (;;);
	return 0;
}
#endif

// End main.c
