#include "buzzer_class.h"

Buzzer::Buzzer(byte pin) {
    this->pin = pin;
    pinMode(pin, OUTPUT);
}

void Buzzer::buzz(int frequency, int duration) {
    tone(pin, frequency, duration);
}

void Buzzer::stop() {
    noTone(pin);
}
