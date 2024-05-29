#include <string.h> // for memset
#include <stdio.h>
#include "pico/stdlib.h"
//#include "pico/binary_info.h"
#include "hardware/pwm.h"

//Global Variables
float div = 50; // must be between 1-255
uint16_t wrap = 50000; // when to rollover, must be less than 65535 --> this is the PWM precision, for 100% duty cycle you will use 50,000

  int main() {
  stdio_init_all(); //Sets up genral turn on for the pico

  //Frequency is (125 MHz / Div) / Wrap
  //Duty Cycle is the percentage of Wrap 
  
  //This if for the PWM pins on the pic
  #define FIRSTPWMPIN 18
  #define SECONDPWMPIN 19
  #define FIRSTDIGITAL 20
  #define SECONDDIGITAL 21

  //First Pin Initalization and Declaration
  gpio_set_function(FIRSTPWMPIN, GPIO_FUNC_PWM); // Set the GPIO to be PWM
  uint slice_num = pwm_gpio_to_slice_num(FIRSTPWMPIN); // Get PWM slice number
  //float div = 50; // must be between 1-255
  pwm_set_clkdiv(slice_num, div); // divider
  //uint16_t wrap = 50000; // when to rollover, must be less than 65535
  pwm_set_wrap(slice_num, wrap);
  pwm_set_enabled(slice_num, true); // turn on the PWM
  //float duty_cycle_percentage = 0.025; 


  //Second Pin Initalization and Declaration
  gpio_set_function(SECONDPWMPIN, GPIO_FUNC_PWM); // Set the GPIO to be PWM
  slice_num = pwm_gpio_to_slice_num(SECONDPWMPIN); // Get PWM slice number
  //float div = 50; // must be between 1-255
  pwm_set_clkdiv(slice_num, div); // divider
  //uint16_t wrap = 50000; // when to rollover, must be less than 65535
  pwm_set_wrap(slice_num, wrap);
  pwm_set_enabled(slice_num, true); // turn on the PWM
  //duty_cycle_percentage = 0.025;

  //Digital Output Initalization 
  gpio_init(FIRSTDIGITAL);
  gpio_set_dir(FIRSTDIGITAL, GPIO_OUT); //GPIO_OUT the pico will output either 1 or 0 --> 1 is reverse and 0 is forward
  gpio_init(SECONDDIGITAL);
  gpio_set_dir(SECONDDIGITAL, GPIO_OUT); 
 

  while(1) {

    pwm_set_gpio_level(FIRSTPWMPIN, wrap); //Make the first motor go forward at full speed
    gpio_put(FIRSTDIGITAL, 0);

    pwm_set_gpio_level(SECONDPWMPIN, 0); //Make the second motor go backward at full speed
    gpio_put(SECONDDIGITAL, 1);



    // pwm_set_gpio_level(LEDPin, wrap*0.025); 
    // sleep_ms(1000);
    // pwm_set_gpio_level(LEDPin, wrap*0.125);
    // sleep_ms(1000);

    // for(int i = 0; i < 100; i++) {
    // pwm_set_gpio_level(LEDPin, wrap * duty_cycle_percentage); // set the duty cycle to 50%
    // duty_cycle_percentage += .001;
    // sleep_ms(10);
    // }
  }
  
}