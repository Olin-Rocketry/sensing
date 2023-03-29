/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  This example code is in the public domain.
 */

// Pin 13 has an LED connected on most Arduino boards.
// Pin 11 has the LED on Teensy 2.0
// Pin 6  has the LED on Teensy++ 2.0
// Pin 13 has the LED on Teensy 3.0
// give it a name:
const int num_leds = 5;

const int leds[5] = {13, 6, 7, 8, 9};

const int ematch_1 = 38;
const int ematch_2 = 39;

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  for (int i = 0; i < num_leds; i++) {
    pinMode(leds[i], OUTPUT);
  }
  pinMode(ematch_1, OUTPUT);
  pinMode(ematch_2, OUTPUT);

  digitalWrite(ematch_1, LOW);
  digitalWrite(ematch_2, LOW);
}

void blinkLeds(int delayTime) {

  for (int i = 0; i < num_leds; i++) {
    digitalWrite(leds[i], HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(delayTime);               // wait for a second
    digitalWrite(leds[i], LOW);    // turn the LED off by making the voltage LOW
    delay(delayTime);               // wait for a second
  }
}

void writeWithDelay(int pin) {
  int ledCycles = 4;
  int cycleTime = 300;

  digitalWrite(pin, HIGH);
  for (int i = 0; i < ledCycles; i++) {
    blinkLeds(cycleTime);
  }

  digitalWrite(pin, LOW);
  // for (int i = 0; i < ledCycles; i++) {
  //   blinkLeds(cycleTime);
  // }
}

// the loop routine runs over and over again forever:
void loop() {
  // writeWithDelay(ematch_1);
  // writeWithDelay(ematch_2);
}