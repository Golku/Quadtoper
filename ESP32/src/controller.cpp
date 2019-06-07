#include <Arduino.h>
#include <ArduinoJson.h>
#include <controller.h>
#include <serverController.h>
#include <motorController.h>
#include <sensorController.h>

DynamicJsonDocument readDoc(1024);
DynamicJsonDocument writeDoc(1024);

bool powerOn;
bool pidOn;

int commandType;
int motorIndex;
int motorVal;
int targetVal;

/*
command:
  1 power (powerOn: true/false)
  2 pid   (pidOn: true/false)
  3 motor (motorIndex: 0,1,2,3 - motorVal: 0-100)
*/
controllerclass::controllerclass(){
}

void controllerclass::setup(){
    powerOn = false;
    pidOn = false;
}

void pidConfig(){
// targetVal = motorVal;
  Serial.print("targetVal: ");
  Serial.print(targetVal); 
}

void controllerclass::dataIn(uint8_t * data){
  //Serial.printf("In Controller: %s\n", data);

  deserializeJson(readDoc, data);
  
  commandType = readDoc["commandType"];

  switch (commandType){
    case 1:
      powerOn = readDoc["powerOn"];
      Serial.print("Power: ");
      Serial.println(powerOn);
      motors.power(powerOn);
      break;
    case 2:
      pidOn = readDoc["pidOn"];
      Serial.print("pid: ");
      Serial.println(pidOn);
      motors.setPid(pidOn);
      break;
    case 3:
      motorIndex = readDoc["motorIndex"];
      motorVal = readDoc["motorVal"];
      motors.changeMotorSpeed(motorIndex, motorVal);
      break;
  }
}

void dataOut(){
  
}

void setup() {

  // Start Serial port
  Serial.begin(115200);

  server.connect();
  motors.setup();
}

void loop() {

  server.socketLoop();
}

controllerclass controller = controllerclass();