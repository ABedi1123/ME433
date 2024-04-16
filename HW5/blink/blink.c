/**
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 **/

#include <stdio.h>
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

 // device has default bus address of 0x76
#define ADDR _u(0b0100000)

// hardware registers
#define REG_IODIR _u(0x00)
#define REG_GPIO _u(0x09)
#define REG_OLAT _u(0x0A)

void chip_init() {
    // use the "handheld device dynamic" optimal setting (see datasheet)
    uint8_t buf[2];

    // send register number followed by its corresponding value
    buf[0] = REG_IODIR;
    buf[1] = 0b01111111;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false); //start bit and writes all the data and read all the data back
}

void set(char v) {
    // use the "handheld device dynamic" optimal setting (see datasheet)
    uint8_t buf[2];

    // send register number followed by its corresponding value
    buf[0] = REG_OLAT;
    buf[1] = v << 7;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false); //start bit and writes all the data and read all the data back
}

int read() {

    uint8_t buf[1];
    uint8_t reg = REG_GPIO;
    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);  // true to keep master control of bus
    i2c_read_blocking(i2c_default, ADDR, buf, 1, false);  // false - finished with bus

    if(buf[0]&0b1 == 0b1) {
        return 0; //turn LED off
    }
    else {
        return 1; //turn LED on
    }
}

int main() {
    stdio_init_all();

    gpio_init(25);

    // I2C is "open drain", pull ups to keep signal high when no data is being sent
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);

    chip_init(); //gp7 is output, gp0 is input

    gpio_set_dir(25, GPIO_OUT);

    sleep_ms(250); // sleep so that data polling and register update don't collide
    while (1) {
        //blink gp 25 on the pico
        gpio_put(25, 1); //high turn it on
        sleep_ms(100);

        gpio_put(25, 0); //set it low turn it off
        sleep_ms(100);
        
        //read from gp0
        if(read()) {
            //set gp7 high
            set(1);

        }
        
        else {
            //set gp7 low
            set(0);
        }
    }
}
