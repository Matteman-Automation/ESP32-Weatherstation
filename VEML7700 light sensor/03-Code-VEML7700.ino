#include <Wire.h>
#include <Adafruit_VEML7700.h>

Adafruit_VEML7700 veml = Adafruit_VEML7700();

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("VEML7700 test");

  // Voor ESP32 kun je eventueel expliciet SDA/SCL opgeven:
  // Wire.begin(21, 22);

  Wire.begin();

  if (!veml.begin()) {
    Serial.println("VEML7700 niet gevonden. Controleer de bedrading.");
    while (1) {
      delay(100);
    }
  }

  Serial.println("VEML7700 gevonden!");

  // Instellingen sensor
  veml.setGain(VEML7700_GAIN_1);
  veml.setIntegrationTime(VEML7700_IT_100MS);

  Serial.println("Sensor gestart.");
}

void loop() {
  float lux = veml.readLux();
  uint16_t white = veml.readWhite();
  uint16_t als = veml.readALS();

  Serial.println("----- Meting -----");
  Serial.print("Lux: ");
  Serial.println(lux);

  Serial.print("White: ");
  Serial.println(white);

  Serial.print("ALS raw: ");
  Serial.println(als);

  delay(2000);
}