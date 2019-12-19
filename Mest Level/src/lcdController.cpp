#include <Arduino.h>
#include <lcdController.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcdObject(0x27, 2,1,0,4,5,6,7,3, POSITIVE);  // Set the LCD I2C address

lcdclass::lcdclass(){
}

void lcdclass::setup(){
    lcdObject.begin(16,2);               // initialize the lcd 
    lcdObject.clear();
    lcdObject.setCursor(0,0);
    lcdObject.print("Height:");
    lcdObject.setCursor(0,1);
    lcdObject.print("Liters:");
}

void lcdclass::display(double floaterHeight, double liters){
    lcdObject.clear();
    lcdObject.setCursor(0,0);
    lcdObject.print("Height:");
    lcdObject.setCursor(0,1);
    lcdObject.print("Liters:");
    lcdObject.setCursor(7,0);
    lcdObject.print(floaterHeight);
    lcdObject.setCursor(7,1);
    lcdObject.print(liters);
}

lcdclass lcd = lcdclass();