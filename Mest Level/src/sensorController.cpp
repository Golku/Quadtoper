#include <Arduino.h>
#include <sensorController.h>
#include <Adafruit_VL53L0X.h>

// address we will assign if dual sensor is present
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31

// set the pins to shutdown
#define SHT_LOX1 25
#define SHT_LOX2 26

Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
// Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();

int distance = 0;


sensorclass::sensorclass(){
}

/*
void setID() {
  // all reset
  digitalWrite(SHT_LOX1, LOW);    
  digitalWrite(SHT_LOX2, LOW);
  delay(10);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  // activating LOX1 and reseting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);

  // initing LOX1
  if(!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while(1);
  }else{
    Serial.println(F("first sensor started"));
  }
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  //initing LOX2
  if(!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while(1);
  }else{
    Serial.println(F("second sensor started"));
  }
}

void read_dual_sensors() {
  
  lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!

  // print sensor one reading
  Serial.print("1: ");
  if(measure1.RangeStatus != 4) {     // if not out of range
    Serial.print(measure1.RangeMilliMeter);
  } else {
    Serial.print("Out of range");
  }
  
  Serial.print(" ");

  // print sensor two reading
  Serial.print(" 2: ");
  if(measure2.RangeStatus != 4) {
    Serial.print(measure2.RangeMilliMeter);
  } else {
    Serial.print("Out of range");
  }
  
  Serial.println();
}
*/

void sensorclass::setup(){
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  if (!lox1.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }

  // pinMode(SHT_LOX1, OUTPUT);
  // pinMode(SHT_LOX2, OUTPUT);

  // Serial.println("Shutdown pins inited...");

  // digitalWrite(SHT_LOX1, LOW);
  // digitalWrite(SHT_LOX2, LOW);

  // Serial.println("Both in reset mode...(pins are low)");
  
  // Serial.println("Starting...");
  // setID();
}

int sensorclass::getDistance(){
  
    VL53L0X_RangingMeasurementData_t measure;

    //Serial.print("Reading a measurement... ");
    lox1.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
        distance = measure.RangeMilliMeter;
    } else {
        //Serial.println(" out of range ");
    }

    return distance;
}

sensorclass sensor = sensorclass();