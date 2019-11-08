#include <Arduino.h>
#include <rtcModuleController.h>
#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

rtcclass::rtcclass(){
}

void rtcclass::setupRtcModule(){
    if (! rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1);
    }

    if (rtc.lostPower()) {
        Serial.println("RTC lost power, lets set the time!");
        // following line sets the RTC to the date &amp; time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date &amp; time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2019, 11, 07, 19, 23, 0));
    }
}

char* rtcclass::getTime(){
    DateTime now = rtc.now();
    // char timeString[] = "hh:mm:ss - DD/MM/YY";
    char timeString[] = "hh:mm:ss";
    return now.toString(timeString);
}

rtcclass rtcModule = rtcclass();