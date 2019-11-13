#include <Arduino.h>
#include <ArduinoJson.h>
#include <controller.h>
#include <serverController.h>
#include <sensorController.h>
#include <rtcModuleController.h>
#include <sdModuleController.h>
#include <MedianFilter.h>

//#include <Wire.h>

DynamicJsonDocument readDoc(1024);
DynamicJsonDocument writeDoc(1024);

int command;
long currentTime;
long nextSend;
int messuredDistance;
int floaterHeight = 0;
int floaterHeightCalibrated = 0;
int maxDistance = 426;
int offset = 0;

MedianFilter filterObject(10, 0);

controllerclass::controllerclass(){
}

void dataOut(int floaterHeight){

  writeDoc["floaterHeightMm"] = floaterHeight;

  size_t outputSize = 256;
  char* output = new char[outputSize];
  
  serializeJson(writeDoc, output, outputSize);

  uint8_t * data = (uint8_t *)output;

  server.sendJson(data);
}

void controllerclass::dataIn(uint8_t * data){
  //Serial.printf("In Controller: %s\n", data);

  deserializeJson(readDoc, data);
  
  command = readDoc["command"];

  //Serial.printf("In Controller: %d\n", command);

  if(command == 1){
     Serial.printf("Command: %d\n", command);
    //dataOut();
  }else{
     Serial.printf("Command: %d\n", command);
  }

}

int calibration(int floaterHeight){

  if(floaterHeight > 15){
    offset = 5;
  }
      
  if(floaterHeight > 50){
    offset = 10;
  }

  if(floaterHeight > 168){
    offset = 15;
  }

  if(floaterHeight > 220){
    offset = 20;
  }

  if(floaterHeight > 290){
    offset = 15;
  }

  if(floaterHeight <= 15){
    offset = 0;
  }

  return floaterHeight+offset;
}

void setup() {

  // Start Serial port
  Serial.begin(115200);
  
  sensor.setup();
  //rtcModule.setup();
  //sdModule.setup();
  //server.connect();
}

void loop() {
  server.socketLoop();
  
  currentTime = millis();

  filterObject.in(sensor.getDistance());

  if(server.isConnected()){

    if(currentTime > nextSend){

      messuredDistance = filterObject.out();

      if(messuredDistance > maxDistance){
        messuredDistance = maxDistance;
      }

      floaterHeight = maxDistance - messuredDistance;

      floaterHeightCalibrated = calibration(floaterHeight);

      // sdModule.logData(rtcModule.getTime(), floaterHeightCalibrated);

      sdModule.logData(rtcModule.getTime(), ((565 * 364 * floaterHeightCalibrated) / 1000000) + 2);

      dataOut(floaterHeightCalibrated);
      
      nextSend = millis() + 1000;
    }
  }

  if(currentTime > nextSend){

    messuredDistance = filterObject.out();

    Serial.print("Distance before (mm): "); Serial.print(messuredDistance);

    if(messuredDistance > maxDistance){
      messuredDistance = maxDistance;
    }

    Serial.print("  Distance after (mm): "); Serial.println(messuredDistance);

    floaterHeight = maxDistance - messuredDistance;

    // Serial.printf("Current time: %s \n", rtcModule.getTime());

    // Serial.print("Floater height before (mm): "); Serial.print(floaterHeight);
    // Serial.print("Floater height after  (mm): "); Serial.println(calibration(floaterHeight));
    //Serial.print("  L: "); Serial.println(  ((565 * 364 * calibration(floaterHeight)) / 1000000) + 2);

    //sdModule.logData(rtcModule.getTime(), 28);;

    nextSend = currentTime + 1000; 
  }
}

controllerclass controller = controllerclass();