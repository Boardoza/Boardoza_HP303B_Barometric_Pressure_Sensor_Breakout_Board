#include <SPI.h>
#include "Boardoza_HP303B.h"

#define HP303B_CS_PIN 5

Boardoza_HP303B sensor;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Initializing Boardoza HP303B...");
  
  sensor.begin(SPI, HP303B_CS_PIN);
  sensor.correctTemp();
  
  delay(100);
  Serial.println("Sensor Ready!");
}

void loop() {
  sensor.setOpMode(CMD_TEMP);
  delay(50);
  int32_t temperature = sensor.measureTemperature();

  sensor.setOpMode(CMD_PRES);
  delay(50);
  int32_t pressure = sensor.measurePressure();

  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" C\t");

  Serial.print("Press: ");
  Serial.print(pressure);
  Serial.println(" Pa");

  delay(1000);
}