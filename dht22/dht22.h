#ifndef _DHT22_H_
#define _DHT22_H_

#include <stdlib.h>
#include <errno.h>
#include <sched.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
//#include <sys/mman.h>
#include <fcntl.h>
#include "gpio.h"

void sleep_milliseconds(uint32_t millis);
void set_max_priority(void);
void set_default_priority(void);

int bbb_dht_read(int gpio_base, int gpio_number, float* humidity, float* temperature);

#endif
