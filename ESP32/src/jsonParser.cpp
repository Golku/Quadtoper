#include <Arduino.h>
#include <jsonParser.h>

jclass::jclass(){

}


void jclass::setup(){
    Serial.println("setting up");
    printOnScreen();
}

void jclass::printOnScreen(){
    Serial.println("printing");
}


jclass json = jclass();