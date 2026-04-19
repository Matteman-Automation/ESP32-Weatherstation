# 🌦️ ESP32 Weather Station with Home Assistant

Build a modular and reliable ESP32-based weather station with multiple sensors, fully integrated into Home Assistant via MQTT.

---

## 🎬 Watch the full video series
👉 [https://youtube.com/YOUR_CHANNEL_LINK](https://www.youtube.com/playlist?list=PLKaMzy72TaQcMz4aPWDMtECpoyzQZCbpT)

---

## 🌐 Full project website (recommended)
👉 [https://matteman.nl/Home-Assistant/Projecten/Weerstation](https://matteman.nl/Home-Assistant/Projecten/Weerstation/index)

---

## 🧠 What is this project?

This is a complete DIY weather station project based on an ESP32.

Instead of a single sensor setup, this project is built as a **modular system**, where each sensor has its own dedicated module and code.

The goal is:
✔ Accurate measurements  
✔ Reliable long-term data  
✔ Seamless integration with Home Assistant  
✔ Easy to expand  

---

## 🔧 Features

- ESP32-based system
- Modular sensor design
- MQTT integration
- OTA updates
- Home Assistant dashboards
- Designed for real-world outdoor usage

---

## 📦 Modules (click to open)

### 🌡️ Temperature & Humidity (SHT41)
👉 `/sht41-temperature-humidity`

Accurate temperature and humidity measurement using the SHT41 sensor.

Includes:
- Arduino code
- MQTT integration
- Calculations (dew point, feels like, absolute humidity)

---

### 🌬️ Air Pressure (BMP388) *(coming soon)*
👉 `/bmp388-pressure`

Measure atmospheric pressure for weather prediction.

---

### ☀️ Light Measurement (VEML7700) *(coming soon)*
👉 `/veml7700-light`

Measure ambient light levels for climate and automation.

---

### 🏠 Home Assistant
👉 `/home-assistant`

Contains:
- MQTT configuration
- Dashboard examples
- YAML snippets

---

## 🔌 Hardware overview

- ESP32
- SHT41 sensor
- (future) BMP388
- (future) VEML7700
- Radiation shield (recommended)

---

## 🧱 Project structure
/sht41-temperature-humidity
/bmp388-pressure
/veml7700-light
/home-assistant
/docs


---

## 🚀 Getting started

1. Start with the SHT41 module  
2. Upload the basic test code  
3. Switch to MQTT version  
4. Add sensors to Home Assistant  
5. Build your dashboard  

---

## ⚠️ Important tip

Proper sensor placement is critical.

Avoid:
- Direct sunlight  
- Mounting against walls  
- Heat sources  

Use a **radiation shield** for accurate results.

---

## 📊 What makes this different?

Most DIY weather stations:
❌ Use cheap sensors  
❌ Have inaccurate measurements  
❌ Lack proper integration  

This project focuses on:
✔ Accuracy  
✔ Reliability  
✔ Real-world usability  

---

## 🔗 Useful links

- YouTube channel: [https://youtube.com/YOUR_CHANNEL_LINK ](https://www.youtube.com/@MattemanAutomation) 
- Website: [https://matteman.nl](https://matteman.nl/)  
- Weather station project page: [https://matteman.nl/Home-Assistant/Projecten/Weerstation](https://matteman.nl/Home-Assistant/Projecten/Weerstation/index)

---

## 🤝 Contributing

Feel free to:
- Use the code
- Improve it
- Expand it

If you build something cool, share it!

---

## 👍 Support

If this project helped you:

⭐ Star this repository  
📺 Subscribe on YouTube  
🌐 Visit the website  

---

## 📜 License

MIT License

---

## 👋 About

Created by Matteman Automation  
Practical ESP32 & Home Assistant projects

👉 https://matteman.nl


