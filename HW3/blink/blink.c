#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"


int main() {
    stdio_init_all();

    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }

    gpio_init(15); //initlaizes the pin to be used as a general purpose I/O
    gpio_set_dir(15, GPIO_OUT); //LED

    gpio_init(13);
    gpio_set_dir(13, GPIO_IN); //BUTTON

    printf("Start!\n");

    adc_init(); //init the adc module
    adc_gpio_init(26); //set the ADC0 pin to be adc input instead of GPIO
    adc_select_input(0); // select to read from ADC0

    gpio_put(15, 1); //turns the LED on

    while (1) {

        while(gpio_get(13) == 0) {
            
        }
        gpio_put(15, 0);

        char message[100];
        printf("Enter a fucking analog number between 1 and 100: ");
        scanf("%s", message);
        printf("message: %s\r\n",message);
        
        int num_times = atoi(message);

        printf("\r\n");
        for(int i = 0; i < num_times; i++) {
            int read = adc_read();
            float convert = (read * 3.3) / 4096;
            printf("Analog Reading: %f\r\n", convert);
            sleep_ms(10);
        }
        gpio_put(15, 1); //turns the LED on

        
        sleep_ms(50);
        
    }

}