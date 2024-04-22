/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#include <stdio.h>
#include "hardware/i2c.h"
//#include "hardware/adc.h"
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

void drawMessage(char* m, int x, int y){
    int i = 0;
    while(m[i]){
        drawChar(x+i*5,y,m[i]);
        i++;
    }
    //update
}


/// \tag::uart_advanced[]

#define UART_ID uart0 //default pins
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1

static int chars_rxed = 0;

volatile int i = 0;
char m[100];


// RX interrupt handler
void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
        // Can we send it back?
        if(ch == '\n' || ch == '\r') {
            m[i] = 0;
            ssd1306_clear();
            drawMessage(m,0,0);
            ssd1306_update();
            i = 0;
            
        }
        else {
            m[i] = ch;
            i++;
        }


        if (uart_is_writable(UART_ID)) {
            // Change it slightly first!
            ch++;
            uart_putc(UART_ID, ch);
        }
        chars_rxed++;
    }
}

int main() {
    //initalization of the LED SCREEN
    stdio_init_all(); //inital chip initaliztion
    char message[30]; //initalize the empty character array
    
    ///adc_init(); //init the adc module
    //adc_gpio_init(26); //set the ADC0 pin to be adc input instead of GPIO
   //adc_select_input(0); // select to read from ADC0

    gpio_init(25);

    // I2C is "open drain", pull ups to keep signal high when no data is being sent
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);

    ssd1306_setup(); //initalize the screen, this will call the I2C read and write functions

    gpio_set_dir(25, GPIO_OUT);

    sleep_ms(250); // sleep so that data polling and register update don't collide

    
    
    
    // Set up our UART with a basic baud rate.
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART); //GP0
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART); //GP1

    // Actually, we want a different speed
    // The call will return the actual baud rate selected, which will be as close as
    // possible to that requested
    // int __unused actual = uart_set_baudrate(UART_ID, BAUD_RATE);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);

    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(UART_ID, false);

    // Set up a RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);

    // OK, all set up.
    // Lets send a basic string out, and then run a loop and wait for RX interrupts
    // The handler will count them, but also reflect the incoming data back with a slight change!
    // uart_puts(UART_ID, "\nHello, uart interrupts\n");

    while (1) {
        //tight_loop_contents();

        //blink gp 25 on the pico
        gpio_put(25, 1); //high turn it on
        sleep_ms(250);

        gpio_put(25, 0); //set it low turn it off
        sleep_ms(250);
    }
}

/// \end:uart_advanced[]
