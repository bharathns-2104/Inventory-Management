#include <HX711_ADC.h>

#define DT_PIN  26  // GPIO pin for data (DT)
#define SCK_PIN  27 // GPIO pin for clock (SCK)

HX711_ADC scale;

void setup() {
  Serial.begin(9600);
  scale.begin(DT_PIN, SCK_PIN);
}

void loop() {
  Serial.println(scale.read());
  delay(1000);
}
