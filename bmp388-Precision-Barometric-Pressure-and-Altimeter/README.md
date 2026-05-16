# BMP388 Barometric Pressure Sensor for the ESP32 Weather Station

In this part of the ESP32 weather station project, we add the **BMP388 barometric pressure sensor**.

With this sensor, we can measure the current air pressure. This is useful because air pressure can tell us a lot about how the weather is changing. A single pressure value is nice to have, but the change in pressure over several hours is much more interesting.

When the air pressure drops quickly, it can indicate worsening weather, stronger winds, or unsettled conditions. When the air pressure rises, it often points to calmer or improving weather.

This code belongs to the video in which the BMP388 is added to the modular ESP32 weather station project from **Matteman-Automation**.

## What does this part do?

In this folder, you will find code and configuration to:

- read the BMP388 with an ESP32
- measure barometric pressure
- convert the pressure value to hPa
- calibrate the measured pressure value
- send the value to Home Assistant using MQTT
- create a 3-hour and 6-hour pressure trend in Home Assistant
- use the pressure trend for a simple weather indication

## Why use the BMP388?

The BMP388 is a small and accurate sensor for measuring barometric pressure.

It works over I2C, which makes it easy to connect to an ESP32.

For a DIY weather station, this sensor is especially useful because it allows you to show not only the current pressure, but also whether the pressure is rising or falling.

That trend makes the measurement much more valuable.

## Important to know

The BMP388 measures the air pressure at the location of the sensor.

This means that altitude affects the measured value. A sensor at sea level will show a different value than the same sensor at a higher location.

That is why it is important to understand whether you are working with:

- the measured pressure at your location
- or pressure corrected to sea level

In this project, the pressure value is calibrated so it can be compared more easily with reference values from nearby weather stations.

## Files in this folder

This folder may contain several files. The most important one is:

### `01-Code-BMP388.ino`

Start with this code first.

This basic test code reads the BMP388 and shows the values in the Serial Monitor of the Arduino IDE.

Use this code to check if:

- the sensor is connected correctly
- the I2C address is correct
- the ESP32 can find the BMP388
- the pressure value looks realistic

Always start with this basic code before using the full project code.

### Full project code

The full project code is used for the complete ESP32 weather station.

In that version, the BMP388 is added to the existing code with:

- WiFi
- MQTT
- OTA updates
- SHT41 temperature and humidity
- BMP388 barometric pressure
- sending sensor values to Home Assistant

Use this code only after the basic test code works correctly.

## I2C wiring

In this project, the BMP388 is connected using I2C.

On many ESP32 boards, these pins are commonly used:

| BMP388 | ESP32 |
|---|---|
| VIN / VCC | 3.3V |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

Some BMP388 modules may support 5V, but always check the specifications of your own module first.

## Common mistakes

If the sensor does not work, check these things first:

- SDA and SCL may be swapped
- the sensor may not have the correct power supply
- GND may not be connected properly
- the I2C address may be `0x76` instead of `0x77`
- start with the basic test code first
- only continue with the full MQTT code after the basic test works

If the sensor is not found, it does not immediately mean that the sensor is broken.  
In many cases, the problem is caused by wiring or by using the wrong I2C address.

## Home Assistant and MQTT

The BMP388 code is not meant to be used on its own.

The ESP32 measures the barometric pressure and sends the value to Home Assistant using MQTT. In Home Assistant, the value can be used for:

- current barometric pressure
- pressure value 3 hours ago
- pressure value 6 hours ago
- pressure difference over 3 hours
- pressure difference over 6 hours
- trend: rising, falling, or stable
- simple weather indication

In this project, the history and trend calculations are handled in Home Assistant.

The advantage is that Home Assistant keeps the historical pressure values, even when the ESP32 restarts.

## Part of the larger weather station project

This BMP388 module is one part of the larger modular ESP32 weather station project.

In earlier parts, temperature and humidity were added.  
In later parts, sensors for light, wind, rain, lightning detection, and other measurements can be added.

The goal is to build a complete weather station step by step, using:

- ESP32
- Arduino code
- MQTT
- Home Assistant
- DIY sensors
- clear dashboards

## Video and extra information

The full explanation of this code is available in the related video on the **Matteman-Automation** YouTube channel.

Extra information, wiring diagrams, and background explanations can be found on:

[https://matteman.nl](https://matteman.nl)

## Advice

Start with the basic test code.

When the BMP388 is correctly shown in the Serial Monitor, you know that the sensor and wiring are working.

After that, continue with the full project code and the Home Assistant integration.

This prevents you from having to troubleshoot the sensor, wiring, WiFi, MQTT, and Home Assistant all at the same time.
