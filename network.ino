#include <WiFi.h>
 
const char* ssid = "OnePlus Nord"; //choose your wireless ssid
const char* password =  "Bharath211204"; //put your wireless password here.
 
void setup() {
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("WiFi status: ");
    Serial.println(WiFi.status());
    Serial.println("Connecting to WiFi..");
  }

 
  Serial.println("Connected to the WiFi network");
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); //show ip address when connected on serial monitor.
}
 
void loop() {}
