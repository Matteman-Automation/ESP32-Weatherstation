
        /*
 *     Maker      : Matteman-Automation               
 *     Project    : Weerstation
 *     Version    : 1.2
 *     Date       : 04-2026
 *     Programmer : Ap Matteman
 *     Board      : ESP32 Wroom
 *     
 *     1.0 - WiFi, MQTT & OTA
 *     1.1 - + SHT41
 *     1.2 - + BMP388
 *     
 *     [Topic!] moet worden aangepast voor inkomende MQTT berichten
 *     
 */ 

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>
#include <Arduino_Secrets.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>

// Nodig voor de SHT41 sensor
#include <Adafruit_SHT4x.h>
Adafruit_SHT4x sht4 = Adafruit_SHT4x();

// Nodig voor de BMP388 sensor
#include <Adafruit_BMP3XX.h>
Adafruit_BMP3XX bmp;

// Nodig voor timing
unsigned long lSSID_MS = 0; // Voor SSID waarden
const unsigned long lSSID_Interval = 20000; // 20 seconde
unsigned long lSHT41_MS = 0; // Voor SHT41 waarden
const unsigned long lSHT41_Interval = 5000; // 5 seconde
unsigned long lBMP388_MS = 0; // Voor BMP388 waarden
const unsigned long lBMP388_Interval = 60000; // 1 minuut

 
const char* ssid = YourSSID;
const char* password = YourWiFiPassWord;
const char* HostName = "Demo_Code";  //Naam van het apparaat in het netwerk

const char* mqtt_broker = YourMQTTserver;
const char* mqtt_user = YourMQTTuser;
const char* mqtt_password = YourMQTTpassword;
String sMQTTName;

int iWiFiTry = 0;
int iMQTTTry = 0;

WiFiClient espClient;
PubSubClient MQTTclient(espClient); // MQTT Client


void Connect2WiFi() { 
  WiFi.mode(WIFI_STA);  //WiFi mode, Client
  iWiFiTry = 0;
  WiFi.begin(ssid, password);
  WiFi.setHostname(HostName);
  Serial.print("Connecting to WiFi ");
  while (WiFi.status() != WL_CONNECTED && iWiFiTry < 11) { //Probeer 11 keer met WiFi te verbinden
    ++iWiFiTry;
    Serial.print(".");
    delay(500);
  }
  if(iWiFiTry < 11){
    Serial.println("");
    Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.print("Verbinding mislukt"); 
  }
}

void Connect2MQTT() {
  // Controleer of WiFi verbonden is, zo niet verbind met WiFi.
  if (WiFi.status() != WL_CONNECTED) { 
    Connect2WiFi; 
  }
  iMQTTTry=0;
  sMQTTName = "weerstation-" + String(WiFi.macAddress());  //Unique MQTT Device name
  Serial.print("Connecting to MQTT ");
  MQTTclient.setServer(mqtt_broker, 1883);
  while (!MQTTclient.connect(sMQTTName.c_str(), mqtt_user, mqtt_password) && iMQTTTry < 11) { ///Probeer 11 keer met MQTT te verbinden
    ++iWiFiTry;
    ++iMQTTTry;
    Serial.print(".");
    delay(500);
  }
  if(iMQTTTry < 11) {
    Serial.println("");
    Serial.println("Verbonden met MQTT");
  } else {
    Serial.println("");
    Serial.println("Verbinding mislukt");
    int iErrorCode = MQTTclient.state();
    Serial.print("MQTT connect failed, code = "); Serial.println(iErrorCode);
  }
}


void setup() {
  Serial.begin(115200);   //Nodig voor de Serial Monitor  

  Connect2WiFi();
  Connect2MQTT();

  // (optioneel) naam en wachtwoord voor OTA
  ArduinoOTA.setHostname(HostName);
  ArduinoOTA.setPassword(OTAPassword);  // Wachtwoord opgeslagen in Arduino_Secrets.h

  // OTA starten
  ArduinoOTA.begin();

  // Specifiek voor I2C
  Wire.begin(); // ESP32: default pins (SDA=21, SCL=22) of set via Wire.begin(SDA, SCL)
  // Specifiek voor SHT41
  if (!sht4.begin()) {
    Serial.println("Kan SHT4x (SHT41) niet vinden op I2C!");
    while (1) delay(10);
  }
  // Optioneel: heater modes (voor condens/vochtige omgevingen), meestal UIT laten voor normale metingen
  // sht4.setHeater(SHT4X_NO_HEATER);
  Serial.println("SHT41 gevonden!");

  // Specifiek voor BMP388, probeer eerst adres 0x77
  if (!bmp.begin_I2C(0x77)) {
    // Probeer daarna 0x76
    if (!bmp.begin_I2C(0x76)) {
      Serial.println("BMP388 niet gevonden. Controleer bedrading en I2C-adres.");
      while (1) {
        delay(10);
      }
    }
  }
  // Instellingen voor meten, oversampling is het gemiddelde van het aantal gemeten waardes en deze geven als output.
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X); //Voor de meting van luchtdruk is ook de temperatuur (intern voor de sensor) belangrijk.
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);  // Haalt kleine schommelingen er uit
  Serial.println("BMP388 gevonden en gereed");
}

// --------------------------------------------------
// Dauwpunt berekenen
// --------------------------------------------------
float calculateDewPoint(float temperature, float humidity)
{
  // Magnus formule
  const float a = 17.62;
  const float b = 243.12;

  float gamma = log(humidity / 100.0) + (a * temperature) / (b + temperature);
  float dewPoint = (b * gamma) / (a - gamma);

  return dewPoint;
}

// --------------------------------------------------
// Gevoelstemperatuur berekenen
// Deze formule werkt vooral goed bij warmere temperaturen
// --------------------------------------------------
float calculateHeatIndex(float temperature, float humidity)
{
  // Heat Index formule gebruikt normaal Fahrenheit
  float tF = temperature * 9.0 / 5.0 + 32.0;

  float heatIndexF =
      -42.379
      + 2.04901523 * tF
      + 10.14333127 * humidity
      - 0.22475541 * tF * humidity
      - 0.00683783 * tF * tF
      - 0.05481717 * humidity * humidity
      + 0.00122874 * tF * tF * humidity
      + 0.00085282 * tF * humidity * humidity
      - 0.00000199 * tF * tF * humidity * humidity;

  // Voor lagere temperaturen voelt deze formule onnatuurlijk aan daarom gewoon de echte temperatuur teruggeven onder 27°C
  if (temperature < 27.0 || humidity < 40.0)
  {
    return temperature;
  }

  float heatIndexC = (heatIndexF - 32.0) * 5.0 / 9.0;
  return heatIndexC;
}

// --------------------------------------------------
// Absolute luchtvochtigheid
// --------------------------------------------------
float calculateAbsoluteHumidity(float temperature, float humidity)
{
  float es = 6.112 * exp((17.67 * temperature) / (temperature + 243.5));
  float ah = es * humidity * 2.1674 / (273.15 + temperature);

  return ah; // g/m3
}

// --------------------------------------------------
// Correctie luchtdruk 
// --------------------------------------------------
float calculateLevelPressure(float pressure_hPa, float altitude_m) {
  return pressure_hPa / pow(1.0 - (altitude_m / 44330.0), 5.255);
}

void loop() {
  unsigned long lNowMS = millis();
  int iRSSI;

  if (lNowMS - lSSID_MS >= lSSID_Interval) {    // Iedere 20 seconden RSSI signaal doorgeven
    lSSID_MS = lNowMS;

    // opvragen luchtdruk ivbm lage waarde bij opstarten.
    bmp.performReading();
    float fPressure = bmp.pressure;    

    iRSSI = WiFi.RSSI();
    if (!MQTTclient.connect(sMQTTName.c_str(), mqtt_user, mqtt_password)) {
      Connect2MQTT();
    }
    if (iWiFiTry < 11) { 
      MQTTclient.publish("sensor/weerstation/rssi", String(iRSSI).c_str());
      Serial.print("sensor/weerstation/rssi = "); Serial.println(iRSSI);
    } else {
      Serial.print("*** ERROR *** Geen WiFi.");
      ESP.restart(); // Restarts the ESP32
    }
  }


  if (lNowMS - lSHT41_MS >= lSHT41_Interval) {    // Iedere 5 seconden Temp & Vocht doorgeven
      lSHT41_MS = lNowMS;

    sensors_event_t humidity, temp;
    if (!sht4.getEvent(&humidity, &temp)) {
      Serial.println("Fout bij uitlezen!");
      delay(2000);
      return;
    } else {
      if (!MQTTclient.connect(sMQTTName.c_str(), mqtt_user, mqtt_password)) {
        Connect2MQTT();
      }
      if (iWiFiTry < 11) { 

        float fDauwPunt = calculateDewPoint(temp.temperature, humidity.relative_humidity);
        float fVoeltAls = calculateHeatIndex(temp.temperature, humidity.relative_humidity);
        float fVochtAbs = calculateAbsoluteHumidity(temp.temperature, humidity.relative_humidity);

        MQTTclient.publish("sensor/weerstation/temp", String(temp.temperature).c_str());
        Serial.print("sensor/weerstation/temp = "); Serial.print(temp.temperature); Serial.println(" °C");
        MQTTclient.publish("sensor/weerstation/vocht", String(humidity.relative_humidity).c_str());
        Serial.print("sensor/weerstation/vocht = "); Serial.print(humidity.relative_humidity); Serial.println(" %");  
        MQTTclient.publish("sensor/weerstation/vochtabs", String(fVochtAbs).c_str());
        Serial.print("sensor/weerstation/vochtabs = "); Serial.print(fVochtAbs); Serial.println(" g/m³");
        MQTTclient.publish("sensor/weerstation/dauwpunt", String(fDauwPunt).c_str());
        Serial.print("sensor/weerstation/dauwpunt = "); Serial.print(fDauwPunt); Serial.println(" °C");
        MQTTclient.publish("sensor/weerstation/voeltals", String(fVoeltAls).c_str());
        Serial.print("sensor/weerstation/voeltals = "); Serial.print(fVoeltAls); Serial.println(" °C");
        

      } else {
        Serial.print("*** ERROR *** Geen WiFi.");
        ESP.restart(); // Restarts the ESP32
      }
    }
  }
  if (lNowMS - lBMP388_MS >= lBMP388_Interval) {    // Iedere minuut
      lBMP388_MS = lNowMS;
      if (!bmp.performReading()) {
        Serial.println("Fout bij uitlezen van BMP388");
        delay(2000);
        return;
      } else {
        if (iWiFiTry < 11) { 
          float fPressure = bmp.pressure / 100.0;  // Pa -> hPa
          float fLevelPressure = calculateLevelPressure(fPressure, 12); // Hoogte callibratie op basis van WeerLive.nl

          MQTTclient.publish("sensor/weerstation/luchtdruk", String(fPressure).c_str());
          Serial.print("sensor/weerstation/luchtdruk = "); Serial.print(fPressure); Serial.println(" hPa");
          MQTTclient.publish("sensor/weerstation/luchtdruk_cal", String(fLevelPressure).c_str());
          Serial.print("sensor/weerstation/luchtdruk_cal = "); Serial.print(fLevelPressure); Serial.println(" hPa");

      } else {
        Serial.print("*** ERROR *** Geen WiFi.");
        ESP.restart(); // Restarts the ESP32
      } 
    }
  }
  
  ArduinoOTA.handle();
}

    