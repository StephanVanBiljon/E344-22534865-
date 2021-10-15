//Stéphan van Biljon - 22534865

#include "Keyboard.h"
 
float supplyVoltage = 0.0;
float batteryVoltage = 0.0;
float batteryCurrent = 0.0;
bool OCPS = false; // Overcharge protection status
byte lightLevel = 0.0;
float supplyScaleFactor = 0.2305;
float batteryScaleFactor = 2.3315;
float lightScaleFactor = 23.641;
float brightVoltage = 0.2;
  
void setup() {
  Serial.begin(9600); // sets up communication between PC and Arduino
  Keyboard.begin();
  pinMode(11, OUTPUT);
}

void loop() {
  supplyVoltage = ((float)(analogRead(10)) * (5.0/1023.0)) * (1/supplyScaleFactor); // You need to measure the Vadc when the supply is 22V
  
  lightLevel = (byte)(((analogRead(A2) * (5.0/1023.0)) - brightVoltage) * lightScaleFactor); // percentage of max ambient light cast to a byte

  batteryVoltage = (float)(((analogRead(A1) * (5.0/1023.0)) - 0.237) / batteryScaleFactor) + 5.5;

  batteryCurrent = (float)(((analogRead(A0) * (5.0/1023.0)) - 1.75)/(50.0 * 0.1)) * 1000.0; //mA

  if(Serial.available() > 0)
  {
    //read the incoming string
    String userInput = Serial.readString();

    if((userInput[0] == 'O') && (userInput[1] == 'V') && (userInput[2] == '1'))
    {
      digitalWrite(11, HIGH);
      OCPS = true;
    }
    else if((userInput[0] == 'O') && (userInput[1] == 'V') && (userInput[2] == '0'))
    {
      digitalWrite(11, LOW);
      OCPS = false;
    }
  }

  String comma = ",";

  Serial.println(OCPS + comma + batteryVoltage + comma + supplyVoltage + comma + batteryCurrent + comma + lightLevel);

  delay(1000);

}
