#ifndef serverController_h
#define serverController_h

class serverclass{
    public:
        serverclass();
        void connect();
        void socketLoop();
        void sendJson(uint8_t * dataOut);
        bool isConnected();
};

extern serverclass server;

#endif