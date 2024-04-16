add_executable(blink)

target_sources(blink PRIVATE blink.c)

# pull in common dependencies
target_link_libraries(blink pico_stdlib hardware_i2c pico_binary_info)

# create map/bin/hex file etc.
pico_add_extra_outputs(blink)