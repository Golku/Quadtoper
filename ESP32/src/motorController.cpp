#include <Arduino.h>
#include <motorController.h>

const int freq = 50;
const int channel1 = 0;
const int channel2 = 1;
const int channel3 = 2;
const int channel4 = 3;
const int resolution = 11;
const int motorPin1 = 33;
const int motorPin2 = 32;
const int motorPin3 = 25;
const int motorPin4 = 26;
const int multiplier = 101;

int motorFLVal;
int motorFRVal;
int motorBLVal;
int motorBRVal;

motorclass::motorclass(){

}

void motorclass::power(bool power){

  if(power){
    ledcWrite(channel1, multiplier);
    ledcWrite(channel2, multiplier);
    ledcWrite(channel3, multiplier);
    ledcWrite(channel4, multiplier);
  }else{
    ledcWrite(channel1, 0);
    ledcWrite(channel2, 0);
    ledcWrite(channel3, 0);
    ledcWrite(channel4, 0);
  }

}

void motorclass::setup(){

    ledcSetup(channel1, freq, resolution);
    ledcSetup(channel2, freq, resolution);
    ledcSetup(channel3, freq, resolution);
    ledcSetup(channel4, freq, resolution);

    ledcAttachPin(motorPin1, channel1);
    ledcAttachPin(motorPin2, channel2);
    ledcAttachPin(motorPin3, channel3);
    ledcAttachPin(motorPin4, channel4);

    motorFLVal = 101;
    motorFRVal = 101;
    motorBLVal = 101;
    motorBRVal = 101;

    ledcWrite(channel1, motorFLVal);
    ledcWrite(channel2, motorFRVal);
    ledcWrite(channel3, motorBLVal);
    ledcWrite(channel4, motorBRVal);
}

void motorclass::setPid(bool pidOn){

}

void motorclass:: changeMotorSpeed(int motorIndex, int motorVal){
  switch (motorIndex){
    case 0:
      motorFLVal = motorVal;
      Serial.print("motorFLVal: ");
      Serial.println(motorFLVal);
      ledcWrite(channel1, (motorFLVal+multiplier));
      break;
    case 1:
      motorFRVal = motorVal;
      Serial.print("motorFRVal: ");
      Serial.println(motorFRVal);
      ledcWrite(channel2, (motorFRVal+multiplier));
      break;
    case 2:
      motorBLVal = motorVal;
      Serial.print("motorBLVal: ");
      Serial.println(motorBLVal);
      ledcWrite(channel3, (motorBLVal+multiplier));
      break;  
    case 3:
      motorBRVal = motorVal;
      Serial.print("motorBRVal: ");
      Serial.println(motorBRVal);
      ledcWrite(channel4, (motorBRVal+multiplier));
      break;
    case 4:

    //   if(pidOn){
    //     pidConfig();
    //   }else{
    //     motorFLVal = motorVal;
    //     motorFRVal = motorVal;
    //     motorBLVal = motorVal;
    //     motorBRVal = motorVal;

    //     ledcWrite(channel1, (motorFLVal+multiplier));
    //     ledcWrite(channel2, (motorFRVal+multiplier));
    //     ledcWrite(channel3, (motorBLVal+multiplier));
    //     ledcWrite(channel4, (motorBLVal+multiplier));

    //     Serial.print("All motors vals: ");
    //     Serial.print(motorVal);
    //   }

      break;  
  }
}

motorclass motors = motorclass();