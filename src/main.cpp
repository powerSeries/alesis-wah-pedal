#include <Arduino.h>
#include <Pedal.h>

const int analogPin = A0;
const int btnPin = D1;

int sensorValue = 0;

// 0 -> Normal pedal for switching between the 2 FX's
// 1 -> Pedal becomes a wah pedal
int pedalMode = 0;
int btnState = 0;

unsigned long lastTime = 0;
unsigned long debounceDelay = 500;

float voltage = 0.0f;

int wahPercent = 0;
float lastVoltage = 0.0f;
int direction = 0; // 0 -> down, 1 -> up

void setupWiFi();
void eventUpdate();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(analogPin, INPUT);
  pinMode(btnPin, INPUT_PULLUP);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorValue = analogRead(analogPin);
  btnState = digitalRead(btnPin);

  if(btnState == LOW)
  {
    if((millis() - lastTime) > debounceDelay)
    {
      lastTime = millis();
      pedalMode = !pedalMode;
      Serial.print("Pedal mode: ");
      Serial.println(pedalMode);
      delay(100);
    }
  }

  voltage = (sensorValue * 3.3) / 1023;
  if(pedalMode == PEDAL_SWITCH && voltage > PEDAL_VOLTAGE_THRESHOLD)
  {
    if((millis() - lastTime) > debounceDelay)
    {
      lastTime = millis();
      eventUpdate();
    }
  }
  else if(pedalMode == PEDAL_WAH)
  {
      eventUpdate();
  }
}

void eventUpdate()
{
  switch (pedalMode)
  {
  case PEDAL_SWITCH:
    Serial.println("Event!");
    break;

  case PEDAL_WAH:
    if (voltage > PEDAL_VOLTAGE_THRESHOLD)
    {
      direction = 1;
    }
    else
    {
      direction = 0;
    }
    if (direction)
    {
      wahPercent += PEDAL_RATE_OF_CHANGE;
      if (wahPercent > 100)
        wahPercent = 100;
    }
    else
    {
      wahPercent -= PEDAL_RATE_OF_CHANGE;
      if (wahPercent < 0)
        wahPercent = 0;
    }
    Serial.print("Wah: ");
    Serial.println(wahPercent);
    break;

  default:
    break;
  }


  delay(100);
}
