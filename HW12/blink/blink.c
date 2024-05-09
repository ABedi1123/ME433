#include <string.h> // for memset
#include <stdio.h>
#include "pico/stdlib.h"
//#include "pico/binary_info.h"
#include "hardware/pwm.h"


  int main() {
  stdio_init_all(); //Sets up genral turn on for the pico

  //Frequency is (125 MHz / Div) / Wrap
  //Duty Cycle is the percentage of Wrap 
  
  #define LEDPin 17 // the built in LED on the Pico
  gpio_set_function(LEDPin, GPIO_FUNC_PWM); // Set the LED Pin to be PWM
  uint slice_num = pwm_gpio_to_slice_num(LEDPin); // Get PWM slice number
  float div = 50; // must be between 1-255
  pwm_set_clkdiv(slice_num, div); // divider
  uint16_t wrap = 50000; // when to rollover, must be less than 65535
  pwm_set_wrap(slice_num, wrap);
  pwm_set_enabled(slice_num, true); // turn on the PWM
  float duty_cycle_percentage = 0.025; 

  while(1) {

    pwm_set_gpio_level(LEDPin, wrap*0.025); 
    sleep_ms(1000);
    pwm_set_gpio_level(LEDPin, wrap*0.125);
    sleep_ms(1000);

    // for(int i = 0; i < 100; i++) {
    // pwm_set_gpio_level(LEDPin, wrap * duty_cycle_percentage); // set the duty cycle to 50%
    // duty_cycle_percentage += .001;
    // sleep_ms(10);
    // }
  }
  
}