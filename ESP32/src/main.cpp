#include <Arduino.h>
#include <ArduinoJson.h>

DynamicJsonDocument readDoc(1024);
DynamicJsonDocument writeDoc(1024);

int target;

/*
target:
  1 esp
  2 motorLeft1
  3 motorLeft2
  4 motorRight1
  5 motorRight2

openration:

esp(1):
  1 powerOn: (true - false)

motoren(2,3,4,5):
  1 speed: (0 - 100)
*/

void espCommand(){
  Serial.print("ESP command");
}

void motorCommand(){
  Serial.print("motor command");
}

char json[] = "{\"target\":\"2\",\"status\":true,\"data\":[48.756080,2.302038]}";

void readJson(){

  deserializeJson(readDoc, json);
  target = readDoc["target"];

  switch (target)
  {
    case 1:

      // if(){

      // }

      espCommand();
      break;
    case 2:
      motorCommand();
      break;
    default:
      break;
  }

}

void writeJson(){

  writeDoc["motorLeft1"] = 56;
  writeDoc["motorLeft2"] = 25;

  size_t outputSize = 256;
  char* output = new char[outputSize];
  
  serializeJson(writeDoc, output, outputSize);

  uint8_t * h = (uint8_t *)output;

  Serial.printf("json: %s\n", h);
}

void setup()
{
    Serial.begin(115200);
    readJson();
}

void loop() {
    // put your main code here, to run repeatedly:
}