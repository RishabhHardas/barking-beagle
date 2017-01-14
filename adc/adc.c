#include "adc.h"

void adc_init(int adc)
{
        strcat(adc_path, ADC_PATH);
        //printf("ADC selected is %d\n", adc);

        switch (adc){
                case 0:
                        strcat(adc_path, "in_voltage0_raw");
                        break;
                case 1:
                        strcat(adc_path, "in_voltage1_raw");
                        break;
                case 2:
                        strcat(adc_path, "in_voltage2_raw");
                        break;
                case 3:
                        strcat(adc_path, "in_voltage3_raw");
                        break;
                case 4:
                        strcat(adc_path, "in_voltage4_raw");
                        break;
                case 5:
                        strcat(adc_path, "in_voltage5_raw");
                        break;
                case 6:
                        strcat(adc_path, "in_voltage6_raw");
                        break;
                case 7:
                        strcat(adc_path, "in_voltage7_raw");
                        break;
                default:
                        break;
        }
}


uint32_t adc_read(char *adc_path)
{
        char adc_val[5] = {'\0'};
        FILE *adc_handler = fopen(adc_path, "r");
        if(adc_handler == NULL)
        {
                printf("Cannot open ADC Path!\n");
                return -1;
        }

        fread(adc_val, 4, 1, adc_handler);
        return atoi(adc_val);

}
