#ifndef sdModuleController_h
#define sdModuleController_h

class sdclass{
    public:
        sdclass();
        void setupSdModule();
        void logData(char * timeString, int floaterHeight);
};

extern sdclass sdModule;

#endif