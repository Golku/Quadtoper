#ifndef sensorController_h
#define sensorController_h

class sensorclass{
    public:
        sensorclass();
        void setup();
        int getDistance();
};

extern sensorclass sensor;

#endif