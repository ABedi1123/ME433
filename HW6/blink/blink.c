#include <stdio.h>
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "ssd1306.h"
#include "font.h"

void drawChar(int x, int y, char letter) {
    int i = 0;
    for(i = 0; i <5; i++) {
        char c = ASCII[letter-32][i]; //represents a letter we are talking about, 2D array
        for(int j = 0; j<8; j++) {
            char bit = (c>>j)&0b1;

            if(bit == 0b1) {
                ssd1306_drawPixel(x + i, y + j, 1); //x has to be shifted by i
            }
            else {
                ssd1306_drawPixel(x + i, y + j, 0);
            }
        }
    }
}

void drawMessage(int x, int y, char* m){
    int i = 0;
    while(m[i]){
        drawChar(x+i*5,y,m[i]);
        i++;
    }
    //update
}

int main() {
    
    stdio_init_all(); //inital chip initaliztion
    char message[30]; //initalize the empty character array
    
    adc_init(); //init the adc module
    adc_gpio_init(26); //set the ADC0 pin to be adc input instead of GPIO
    adc_select_input(0); // select to read from ADC0

    gpio_init(25);

    // I2C is "open drain", pull ups to keep signal high when no data is being sent
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);

    ssd1306_setup(); //initalize the screen, this will call the I2C read and write functions

    gpio_set_dir(25, GPIO_OUT);

    sleep_ms(250); // sleep so that data polling and register update don't collide

    while(true) {
        
        //blink gp 25 on the pico
        gpio_put(25, 1); //high turn it on
        sleep_ms(250);

        gpio_put(25, 0); //set it low turn it off
        
        unsigned int start = to_us_since_boot(get_absolute_time());

        int read = adc_read();
        float convert = (read * 3.3) / 4096;
        
        sprintf(message, "ADC Voltage = %f", convert);
        drawMessage(1,1,message); //row 2 column 2
        ssd1306_update();
        
        unsigned int stop = to_us_since_boot(get_absolute_time());

        unsigned int i = stop - start;
        sprintf(message, "FPS = %f", 1000000.0/i); //units of stop and start are microseconds, FPS = Frames Per Second
        drawMessage(1,10,message); //x is column and y is row and start with the 0th index so its 3 row, second column
        ssd1306_update();
        sleep_ms(250);
    }
    
}