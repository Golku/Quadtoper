#ifndef serverController_h
#define serverController_h

class serverclass{
    public:
        serverclass();
        void connect();
        void socketLoop();
};

extern serverclass server;

#endif