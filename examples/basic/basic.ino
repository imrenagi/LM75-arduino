#include <lm75.h>

Concrete_LM75 temperature;

void setup() {
  while(!Serial) {}
  Serial.begin(9600);
  Wire.begin();
  temperature.setOverTemperatureShutdown(30.0);
  temperature.setHysteresisTemperature(20.0);
}

void loop() {
  Serial.print(temperature.readTemperature());
  Serial.print(" ");
  Serial.print(temperature.readOverTemperatureShutdown());
  Serial.print(" ");
  Serial.print(temperature.readHysteresisTemperature());
  Serial.println();
  delay(250);
}
