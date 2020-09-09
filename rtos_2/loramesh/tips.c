#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <math.h>

#include <sys/time.h>

#include "uartlib.h"

#include "tips.h"

#define  strncmp __builtin_strncmp
#undef strncmp

int strncmp(const char *src, const char *dst, size_t len)
{

	size_t i = 0;

	while (*(src + i) == *(dst + i)) {
		if (i++ == len)
			return (0);
		if ((*(src + i) == 0x0) || (*(dst + i) == 0x0))
			return (0);
	}
	if (*(src + i) > *(dst + i))
		return (1);
	return (-1);
}

uint8_t verify_validly(const char *src)
{
	int i = 1, j, retval = 0, cmv = 0;

	while (*(src + i++) != '*') {
		retval ^= *(src + i - 1);
	}

	while (*(src + i) != 0x0 && *(src + i) != 0xd
	       && *(src + i) != 0x20 && *(src + i) != 0xa) {

		j = *(src + i++) - '0';

		if (j > 10)
			j -= 7;
		else {
			cmv = cmv * 0x10 + j;
			continue;
		}

		if (j > 0xF)
			j -= 0x20;
		if (j > 0xF)
			continue;
		cmv = cmv * 0x10 + j;
	}

	return ((cmv == retval) ? 0 : 1);
}


uint8_t TM_GPS_INT_Atoi(char *str, uint32_t * val)
{
	uint8_t count = 0;
	*val = 0;
	while (GPS_IS_DIGIT(*str)) {
		*val = *val * 10 + GPS_C2N(*str++);
		count++;
	}
	return count;
}

uint8_t TM_GPS_INT_Hex2Dec(char c)
{
	if (c >= '0' && c <= '9') {
		return c - '0';	/* 0 - 9 */
	} else if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;	/* 10 - 15 */
	} else if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;	/* 10 - 15 */
	}
	return 0;
}


void timer_setup(void) {
  rcc_periph_clock_enable(RCC_TIM3);
  rcc_periph_reset_pulse(RST_TIM3);
  timer_set_mode(TIM3,TIM_CR1_CKD_CK_INT_MUL_4,TIM_CR1_CMS_EDGE,TIM_CR1_DIR_UP);
  timer_set_prescaler(TIM3,360);          // Clock counts every 5 usec
  timer_one_shot_mode(TIM3);
  timer_set_period(TIM3,20);              // 20 * 5 usec => 100 usec
}


void delay_100us(unsigned int val) {
 unsigned int i;
 for ( i = 0 ; i < val ; i++ ) {
  timer_clear_flag(TIM3,TIM_SR_UIF);      // UIF=0
  timer_enable_counter(TIM3);             // Start timer
  while ( !timer_get_flag(TIM3,TIM_SR_UIF) )
  ;                               // Until UIF=1
 }
}

