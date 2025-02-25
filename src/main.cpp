#include <Arduino.h>

const int analogPin = A0;
int sensorValue = 0;

unsigned long lastTime = 0;
unsigned long debounceDelay = 50;


void setupWiFi();
void eventUpdate();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorValue = analogRead(analogPin);

  float voltage = (sensorValue * 3.3) / 1023;
  if(voltage > 0.07f)
  {
    if((millis() - lastTime) > debounceDelay)
    {
      lastTime = millis();
      eventUpdate();
    }
  }
}

void eventUpdate()
{
  Serial.println("Event!");
  delay(100);
}
