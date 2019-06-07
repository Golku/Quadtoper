#include <Arduino.h>
#include <sensorController.h>

sensorclass::sensorclass(){

}

void sensorclass::setup(){
    printOnScreen();
}


sensorclass sensors = sensorclass();