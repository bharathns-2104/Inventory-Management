#include <WiFi.h>
#include <HX711_ADC.h>
#include <EEPROM.h>
const int HX711_dout = 23; //esp > HX711 dout pin
const int HX711_sck = 22; //esp > HX711 sck pin
HX711_ADC LoadCell(HX711_dout, HX711_sck);
const int calVal_eepromAdress = 0;
long t;
String apiKey = "4FC9H7PKH6AUXXID";     //  Enter your Write API key from ThingSpeak
const char *ssid =  "OnePlus Nord";     // replace with your wifi ssid and wpa2 key
const char *pass =  "Bharath211204";
const char *server = "api.thingspeak.com";
WiFiClient client;
void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  LoadCell.begin();
  float calibrationValue;// calibration value (see example file "Calibration.ino")
//if defined(ESP8266)|| defined(ESP32)
  EEPROM.begin(512);      // uncomment this if you use ESP8266/ESP32 and want to fetch the calibration value from eeprom
//endif
  EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch the calibration value from eeprom
  long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check ESP>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }
}
void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity
  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;
  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData();
      if (i<0){
        i=0;
      }
      Serial.print("Load_cell output val: ");
      Serial.println(i);
      if (client.connect(server, 80)) {
        Serial.println("Connected to ThingSpeak server");

        String postStr = "field1=" + String(i);
      // Construct the POST request
        String httpRequest = "POST /update HTTP/1.1\n";
        httpRequest += "Host: api.thingspeak.com\n";
        httpRequest += "Connection: close\n";
        httpRequest += "X-THINGSPEAKAPIKEY: " + apiKey + "\n";
        httpRequest += "Content-Type: application/x-www-form-urlencoded\n";
        httpRequest += "Content-Length: " + String(postStr.length()) + "\n\n";
        httpRequest += postStr;

        // Send the HTTP request
        client.print(httpRequest);
        Serial.println("Sending to ThingSpeak.");
      }
      client.stop();
      Serial.println("Waiting...");
      // thingspeak needs minimum 15 sec delay between updates
      delay(10);
      newDataReady = 0;
      t = millis();
    }
  }
}
