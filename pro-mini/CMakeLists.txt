cmake_minimum_required(VERSION 3.20.5)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/avr-toolchain.cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(veggie-tower-zone C)

# MCU settings
set(MCU atmega328p)
set(F_CPU 8000000UL)

# Zone I2C address — pass at build time:
# cmake -DZONE_ADDR=0x10 ..
if(NOT DEFINED ZONE_ADDR)
    message(FATAL_ERROR "ZONE_ADDR must be defined. Pass -DZONE_ADDR=0x10 (zone 1), 0x11 (zone 2), or 0x12 (zone 3)")
endif()

set(CMAKE_C_FLAGS "-mmcu=${MCU} -DF_CPU=${F_CPU} -DZONE_ADDR=${ZONE_ADDR} -O2 -Wall -Wextra -std=gnu99")

add_executable(${PROJECT_NAME}
    src/main.c
    src/i2c_peripheral.c
    src/uart.c
    src/sensors.c
    src/actuators.c
    src/bme280.c
    src/adc.c
    src/spi.c
)

target_include_directories(${PROJECT_NAME} PRIVATE include)

# Generate .hex file for flashing
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.hex
    COMMENT "Generating HEX file"
)

# Print size after build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}>
    COMMENT "Binary size"
)

# Flash target
add_custom_target(flash
    COMMAND avrdude -c avrisp2 -P usb -p ${MCU} -U flash:w:${PROJECT_NAME}.hex:i
    DEPENDS ${PROJECT_NAME}
    COMMENT "Flashing zone controller"
)

# Fuse target — verify values before running!
add_custom_target(set_fuses
    COMMAND avrdude -c avrisp2 -P usb -p ${MCU}
        -U lfuse:w:0xFF:m
        -U hfuse:w:0xD7:m
        -U efuse:w:0xFF:m
    COMMENT "Writing fuse bits — verify values before running!"
)
