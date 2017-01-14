#ifndef _ADC_H_
#define _ADC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define ADC_PATH "/sys/bus/iio/devices/iio:device0/"

char adc_path[64];

void adc_init(int);
uint32_t adc_read(char *);


#endif
