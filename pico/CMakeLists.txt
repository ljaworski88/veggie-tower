cmake_minimum_required(VERSION 3.20.5)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(veggie-tower)

target_sources(app PRIVATE
    src/main.c
    src/wifi.c
    src/mqtt.c
    src/i2c.c
    src/sensors.c
    src/settings.c
    src/watering.c
)
