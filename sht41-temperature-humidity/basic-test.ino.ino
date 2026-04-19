
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_SHT4x.h>

Adafruit_SHT4x sht4 = Adafruit_SHT4x();

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Wire.begin(); // ESP32: default pins (SDA=21, SCL=22) of set via Wire.begin(SDA, SCL)

  if (!sht4.begin()) {
    Serial.println("Kan SHT4x (SHT41) niet vinden op I2C!");
    while (1) delay(10);
  }

  // Optioneel: heater modes (voor condens/vochtige omgevingen), meestal UIT laten voor normale metingen
  // sht4.setHeater(SHT4X_NO_HEATER);

  Serial.println("SHT41 gevonden!");
}

void loop() {
  sensors_event_t humidity, temp;
  if (!sht4.getEvent(&humidity, &temp)) {
    Serial.println("Fout bij uitlezen!");
    delay(2000);
    return;
  }

  Serial.print("Temperatuur: ");
  Serial.print(temp.temperature);
  Serial.println(" °C");

  Serial.print("Luchtvochtigheid: ");
  Serial.print(humidity.relative_humidity);
  Serial.println(" %");

  delay(2000);
}
