#include <Arduino.h>
#include <sensorController.h>
#include <Adafruit_VL53L0X.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
int distance = 0;

sensorclass::sensorclass(){
}

void sensorclass::setup(){
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
}

int sensorclass::getDistance(){
  
    VL53L0X_RangingMeasurementData_t measure;

    //Serial.print("Reading a measurement... ");
    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
        distance = measure.RangeMilliMeter;
    } else {
        //Serial.println(" out of range ");
    }

    return distance;
}

sensorclass sensor = sensorclass();