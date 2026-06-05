# VEML7700 Light Sensor for the ESP32 Weather Station

In this part of the ESP32 weather station project, we add the VEML7700 light sensor.

With this sensor, we can measure the real amount of outdoor light. This is useful because Home Assistant can calculate sunrise and sunset times, but that does not always tell you how much light there really is outside.

On a bright summer evening, it can stay light for a long time.
On a dark winter day, or during heavy rain, it can be dark enough to turn on the lights even in the middle of the day.

By measuring the real outdoor light level, Home Assistant can make smarter decisions.

This code belongs to the video in which the VEML7700 is added to the modular ESP32 weather station project from Matteman-Automation.

## What does this part do?

In this folder, you will find Arduino code and Home Assistant automation examples to:

* read the VEML7700 with an ESP32
* measure the outdoor light level
* show the light value in the Serial Monitor
* send the light value to Home Assistant using MQTT
* update the ESP32 wirelessly using OTA
* add the VEML7700 to the existing ESP32 weather station code
* use the measured light value for smarter Home Assistant automations
* automatically control lighting or sun shading based on real outdoor light

The goal is not only to show a light value in Home Assistant, but also to use that value in practical automations.

For example:

* turn lights on when it is really dark outside
* prevent lights from switching too early on bright evenings
* switch lights on during very dark daytime weather
* turn lights off again when there is enough daylight
* control shutters, blinds or sun shades based on real outdoor light
* create a Home Assistant dashboard with outdoor light history

## Why use the VEML7700?

The VEML7700 is a digital ambient light sensor.

For a very simple project, you could also use an LDR, or light dependent resistor.
An LDR is cheap and can be useful if you only want to know whether it is light or dark.

But for this weather station project, I want something more useful than only a simple light/dark value.

The VEML7700 gives a light value in lux.
That makes it easier to see how the amount of light changes during the day.

For example:

* sunrise
* sunset
* clouds
* rain
* bright sunlight
* dark winter days
* sudden changes in light during bad weather

Another advantage is that the VEML7700 works over I2C.
This makes it easy to connect to an ESP32 and also easy to combine with other I2C sensors in the weather station, such as the SHT41 and BMP388.

## Important to know

The VEML7700 measures the amount of light that reaches the sensor.

In my weather station, the sensor is mounted inside the housing and protected by a translucent 3D printed window.

Because of that, the measured lux value is not meant to be used as an official calibrated lux measurement.

But that is not a problem for this project.

For Home Assistant automations, the exact lux value is often less important than the trend and the practical light level.

What matters is that you can reliably see the difference between:

* bright daylight
* cloudy weather
* dusk
* darkness
* sudden dark conditions during rain

That makes the sensor very useful for smart lighting and sun shade automations.

## Files in this folder

This folder contains three Arduino sketches and four Home Assistant automation examples.

### `03-Code-VEML7700.ino`

Start with this code first.

This is the basic test code for the VEML7700.

It reads the sensor and shows the light value in the Serial Monitor of the Arduino IDE.

Use this code to check if:

* the sensor is connected correctly
* the ESP32 can find the VEML7700
* the I2C connection is working
* the light value changes when the amount of light changes
* the sensor gives realistic values

Always start with this basic code before using the WiFi, MQTT, OTA or full weather station code.

This prevents you from having to troubleshoot the sensor, wiring, WiFi, MQTT and Home Assistant all at the same time.

### `VEML7700_WiFI_MQTT_OTA.ino`

This version builds on the basic VEML7700 code.

It adds:

* WiFi
* MQTT
* OTA updates
* sending the light value to Home Assistant

Use this code after the basic VEML7700 test code works correctly.

With this version, the ESP32 reads the VEML7700 and publishes the light value to an MQTT topic.
Home Assistant can then use that MQTT value as a sensor.

OTA updates make it possible to update the ESP32 wirelessly after the first upload by USB.

### `Weerstation_SHT41_BMP388_VEML7700.ino`

This is the complete weather station code for this part of the project.

In this version, the VEML7700 is added to the existing weather station code with:

* SHT41 temperature and humidity
* BMP388 barometric pressure
* VEML7700 outdoor light
* WiFi
* MQTT
* OTA updates
* sending sensor values to Home Assistant

Use this code only after the separate basic test code works correctly.

## Home Assistant automation files

This folder also contains four Home Assistant YAML automation examples.

These examples show how the VEML7700 light value can be used in practical Home Assistant automations.

They are meant as examples and must be adjusted to your own Home Assistant entities.

The automations can be used for things like:

* turning lights on in the morning when it is still too dark outside
* turning lights on during the day when the outdoor light level drops below a chosen threshold
* turning lights off again when there is enough daylight
* turning lights on in the evening when it is really dark outside
* combining measured lux values with time, sunrise, sunset and workday conditions

If your YAML filenames are different, update the list below to match your own repository.

Suggested file structure:

```text
03-Code-VEML7700.ino
VEML7700_WiFI_MQTT_OTA.ino
Weerstation_SHT41_BMP388_VEML7700.ino

home-assistant-automation-morning-lights.yaml
home-assistant-automation-daytime-darkness.yaml
home-assistant-automation-lights-off-daylight.yaml
home-assistant-automation-evening-lights.yaml
```

## I2C wiring

In this project, the VEML7700 is connected using I2C.

On many ESP32 boards, these pins are commonly used:

| VEML7700  | ESP32   |
| --------- | ------- |
| VIN / VCC | 3.3V    |
| GND       | GND     |
| SDA       | GPIO 21 |
| SCL       | GPIO 22 |

Some VEML7700 breakout boards may support both 3.3V and 5V, depending on the board design.
Always check the specifications of your own module first.

For an ESP32 project, 3.3V is usually the safest choice.

## I2C address

The VEML7700 normally uses I2C address:

```text
0x10
```

If the sensor is not found, first check the wiring and run an I2C scanner.

If the ESP32 does not see the sensor at address `0x10`, it usually means there is a wiring, power or connection problem.

## Libraries

For the Arduino IDE, install the following libraries:

```text
Adafruit VEML7700 Library
Adafruit BusIO
```

For the WiFi, MQTT and OTA versions, you also need the libraries that are used by the rest of the ESP32 weather station project.

Depending on your code, this may include:

```text
WiFi
PubSubClient
ArduinoOTA
Wire
```

For the complete weather station code, you also need the libraries for the other sensors used in the project, such as the SHT41 and BMP388 libraries.

## Common mistakes

If the sensor does not work, check these things first:

* SDA and SCL may be swapped
* GND may not be connected properly
* the sensor may not be powered correctly
* the wrong ESP32 pins may be used
* the required Arduino libraries may not be installed
* the I2C address may not be detected
* the sensor may be mounted in a place where too little light reaches it
* the Serial Monitor may be set to the wrong baud rate
* WiFi credentials may be incorrect in the MQTT version
* the MQTT broker address may be wrong
* the Home Assistant entity names in the YAML files may not match your setup

If the sensor is not found, it does not immediately mean that the sensor is broken.

In many cases, the problem is caused by wiring, power or using the wrong pins.

## Sensor placement

The placement of the VEML7700 is important.

For this weather station, the sensor is mounted inside the weather station housing.
A translucent 3D printed window above the sensor allows light to reach the sensor while protecting it from rain.

This makes the sensor more suitable for outdoor use, but it also means that the measured lux value is influenced by the housing.

That is why the measured value should be used as a practical reference value for this setup.

For automations, this is usually good enough.

You can also place the sensor indoors, for example in the living room.

In that case, you are not measuring outdoor light, but the real light level inside the room.
That can be useful if you want your lights to respond to the actual light level in your house.

## Home Assistant and MQTT

The ESP32 reads the VEML7700 and sends the measured light value to Home Assistant using MQTT.

In Home Assistant, this value can be used for:

* a current outdoor light sensor
* a light history graph
* a dashboard card
* lighting automations
* sun shade or shutter automations
* detecting dark daytime conditions

The main advantage is that Home Assistant can react to the real measured light level instead of only using sunrise and sunset times.

For example, you can create an automation that turns on the lights when:

* it is a workday
* the outdoor light level is below a chosen threshold
* and the time is within a useful time window

You can also turn the lights off again when the outdoor light level rises above the threshold.

## Example MQTT sensor in Home Assistant

Depending on your MQTT setup, your sensor may be created automatically through MQTT discovery, or you may define it manually.

A simple manual MQTT sensor can look like this:

```yaml
mqtt:
  sensor:
    - name: "Weather Station Light"
      state_topic: "weatherstation/light"
      unit_of_measurement: "lx"
      device_class: illuminance
      state_class: measurement
```

Adjust the `state_topic` to match the MQTT topic used in your Arduino code.

## Home Assistant automation examples

The included YAML automation files are examples.

Before using them, change the entity names to match your own Home Assistant installation.

Example entities used in my setup may include:

```yaml
sensor.weerstation_licht
binary_sensor.workday_sensor
```

Your own light or switch entities will probably be different.

For example:

```yaml
switch.keuken_led_sonoff_relay
switch.woonkamer_lamp_kachel
switch.sonoff_1000af2fcd_1
```

### Example: turn lights on when it is too dark

This is a simplified example.
Use the included YAML files as the more complete versions.

```yaml
alias: Turn lights on when outdoor light is low
description: Turn on lights when the measured outdoor light level is below the threshold.
trigger:
  - platform: numeric_state
    entity_id: sensor.weerstation_licht
    below: 400
condition:
  - condition: time
    after: "07:00:00"
    before: "23:30:00"
action:
  - service: switch.turn_on
    target:
      entity_id:
        - switch.keuken_led_sonoff_relay
        - switch.woonkamer_lamp_kachel
mode: single
```

### Example: turn lights off when there is enough daylight

```yaml
alias: Turn lights off when outdoor light is high
description: Turn off lights when there is enough measured outdoor light.
trigger:
  - platform: numeric_state
    entity_id: sensor.weerstation_licht
    above: 400
    for:
      minutes: 5
condition:
  - condition: time
    after: "07:00:00"
    before: "23:30:00"
action:
  - service: switch.turn_off
    target:
      entity_id:
        - switch.keuken_led_sonoff_relay
        - switch.woonkamer_lamp_kachel
mode: single
```

## Choosing a lux threshold

The best lux threshold depends on your own setup.

In my project, a value around `400 lux` can be used as a starting point for lighting automations.

But every situation is different.

The measured value depends on:

* the sensor position
* the housing
* whether the sensor is behind a transparent or translucent cover
* the direction of the sensor
* nearby shadows
* the weather
* the time of year

The best approach is to first collect a few days of light data in Home Assistant.

Then look at the graph and decide which value makes sense for your own automation.

## Example use cases

The VEML7700 can be used for several practical Home Assistant automations.

### Smart lighting

Turn lights on when it is really dark outside, instead of only using sunset.

This prevents lights from switching too early on bright days or too late on dark cloudy days.

### Dark weather detection

If the light level drops during the day because of heavy clouds or rain, Home Assistant can temporarily turn on lights.

### Sun shade control

When the light level is very high, Home Assistant can close shutters, blinds or sun shades.

This can be useful on sunny days when direct sunlight heats up the room.

### Dashboard

Show the outdoor light level in Home Assistant and use a history graph to see how the light changes during the day.

This makes it easy to compare sunny, cloudy and rainy days.

## Recommended order

Use the files in this order:

1. Start with `03-Code-VEML7700.ino`
2. Check the Serial Monitor
3. Confirm that the light value changes when the amount of light changes
4. Continue with `VEML7700_WiFI_MQTT_OTA.ino`
5. Check if the value arrives in Home Assistant through MQTT
6. Continue with `Weerstation_SHT41_BMP388_VEML7700.ino`
7. Add the Home Assistant YAML automations
8. Adjust the lux threshold and entity names for your own setup

This step-by-step approach prevents you from troubleshooting everything at the same time.

## Part of the larger weather station project

This VEML7700 module is one part of the larger modular ESP32 weather station project.

In earlier parts, temperature, humidity and barometric pressure were added.

In this part, outdoor light is added.

In later parts, sensors for rain, wind, wind direction, lightning detection and other measurements can be added.

The goal is to build a complete weather station step by step, using:

* ESP32
* Arduino code
* MQTT
* Home Assistant
* DIY sensors
* clear dashboards
* practical automations

## Video and extra information

The full explanation of this code is available in the related video on the Matteman-Automation YouTube channel.

Extra information, wiring diagrams, background explanations and project pages can be found on:

```text
https://matteman.nl
```

The GitHub repository for the ESP32 weather station project can be found at:

```text
https://github.com/Matteman-Automation/ESP32-Weatherstation
```

## Advice

Start with the basic test code.

When the VEML7700 is correctly shown in the Serial Monitor and the light value changes when the amount of light changes, you know that the sensor and wiring are working.

After that, continue with the WiFi, MQTT and OTA version.

Only when that works correctly, add the sensor to the full weather station code.

Finally, add the Home Assistant automation files and adjust the entity names and lux thresholds for your own situation.

This prevents you from having to troubleshoot the sensor, wiring, WiFi, MQTT, OTA and Home Assistant all at the same time.
