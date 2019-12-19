#ifndef lcdController_h
#define lcdController_h

class lcdclass{
    public:
        lcdclass();
        void setup();
        void display(double floaterHeight, double liters);
};

extern lcdclass lcd;

#endif