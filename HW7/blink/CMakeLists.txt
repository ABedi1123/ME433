add_executable(blink)

target_sources(blink PRIVATE blink.c)
target_sources(blink PRIVATE ssd1306.c)

# pull in common dependencies
target_link_libraries(blink pico_stdlib hardware_i2c pico_binary_info hardware_uart)


# create map/bin/hex file etc.
pico_add_extra_outputs(blink)