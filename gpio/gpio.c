#include "gpio.h"

int bbb_get_gpio(uint32_t base_addr, int number,gpio_map *gpio)
{
        int fd =0;
        if ((base_addr != GPIO0) && (base_addr != GPIO1) && (base_addr != GPIO2) && (base_addr != GPIO3))
        {
                printf("Invalid base address of GPIO!\n");
                return -1;
        }

        fd = open("/dev/mem", O_RDWR | O_SYNC);
        if(fd == -1)
        {
                printf("Error Opening /dev/mem\n");
                return -1;
        }

        gpio->base_address = (uint32_t*)mmap(NULL, GPIO_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, base_addr);
        if(gpio->base_address ==  MAP_FAILED)
        {
                printf("Mapping Failed!\n");
                return -1;
        }

        gpio->number = number;

return 0;
}

void bbb_set_gpio_input(gpio_map *gpio)
{
        uint32_t *in_address = (gpio->base_address + (0x134/4));
        *in_address |= (1 << gpio->number);
}

void bbb_set_gpio_output(gpio_map *gpio)
{
        uint32_t *out_address = (gpio->base_address + (0x134/4)) ;
        *out_address &= (0xFFFFFFFF ^ (1 << gpio->number));
}

void bbb_gpio_high(gpio_map *gpio)
{
        uint32_t *high_address = (gpio->base_address + (0x194/4));
        *high_address = (1 << gpio->number);
}

void bbb_gpio_low(gpio_map *gpio)
{
        uint32_t *low_address = (gpio->base_address + (0x190/4));
        *low_address = (1 << gpio->number);
}

uint32_t bbb_gpio_get_input(gpio_map *gpio)
{
        return gpio->base_address[0x138/4] & (1 << gpio->number);
}


