#pragma once
#include <Arduino.h>

class Buzzer {
private:
    byte pin;

public:
    Buzzer(byte pin);
    void buzz(int frequency, int duration);
    void stop();
};
