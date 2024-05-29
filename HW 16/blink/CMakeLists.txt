add_executable(blink blink.c)

target_sources(blink PRIVATE blink.c)

# pull in common dependencies
target_link_libraries(blink pico_stdlib pico_binary_info hardware_pwm)



# create map/bin/hex file etc.
pico_add_extra_outputs(blink)