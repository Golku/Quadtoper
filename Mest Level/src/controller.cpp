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

/*
void i2cScanner(){

  byte error, address;

  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;

  for(address = 1; address < 127; address++ ) {

    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {

      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;

    }else if (error==4) {

      Serial.print("Unknow error at address 0x");

      if (address<16) {
        Serial.print("0");
      }

      Serial.println(address,HEX);

    }    
  }

  if (nDevices == 0) {

    Serial.println("No I2C devices found\n");

  }
  else {

    Serial.println("done\n");

  }

  delay(5000);

}
*/

void setup() {

  // Start Serial port
  Serial.begin(115200);
  
  sensor.setup();
  //rtcModule.setupRtcModule();
  //sdModule.setupSdModule();
  server.connect();

  // Serial.printf("Current time: %s \n", rtcModule.getTime());
  // Serial.printf("Distance: %d \n", sensor.getDistance());
}

void loop() {
  server.socketLoop();
  
  currentTime = millis();

  filterObject.in(sensor.getDistance());
  
  /*
  if(currentTime > nextSend){

    messuredDistance = filterObject.out();

    //Serial.print("Distance before (mm): "); Serial.print(messuredDistance);

    if(messuredDistance > maxDistance){
      messuredDistance = maxDistance;
    }


    //Serial.print("  Distance after (mm): "); Serial.println(messuredDistance);

    floaterHeight = maxDistance - messuredDistance;

    Serial.printf("Current time: %s \n", rtcModule.getTime());

    //Serial.print("Floater height before (mm): "); Serial.print(floaterHeight);
    Serial.print("Floater height after  (mm): "); Serial.println(calibration(floaterHeight));
    //Serial.print("  L: "); Serial.println(  ((565 * 364 * calibration(floaterHeight)) / 1000000) + 2);

    sdModule.logData(rtcModule.getTime(), calibration(floaterHeight));;

    nextSend = currentTime + 1000; 
  }
  */

  if(server.isConnected()){

    if(currentTime > nextSend){

      messuredDistance = filterObject.out();

      if(messuredDistance > maxDistance){
        messuredDistance = maxDistance;
      }

      floaterHeight = maxDistance - messuredDistance;

      dataOut(calibration(floaterHeight));

      nextSend = currentTime + 1000;
    }
  }
}

controllerclass controller = controllerclass();