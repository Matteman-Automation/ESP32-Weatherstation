# 🌡️ ESP32 Temperature & Humidity (SHT41) – Home Assistant

Measure temperature and humidity accurately using an ESP32 and SHT41 sensor, with full integration into Home Assistant via MQTT.

---

## 🎬 Watch the full tutorial
👉 https://youtube.com/YOUR_VIDEO_LINK

---

## 🌐 Full step-by-step guide
👉 https://matteman.nl/YOUR_PROJECT_PAGE

---

## 🧠 What is this module?

This module is part of a modular ESP32 weather station.

It focuses on:
- Accurate **temperature measurement**
- Reliable **humidity readings**
- Smart climate data for Home Assistant

Most DIY setups measure temperature incorrectly (for example against a wall or in sunlight).  
This setup is designed to give **real outdoor air measurements**.

---

## 🔧 Features

- ESP32 + SHT41 sensor
- I2C communication
- MQTT integration
- OTA updates
- Stable long-term measurements
- Designed for outdoor use (radiation shield)

---

## 📊 Calculated values

This module also calculates:

- 🌡️ Temperature (°C)
- 💧 Relative humidity (%)
- 🌫️ Dew point
- 🧠 Feels like temperature
- 💨 Absolute humidity
- 📶 WiFi signal strength

---

## 🧰 Hardware required

- ESP32 development board
- SHT41 breakout board
- Jumper wires / soldering
- Power supply
- Radiation shield (recommended)

---

## 🔌 Wiring (I2C)

| SHT41 | ESP32 |
|------|------|
| VCC  | 3.3V |
| GND  | GND  |
| SDA  | GPIO21 |
| SCL  | GPIO22 |

---

## ⚠️ Important lesson learned

Do NOT drill or modify the sensor or PCB.

I made this mistake and destroyed:
- Multiple sensors  
- One ESP32 😅  

👉 Always solder directly to the pins.

---

## 💻 Code

### 1. Basic test
File: basic-test.ino

Use this to verify:
- Sensor is detected
- Values are correct

---

### 2. Full project
File: esp32-sht41-mqtt.ino

Includes:
- WiFi connection
- MQTT communication
- OTA updates
- Sensor calculations

---

## 🏠 Home Assistant (MQTT)

Example configuration:

```yaml
mqtt:
  sensor:
    - name: "Weatherstation Temperature"
      state_topic: "weatherstation/temperature"
      unit_of_measurement: "°C"
      icon: "mdi:thermometer"

    - name: "Weatherstation Humidity"
      state_topic: "weatherstation/humidity"
      unit_of_measurement: "%"
      icon: "mdi:water-percent"

## 📊 Dashboard ideas

Works great with:

Mini Graph Card
Mushroom Cards
ApexCharts

Visualize:

Real-time data
Trends over 24h or 7 days
Outdoor climate behavior
🚀 Next steps

Expand your weather station with:

🌬️ BMP388 (air pressure)
☀️ VEML7700 (light)
🌧️ Rain sensor
🌪️ Wind sensor
💡 Why use the SHT41?

Compared to cheaper sensors:

✔ Higher accuracy
✔ Better long-term stability
✔ Reliable outdoor performance

🤝 Contributing

Feel free to:

Use this code
Modify it
Improve it
👍 Support

If this helped you:

⭐ Star this repository
📺 Subscribe on YouTube
🌐 Visit the website

📜 License

MIT License
