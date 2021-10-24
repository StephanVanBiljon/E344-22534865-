//Stéphan van Biljon - 22534865

#include "Keyboard.h"
 
float supplyVoltage = 0.0;
float batteryVoltage = 0.0;
float batteryCurrent = 0.0;
byte lightLevel = 0.0;
float supplyScaleFactor = 0.2305;
float batteryScaleFactor = 2.3315;
float lightScaleFactor = 23.641;
float brightVoltage = 0.2;
byte ledBrightness = 255;
bool ledState = false;
  
void setup() {
  Serial.begin(9600); // sets up communication between PC and Arduino (Do I need these two lines when I have python setting it up?
  Keyboard.begin(); // This line as well
  pinMode(11, OUTPUT);
  pinMode(9, OUTPUT);
}

void loop() {
  supplyVoltage = ((float)(analogRead(10)) * (5.0/1023.0)) * (1/supplyScaleFactor); // You need to measure the Vadc when the supply is 22V
  
  lightLevel = (byte)(((analogRead(A2) * (5.0/1023.0)) - brightVoltage) * lightScaleFactor); // percentage of max ambient light cast to a byte

  batteryVoltage = (float)(((analogRead(A1) * (5.0/1023.0)) - 0.237) / batteryScaleFactor) + 5.5;

  batteryCurrent = (float)(((analogRead(A0) * (5.0/1023.0)) - 1.75)/(50.0 * 0.1)) * 1000.0; //mA

  if(Serial.available() > 0)
  {
    //read the incoming string
    String userInput = Serial.readStringUntil('\n'); // value between 0-100 or "Toggle Charging" or nothing

    if(userInput == "Toggle Charging")
    {
      ledState = !ledState;
      digitalWrite(11, ledState);
    }
    else if(userInput.length() != 0)
    {
      ledBrightness = (byte)(((float)(userInput.toInt())/100.0) * 255.0); //0-255 decides PWM duty cycle (and LED brightness) therefore we need a certain percentage of 255 to set the brightness
      analogWrite(9, ledBrightness);
    } 
  }

  Serial.println(batteryVoltage);
  Serial.println(batteryCurrent);
  Serial.println(supplyVoltage);
  Serial.println(lightLevel);

  delay(1000);
}
