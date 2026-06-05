
        /*
 *     Maker      : Matteman-Automation               
 *     Project    : VEML7700 verbinden met WiFi, MQTT & OTA
 *     Version    : 1.1
 *     Date       : 12-2025
 *     Programmer : Ap Matteman
 *     Board      : ESP32 Wroom
 *     
 *     1.1 - Verbetering MQTT controle.
 *     
 *     [Topic!] moet worden aangepast voor inkomende MQTT berichten
 *     
 */ 

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>
#include <Arduino_Secrets.h>

#include <Wire.h>
#include <Adafruit_VEML7700.h>

Adafruit_VEML7700 veml = Adafruit_VEML7700();
 
const char* ssid = YourSSID;
const char* password = YourWiFiPassWord;
const char* HostName = "Demo_Code";  //Naam van het apparaat in het netwerk

const char* mqtt_broker = YourMQTTserver;
const char* mqtt_user = YourMQTTuser;
const char* mqtt_password = YourMQTTpassword;
String sMQTTName;

int iWiFiTry = 0;
int iMQTTTry = 0;

unsigned long lSSID_MS = 0; // Voor SSID waarden
const unsigned long lSSID_Interval = 20000; // 20 seconde

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

void callback(char *topic, byte *payload, unsigned int length) {
  String sStatus;  // Bevat de waarde

  Serial.println("-----------------------");
  // Convert Char* to String
  String STopic = topic;    
  payload[length] = 0;   String recv_payload = String(( char *) payload);
  
  Serial.print("Message arrived in topic: "); Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
      Serial.print((char) payload[i]);
      sStatus+= (char)payload[i];
  }
  sStatus.trim();
  Serial.println();

  if(strcmp(topic,"[Topic!]")==0) {
    // Doe iets als de [Topic!] binnenkomrt met sStatus
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

  MQTTclient.setCallback(callback);
  // MQTTclient.subscribe("[Topic!]");
  
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
  unsigned long lNowMS = millis();
  int iRSSI;

   MQTTclient.loop();
   ArduinoOTA.handle();

  if (lNowMS - lSSID_MS >= lSSID_Interval) {    // Iedere 20 seconden RSSI signaal doorgeven
    lSSID_MS = lNowMS;
    float fLux = veml.readLux();
    uint16_t white = veml.readWhite();
    uint16_t als = veml.readALS();
  
    iRSSI = WiFi.RSSI();
    if (!MQTTclient.connect(sMQTTName.c_str(), mqtt_user, mqtt_password)) {
      Connect2MQTT();
    }
    if (iWiFiTry < 11) { 
      MQTTclient.publish("sensor/weerstation/rssi", String(iRSSI).c_str());
      Serial.print("sensor/weerstation/rssi = "); Serial.println(iRSSI);

      MQTTclient.publish("sensor/weerstation/licht_lux", String(fLux).c_str());
      Serial.print("sensor/weerstation/licht_lux = "); Serial.print(fLux); Serial.println(" lux");

      MQTTclient.publish("sensor/weerstation/licht_white", String(white).c_str());
      Serial.print("sensor/weerstation/licht_white = "); Serial.println(white); 

      MQTTclient.publish("sensor/weerstation/licht_als", String(als).c_str());
      Serial.print("sensor/weerstation/licht_als = "); Serial.println(als); 
    } else {
      Serial.print("*** ERROR *** Geen WiFi.");
      ESP.restart(); // Restarts the ESP32
    }
  }  
}

    