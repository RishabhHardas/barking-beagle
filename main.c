#include <stdio.h>
#include "gpio.h"
#include "dht22.h"

int main()
{
	//gpio_map mygpio;
	float temperature = 0.0;
	float humidity = 0.0;
	while(1)
	{
		bbb_dht_read(GPIO0, 20, &humidity, &temperature);
		printf("Temperature = %f\n", temperature);
		printf("Humidity = %f\n", humidity);
		sleep(2);
	}

}
