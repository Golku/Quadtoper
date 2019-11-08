#ifndef rtcModuleController_h
#define rtcModuleController_h

class rtcclass{
    public:
        rtcclass();
        void setupRtcModule();
        char* getTime();
};

extern rtcclass rtcModule;

#endif