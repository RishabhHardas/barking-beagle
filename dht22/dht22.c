#include <stdio.h>
#include "dht22.h"
#include "gpio.h"

void sleep_milliseconds(uint32_t millis)
{
	struct timeval deltatime;
	deltatime.tv_sec = millis / 1000;
	deltatime.tv_usec = (millis % 1000) * 1000;

	struct timeval walltime;
	gettimeofday(&walltime, NULL);

	struct timeval endtime;
	timeradd(&walltime, &deltatime, &endtime);

	while(timercmp(&walltime, &endtime, <))
	{
		gettimeofday(&walltime, NULL);
	}
}

void set_max_priority()
{
	struct sched_param sched;
	memset(&sched, 0, sizeof(sched));
	sched.sched_priority = sched_get_priority_max(SCHED_FIFO);
	if(sched_setscheduler(0, SCHED_FIFO, &sched) == -1)
	{
		printf("Could not set max priority!\n");
	}
}

void set_default_priority()
{
	struct sched_param sched;
	memset(&sched, 0, sizeof(sched));
	sched.sched_priority = 0;
	if(sched_setscheduler(0, SCHED_OTHER, &sched) == -1)
	{
		printf("Could not set default priority!\n");
	}
}

#define DHT_MAXCOUNTS 32000
#define DHT_PULSES 41

int bbb_dht_read(int gpio_base, int gpio_number, float* humidity, float* temperature)
{
	if(humidity == NULL || humidity == NULL)
	{
		return -1;
	}

	*temperature = 0.0;
	*humidity = 0.0;

	//Every Pulse is preceeded by 50usec pulse
	int pulseCounts[DHT_PULSES*2] = {0};

	gpio_map dht22_gpio;
	if(bbb_get_gpio(gpio_base, gpio_number, &dht22_gpio) != 0)
	{
		printf("Cannot get GPIO for DHT!\n");
		return -1;
	}

	bbb_set_gpio_output(&dht22_gpio);

	set_max_priority();


	bbb_gpio_high(&dht22_gpio);
	sleep_milliseconds(500);

	bbb_gpio_low(&dht22_gpio);
	sleep_milliseconds(20);

	bbb_set_gpio_input(&dht22_gpio);

	uint32_t count = 0;
	while(bbb_gpio_get_input(&dht22_gpio)) {
		if(++count >= DHT_MAXCOUNTS)
		{
			set_default_priority();
			return -1;
		}
	}

	printf("Counts = %d\n", count);
	// Record pulse widths for the expected result bits.
	int i = 0;
	for(i = 0; i < DHT_PULSES*2; i+=2)
	{
		//Count how long pin is low and store it in pulseCount
		while(!bbb_gpio_get_input(&dht22_gpio))
		{
			if(++pulseCounts[i] >= DHT_MAXCOUNTS)
			{
				set_default_priority();
				return -1;
			}
		}

		// Count how long pin is high and store in pulseCounts[i+1]
		while(bbb_gpio_get_input(&dht22_gpio))
		{
			if(++pulseCounts[i+1] >= DHT_MAXCOUNTS)
			{
				set_default_priority();
				return -1;
			}
		}
	}


	set_default_priority();


	uint32_t threshold = 0;
	i = 0;
	for(i = 2; i < DHT_PULSES*2; i +=2)
	{
		threshold += pulseCounts[i];
	}

	threshold /= DHT_PULSES - 1;

	uint8_t data[5] = {0};
	for(i = 3; i < DHT_PULSES*2; i += 2)
	{
		int index = (i-3)/16;
		data[index] <<= 1;
		if(pulseCounts[i] >= threshold)
		{
			data[index] |= 1;
		}
	}

	if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))
	{
      		// Calculate humidity and temp for DHT22 sensor.
		*humidity = (data[0] * 256 + data[1]) / 10.0f;
	      	*temperature = ((data[2] & 0x7F) * 256 + data[3]) / 10.0f;
	  	return 0;
  	}
  	else
	{
    		return -1;
	}

}
