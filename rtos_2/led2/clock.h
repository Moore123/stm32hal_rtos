#ifndef _CLOCK_H
#define _CLOCK_H

#include <stdint.h>

#if 0
void usb_comm_init();
int usb_comm_write(const char*, int);
int usb_comm_read(char*, int);
uint32_t usb_comm_read_int();
uint32_t usb_comm_read_available();
#endif 

typedef struct {
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t dayOfWeek;
	uint32_t raw;
} __attribute__((packed)) clock_time_t;


void clock_init(void);
void clock_set_time_ui(void);
void clock_get_time_ui(void);
void clock_get_time(clock_time_t*);
uint32_t clock_get_raw_time(void);

#endif
