# Architecture

## Overview
The Pico W acts as the central coordinator, collecting sensor data from three
zone controllers (Pro Minis) and reporting to Home Assistant over MQTT. Zone
setpoints can be updated at any time through Home Assistant and are persisted
on the Pico W, surviving power cycles.

## Communication
- Pico W <-> Home Assistant: MQTT over WiFi
- Pico W <-> Pro Minis: I2C (Pico W as master)
- Pro Mini <-> CO2 sensor (MH-Z19): UART
- Pro Mini <-> DHT22: single-wire
- Pro Mini <-> soil moisture sensors: analog in (A0-A3)
- Pro Mini <-> fan, lights, solenoids: GPIO/PWM

## MQTT Topics
### Telemetry (Pico W -> Home Assistant)
| Topic | Description |
|-------|-------------|
|`veggie-tower/zone/{n}/subzone/{s}/moisture` | Subzone Average Soil Moisture |
| `veggie-tower/zone/{n}/subzone/{s}/moisture/raw/{sensor}` | Raw Soil Moisture Sensor Readings |
| `veggie-tower/zone/{n}/temperature` | Air temperature |
| `veggie-tower/zone/{n}/humidity` | Air humidity |
| `veggie-tower/zone/{n}/co2` | CO2 level |
| `veggie-tower/pico/temperature` | Pico W air temperature |
| `veggie-tower/pico/humidity` | Pico W air humidity |
| `veggie-tower/pico/pressure` | Pico W air pressure |
| `veggie-tower/pico/co2` | Pico W CO2 level |

### Setpoints (Home Assistant -> Pico W)
| Topic | Description |
|-------|-------------|
| `veggie-tower/zone/{n}/subzone/{s}/setpoint/moisture-low` | Moisture threshold to trigger watering |
| `veggie-tower/zone/{n}/subzone/{s}/setpoint/watering-duration` | How long a watering cycle lasts |
| `veggie-tower/zone/{n}/subzone/{s}/setpoint/moisture-target` | Target moisture level after watering cycle |
| `veggie-tower/zone/{n}/subzone/{s}/setpoint/watering-settle-time` | Wait time in seconds before post-water check |
| `veggie-tower/zone/{n}/subzone/{s}/setpoint/watering-max-cycles` | Maximum number of water/check cycles before giving up |
| `veggie-tower/zone/{n}/subzone/{s}/sensor-map` | JSON assignment of sensors to subzone |
| `veggie-tower/zone/{n}/setpoint/fan-speed` | Fan PWM duty cycle (0-100%) |
| `veggie-tower/zone/{n}/setpoint/light-on` | Light on time (HH:MM) |
| `veggie-tower/zone/{n}/setpoint/light-off` | Light off time (HH:MM) |
| `veggie-tower/zone/{n}/setpoint/co2-threshold` | CO2 level to trigger fan increase |
| `veggie-tower/zone/{n}/setpoint/temp-threshold` | Temperature threshold to trigger fan increase |

### Status (Pico W -> Home Assistant)
| Topic | Description |
|-------|-------------|
| `veggie-tower/zone/{n}/subzone/{s}/status/watering` | Currently watering true/false |
| `veggie-tower/zone/{n}/status/lights` | Lights on/off |
| `veggie-tower/zone/{n}/status/fan-speed` | Current fan PWM duty cycle |
| `veggie-tower/status/online` | System heartbeat |

## Setpoint Persistence
Setpoints received over MQTT are persisted to flash on the Pico W using
Zephyr's settings subsystem, surviving power cycles. On boot the Pico W
loads persisted setpoints before initiating zone communication and publishes
current setpoints back to Home Assistant to resync its state.

## Subzone Configuration
Each zone has up to 4 soil moisture sensors (A0-A3) which are grouped into
subzones. Each subzone has its own solenoid valve. Sensor-to-subzone
assignment is configurable via Home Assistant.

Example assignment for a zone with 3 subzones:
| Sensor | Subzone |
|--------|---------|
| A0     | 1       |
| A1     | 1       |
| A2     | 2       |
| A3     | 3       |

## Watering Logic
1. Check soil moisture across all subzone sensors
2. If average is below `moisture-low` threshold, begin watering cycle:
   a. Open solenoid valve for `watering-duration` seconds
   b. Close valve, wait `watering-settle-time` seconds for capillary action
   c. Re-check soil moisture
   d. If still below `moisture-target`, calculate a new duration and repeat
   e. Repeat up to `watering-max-cycles` times
3. If moisture target not reached after max cycles, publish alert to Home Assistant

## I2C Addresses
| Zone | Pro Mini Address |
|------|-----------------|
| 1    | 0x10            |
| 2    | 0x11            |
| 3    | 0x12            |

## I2C Protocol
Commands and responses between the Pico W and Pro Minis use a simple
fixed-format binary protocol. Details TBD during implementation.
