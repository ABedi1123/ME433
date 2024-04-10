/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

//SPI is super fast baud rate, but it takes a lot of pins to talk to chips, ideally you obly have like one or two things for the SPI

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
//#include "pico/binary_info.h"
#include "hardware/spi.h"
#include "math.h"

static inline void cs_select() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    asm volatile("nop \n nop \n nop");
}

static void write_register(uint8_t channel, uint16_t volt) {
    uint8_t buf[2];
	
    uint temp = 0; //intializng the 16-bit number
    
    temp |= volt << 2; //this is just asigning a ten bit number on to the unsigned integer to temp

    temp |= 0b111 << 12; //this puts 111 in the 13th, 14th, and 15th place

    temp |= channel << 15; //this will assign chanell A or B 

    buf[0] = temp >> 8; //for the first 8-bits let me pass in the temp1
    buf[1] = temp; //for the second 8-bits lets me pass in the temp2

    cs_select();
    spi_write_blocking(spi_default, buf, 2);
    cs_deselect();
    
}

int main() {
    stdio_init_all();
	
    printf("Hello, ME433! Reading raw data from registers via SPI...\n");

    // This example will use SPI0 at 0.5MHz.
    spi_init(spi_default, 500 * 1000);  //baud rate
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    
    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);

    uint triangle_wave[1000]; //needs to be an array which will give the reference for the Oscillscope wave to read
	uint sine_wave[500];
	
	uint vol_high = 1023;
	   
	        //Sine Wave generation
            for (int i = 0; i < 500; i++)
	        {
	            float radians = sin(M_PI * 2 * ((float)i / 499));
	            sine_wave[i] = (radians + 1) * vol_high / 2;
	        }
	    
	uint step = vol_high / 500;

	        //Triangle Wave generation
            for (int i = 0; i < 500; i++)
	        {
	            triangle_wave[i] = step * i;
	        }
	        for (int i = 0; i < 500; i++)
	        {
	            triangle_wave[500 + i] = vol_high - step * i;
	        }
	    

    while (1) {
       for(int i = 0; i < 1000; i++) {
        //pass the channel and the voltage in 
        if(i <= 499) {
             write_register(0, sine_wave[i]); //channel A 
        }
        else {
            write_register(0, sine_wave[i - 500]); //channel A
        }

        write_register(1, triangle_wave[i]);

        sleep_ms(1);

       }

    }
}
