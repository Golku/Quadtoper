#ifndef sensorController_h
#define sensorController_h

class sensorclass{
    public:
        sensorclass();
        void setup();
        void getAngles();
};

extern sensorclass sensors;

#endif