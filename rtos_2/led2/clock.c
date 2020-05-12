#include "clock.h"
#include <stdio.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/pwr.h>

static void clock_set_time(const clock_time_t*);

void clock_init(void) {

	rcc_periph_clock_enable(RCC_PWR);
	rcc_periph_clock_enable(RCC_BKP);


	pwr_disable_backup_domain_write_protect();

	rcc_osc_on(RCC_LSE);
	rcc_enable_rtc_clock();
	rcc_set_rtc_clock_source(RCC_LSE);

	rtc_enter_config_mode();
	rcc_set_rtc_clock_source(RCC_LSE);
	rtc_set_prescale_val(0x7FFF);
	rtc_exit_config_mode();

	pwr_enable_backup_domain_write_protect();

}

void clock_set_time_ui(void){
	clock_time_t tm;
	printf("Enter day of the week [1-7]: ");
	scanf("%1hhd", &tm.dayOfWeek);
	printf("Enter number of hours [0-23]: ");
	scanf("%2hhd", &tm.hours);
	printf("Enter number of minutes [0-59]: ");
	scanf("%2hhd", &tm.minutes);
	printf("Enter number of seconds [0-59]: ");
	scanf("%2hhd", &tm.seconds);

	clock_set_time(&tm);
}

static void clock_set_time(const clock_time_t* tm){
	uint32_t time = 0;
	time += (tm->dayOfWeek - 1) * (60*60*24);
	time += tm->hours * (60*60);
	time += tm->minutes * 60;
	time += tm->seconds;

	rtc_enter_config_mode();
	rtc_set_counter_val(time);
	rtc_exit_config_mode();


}

void clock_get_time_ui(void){
	clock_time_t tm;
	clock_get_time(&tm);

	printf("%d  %02d:%02d:%02d\r\n",
			tm.dayOfWeek,
			tm.hours,
			tm.minutes,
			tm.seconds
	);
}

void clock_get_time(clock_time_t* tm){
	uint32_t time = rtc_get_counter_val();

	tm->raw = time;
	tm->dayOfWeek = ((time / (60*60*24)) % 7) + 1;
	tm->hours = (time / (60*60)) % 24;
	tm->minutes = (time / 60) % 60;
	tm->seconds = time % 60;
}

uint32_t clock_get_raw_time(void) {
	return rtc_get_counter_val();
}
