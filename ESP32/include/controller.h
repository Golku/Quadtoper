#ifndef controller_h
#define controller_h

class controllerclass{
    public:
        controllerclass();
        void setup();
        void dataIn(uint8_t * data);
};

extern controllerclass controller;

#endif