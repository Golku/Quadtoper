#ifndef motorController_h
#define motorController_h

class motorclass{
    public:
        motorclass();
        void setup();
        void power(bool power);
        void setPid(bool pidOn);
        void changeMotorSpeed(int motorIndex, int motortVal);
};

extern motorclass motors;

#endif