#ifndef _GPIO_H_
#define _GPIO_H_


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <sys/mman.h>
#include <fcntl.h>


//BBB ha 4 GPIO Chips, the Addresses are as defined below.
//Size for each address is 4KB
#define GPIO_SIZE 4096
#define GPIO0   0x44E07000
#define GPIO1   0x4804C000
#define GPIO2   0x481AC000
#define GPIO3   0x481AE000


typedef struct {
        uint32_t* base_address;
        char    number;
} gpio_map;


int bbb_get_gpio(uint32_t base_addr, int number,gpio_map *gpio);
void bbb_set_gpio_input(gpio_map *gpio);
void bbb_set_gpio_output(gpio_map *gpio);
void bbb_gpio_high(gpio_map *gpio);
void bbb_gpio_low(gpio_map *gpio);
uint32_t bbb_gpio_get_input(gpio_map *gpio);


#endif
