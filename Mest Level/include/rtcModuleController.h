#ifndef rtcModuleController_h
#define rtcModuleController_h

class rtcclass{
    public:
        rtcclass();
        void setup();
        char* getTime();
};

extern rtcclass rtcModule;

#endif