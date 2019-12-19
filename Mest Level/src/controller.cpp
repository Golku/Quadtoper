#include <Arduino.h>
#include <ArduinoJson.h>
#include <controller.h>
#include <serverController.h>
#include <sensorController.h>
#include <rtcModuleController.h>
#include <sdModuleController.h>
#include <MedianFilter.h>
#include <lcdController.h>

DynamicJsonDocument readDoc(1024);
DynamicJsonDocument writeDoc(1024);

int command;
long currentTime;
long nextSend;
int messuredDistance;
int maxDistance = 426;
int offset = 0;
int floaterHeight = 0;
double floaterHeightCalibrated = 0;
double volumeInLiters = 0;

bool showDistance = false;

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

double calibration(int floaterHeight){

  if(floaterHeight > 15){
    offset = 5;
  }
      
  if(floaterHeight > 50){
    offset = 10;
  }

  if(floaterHeight > 145){
    offset = 15;
  }

  if(floaterHeight > 200){
    offset = 20;
  }

  if(floaterHeight <= 15){
    offset = 0;
  }

  return floaterHeight+offset;
}

void serialCommands(){
  if(Serial.available() > 0){
    char letter = Serial.read();

    if(letter == '0'){
      showDistance = true;
    }else if(letter == '1'){
      showDistance = false;
    }
  }
}

void setup() {

  Serial.begin(115200);

  sensor.setup();
  //rtcModule.setup();
  //sdModule.setup();
  lcd.setup();
  // server.connect();
}

void loop() {
  //server.socketLoop();
  serialCommands();
  
  currentTime = millis();

  filterObject.in(sensor.getDistance());

  // if(server.isConnected()){

    if(currentTime > nextSend){

      messuredDistance = filterObject.out();

      if(showDistance){
        Serial.print("Distance (mm): "); Serial.print(messuredDistance);
      }

      if(messuredDistance > maxDistance){
        messuredDistance = maxDistance;
      }

      if(showDistance){
        Serial.print(" Distance after (mm): "); Serial.print(messuredDistance);
      }

      floaterHeight = maxDistance - messuredDistance;

      if(showDistance){
        Serial.print(" Floater Height (mm): "); Serial.print(floaterHeight);
      }

      if(showDistance){
        Serial.print(" calibrated (mm): "); Serial.println(calibration(floaterHeight));
      }

      floaterHeightCalibrated = calibration(floaterHeight) + 15; //Water + 6 mm

      // volumeInLiters = ((565 * 364 * floaterHeightCalibrated) / 1000000); // 6 mm = 1.23
      volumeInLiters = (0.0005*(floaterHeightCalibrated * floaterHeightCalibrated)) + (0.47*floaterHeightCalibrated);

      //sdModule.logData(rtcModule.getTime(), volumeInLiters);

      lcd.display(floaterHeightCalibrated, volumeInLiters);

      // dataOut(floaterHeightCalibrated);
      
      nextSend = millis() + 1000;
    }
  // }

  if(currentTime > nextSend){

    // messuredDistance = filterObject.out();

    // Serial.print("Distance before (mm): "); Serial.println(messuredDistance);

    // if(messuredDistance > maxDistance){
    //   messuredDistance = maxDistance;
    // }

    // Serial.print("  Distance after (mm): "); Serial.println(messuredDistance);

    // floaterHeight = maxDistance - messuredDistance;

    // Serial.printf("Current time: %s \n", rtcModule.getTime());

    // Serial.print("Floater height before (mm): "); Serial.print(floaterHeight);
    // Serial.print("Floater height after  (mm): "); Serial.println(calibration(floaterHeight));
    // Serial.print("  L: "); Serial.println(  ((565 * 364 * calibration(floaterHeight)) / 1000000) + 2);
    
    // floaterHeight++;

    // floaterHeightCalibrated = calibration(floaterHeight);

    // volumeInLiters = ((565 * 364 * floaterHeightCalibrated) / 1000000) + 2;

    // Serial.print("Volume: "); Serial.println(volumeInLiters);

    // lcd.display(floaterHeightCalibrated, volumeInLiters);

    // sdModule.logData(rtcModule.getTime(), volumeInLiters);;

    // nextSend = currentTime + 1000; 
  }
}

controllerclass controller = controllerclass();