# Veggie Tower

Automated vertical garden controller using a Raspberry Pi Pico W (Zephyr RTOS)
as the central coordinator and Arduino Pro Mini (bare metal AVR C) nodes for
per-zone control. All zone setpoints are configurable through Home Assistant.

## Hardware
- 1x Raspberry Pi Pico W — central coordinator
- 3x Arduino Pro Mini (ATmega328P) — zone controllers

## Setpoints (configurable via Home Assistant)
- Watering duration (per zone)
- Soil moisture thresholds (water vs no-water, per zone)
- Fan PWM speed (per zone)
- Light on/off schedule (per zone)
- CO2 thresholds (per zone and global)
- Temperature/humidity thresholds (per zone and global)

## Setup

### Pico W (Zephyr)
1. Copy `pico/credentials.conf.template` to `pico/credentials.conf` and fill in your values
2. Activate the Zephyr venv: `source ~/.local/zephyrproject/.venv/bin/activate.fish`
3. Build: `west build -b rpi_pico/rp2040/w pico -- -DEXTRA_CONF_FILE="credentials.conf"`

### Pro Mini (AVR)
Coming soon.

## Architecture
See `docs/architecture.md`.
