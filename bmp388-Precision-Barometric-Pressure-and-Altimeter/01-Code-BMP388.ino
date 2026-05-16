#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>

Adafruit_BMP3XX bmp;

// Zet hier de actuele luchtdruk voor een hoogtemeting.
#define LEVELPRESSURE_HPA (999.54)

void setup() {
  Serial.begin(115200);
  delay(1000);

  // ESP32 standaard I2C pinnen: SDA = 21, SCL = 22
  Wire.begin(21, 22);

  Serial.println("BMP388 test gestart");

  // Probeer eerst adres 0x77
  if (!bmp.begin_I2C(0x77)) {
    // Probeer daarna 0x76
    if (!bmp.begin_I2C(0x76)) {
      Serial.println("BMP388 niet gevonden. Controleer bedrading en I2C-adres.");
      while (1) {
        delay(10);
      }
    }
  }

  // Instellingen voor meten
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  Serial.println("BMP388 gevonden en gereed");
}

void loop() {
  if (!bmp.performReading()) {
    Serial.println("Fout bij uitlezen van BMP388");
    delay(2000);
    return;
  }

  Serial.print("Temperatuur = ");
  Serial.print(bmp.temperature);
  Serial.println(" °C");

  Serial.print("Druk = ");
  Serial.print(bmp.pressure / 100.0);   // Pa -> hPa
  Serial.println(" hPa");

  Serial.print("Geschatte hoogte = ");
  Serial.print(bmp.readAltitude(LEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.println();
  delay(2000);
}